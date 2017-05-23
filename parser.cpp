/*
 * =====================================================================================
 *
 *       Filename:  parser.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  26.04.2017 21:31:56
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>

#include "parser.h"

void Parser::initParser() {
	expected_tokens[GLOBAL_SCOPE] = { Token::STRING, Token::DEFINE };
	expected_tokens[GLOBAL_SCOPE_KEY] = { Token::KEY_VALUE_SEPARATOR };
	expected_tokens[GLOBAL_SCOPE_VALUE] = { Token::STRING };
	expected_tokens[DEFINE] = { Token::HOST, Token::SERVICE, Token::SERVICEGROUP, Token::CONTACT,
		Token::HOSTGROUP, Token::COMMANDGROUP, Token::COMMAND, Token::TIMEPERIOD };
	expected_tokens[DEFINED_OBJECT] = { Token::OBJECT_START };
	expected_tokens[INSIDE_OBJECT] = { Token::STRING, Token::LONG_VALUE,
		Token::USE, Token::MEMBERS, Token::OBJECT_END };
	expected_tokens[INSIDE_OBJECT_KEY] = { Token::STRING };
	expected_tokens[INSIDE_OBJECT_VALUE] = { Token::STRING, Token::LONG_VALUE, 
		Token::USE, Token::MEMBERS, Token::OBJECT_END, Token::VALUE_SEPARATOR };
	expected_tokens[INSIDE_OBJECT_LONG_VALUE] = { Token::STRING, Token::OBJECT_END, Token::LONG_VALUE,
		Token::USE, Token::MEMBERS, Token::VALUE_SEPARATOR, Token::KEY_VALUE_SEPARATOR };
	expected_tokens[INSIDE_OBJECT_USE] = { Token::STRING };
	expected_tokens[INSIDE_OBJECT_NEXT_USE] = { Token::VALUE_SEPARATOR, Token::OBJECT_END,
		Token::STRING, Token::USE, Token::MEMBERS, Token::LONG_VALUE };
	expected_tokens[INSIDE_OBJECT_MEMBER] = { Token::STRING };
	expected_tokens[INSIDE_OBJECT_NEXT_MEMBER] = { Token::VALUE_SEPARATOR, Token::OBJECT_END,
		Token::STRING, Token::USE, Token::MEMBERS, Token::LONG_VALUE };


	semantic_actions[Token::STRING] = std::bind( &Parser::onString, this );
	semantic_actions[Token::DEFINE] = std::bind( &Parser::onDefine, this );
	semantic_actions[Token::HOST] = std::bind( &Parser::onHost, this );
	semantic_actions[Token::HOSTGROUP] = std::bind( &Parser::onHostgroup, this );
	semantic_actions[Token::SERVICE] = std::bind( &Parser::onService, this );
	semantic_actions[Token::TIMEPERIOD] = std::bind( &Parser::onTimeperiod, this );
	semantic_actions[Token::OBJECT_START] = std::bind( &Parser::onObjectStart, this );
	semantic_actions[Token::OBJECT_END] = std::bind( &Parser::onObjectEnd, this );
	semantic_actions[Token::COMMENT] = std::bind( &Parser::onComment, this );
	semantic_actions[Token::COMMAND] = std::bind( &Parser::onCommand, this );
	semantic_actions[Token::CONTACT] = std::bind( &Parser::onContact, this );
	semantic_actions[Token::LONG_VALUE] = std::bind( &Parser::onLongValue, this );
	semantic_actions[Token::MEMBERS] = std::bind( &Parser::onMembers, this );
	semantic_actions[Token::USE] = std::bind( &Parser::onUse, this );
	semantic_actions[Token::VALUE_SEPARATOR] = std::bind( &Parser::onValueSeparator, this );
	semantic_actions[Token::KEY_VALUE_SEPARATOR] = std::bind( &Parser::onKeyValueSeparator, this );

	object_id = 0;
}

void Parser::printInfo() const
{
	for( const auto& h : hosts )
	{
		h->printInfo();
	}
	for( const auto& hg : hostgroups )
	{
		hg->printInfo();
	}
	for( const auto &s : services )
	{
		s->printInfo();
	}
}

bool Parser::isTokenTypeExpected(const Token::TYPE& type)
{
	if( type == Token::COMMENT ) return true;
	return expected_tokens[state].count( type ) != 0;
}

void Parser::parseSource( Source& source, Lexer& lexer) {
	file = source.getSourceName();
	std::cout<<file<<std::endl;
	row = 0;
	state = GLOBAL_SCOPE;
	token = lexer.getNextToken(source);
	while( token.getType() != Token::END_OF_FILE ) {
		if( !isTokenTypeExpected( token.getType() ) ) {
			throw UnexpectedTokenException( token.getTypeString() );
		}

		semantic_actions[ token.getType() ]();
		row = token.getRow();
		token = lexer.getNextToken(source);
	}
}

void Parser::onString() {
	if( state == STATE::GLOBAL_SCOPE ) {
		key = token.getValue();
		state = STATE::GLOBAL_SCOPE_KEY;
		return;
	}

	if( state == STATE::GLOBAL_SCOPE_VALUE ) {
		value = token.getValue();
		state = STATE::GLOBAL_SCOPE;
		global_object.addKeyValue( key, value );
	}

	if( state == STATE::INSIDE_OBJECT ) {
		key = token.getValue();
		state = STATE::INSIDE_OBJECT_KEY;
		return;
	}

	if( state == STATE::INSIDE_OBJECT_KEY ) {
		value += token.getValue();
		state = STATE::INSIDE_OBJECT_VALUE;
		return;
	}

	if( state == STATE::INSIDE_OBJECT_VALUE ) {
		object->addKeyValue( key, value );
		key = token.getValue();
		value = std::string();
		state = STATE::INSIDE_OBJECT_KEY;
		return;
	}

	if( state == STATE::INSIDE_OBJECT_USE ) {
		object->addUse( token.getValue() );
		state = STATE::INSIDE_OBJECT_NEXT_USE;
		return;
	}

	if( state == STATE::INSIDE_OBJECT_NEXT_USE ) {
		state = INSIDE_OBJECT_KEY;
		key = token.getValue();
		return;
	}

	if( state == STATE::INSIDE_OBJECT_MEMBER ) {
		object->addMember( token.getValue() );
		state = STATE::INSIDE_OBJECT_NEXT_MEMBER;
		return;
	}

	if( state == STATE::INSIDE_OBJECT_NEXT_MEMBER ) {
		key = token.getValue();
		state = STATE::INSIDE_OBJECT_KEY;
		return;
	}

	if( state == STATE::INSIDE_OBJECT_LONG_VALUE ) {
		if( token.getRow() == row ) {
			value = value + token.getValue() + " ";
			return;
		}
		else {
			object->addKeyValue( key, value );
			key = token.getValue();
			state = STATE::INSIDE_OBJECT_KEY;
			return;
		}
	}
}

void Parser::onDefine() {
	state = STATE::DEFINE;
}

void Parser::onHost() {
	hosts.push_back( new Host() );
	object = hosts.back();
	state = STATE::DEFINED_OBJECT;
}

void Parser::onHostgroup() {
	hostgroups.push_back( new Hostgroup() );
	object = hostgroups.back();
	state = STATE::DEFINED_OBJECT;
}

void Parser::onService() {
	services.push_back( new Service() );
	object = services.back();
	state = STATE::DEFINED_OBJECT;
}

void Parser::onServicegroup() {
	servicegroups.push_back( new Servicegroup() );
	object = servicegroups.back();
	state = STATE::DEFINED_OBJECT;
}

void Parser::onCommand() {
	commands.push_back( new Command() );
	object = commands.back();
	state = STATE::DEFINED_OBJECT;
}

void Parser::onCommandgroup() {
	commandgroups.push_back( new Commandgroup() );
	object = commandgroups.back();
	state = STATE::DEFINED_OBJECT;
}

void Parser::onContact() {
	contacts.push_back( new Contact() );
	object = contacts.back();
	state = STATE::DEFINED_OBJECT;
}

void Parser::onTimeperiod() {
	timeperiods.push_back( new Timeperiod() );
	object = timeperiods.back();
	state = STATE::DEFINED_OBJECT;
}

void Parser::onObjectStart() {
	object->setID( object_id );
	++object_id;
	value = std::string();
	state = STATE::INSIDE_OBJECT;
}

void Parser::onObjectEnd() {
	if( state == STATE::INSIDE_OBJECT_VALUE ) {
		object->addKeyValue( key, value );
	}

	if( state == STATE::INSIDE_OBJECT_LONG_VALUE ) {
		addLongValue();
	}


	state = STATE::GLOBAL_SCOPE;
	object = nullptr;
}

void Parser::onComment() {

}

void Parser::onLongValue() {
	if( state == STATE::INSIDE_OBJECT_VALUE ) {
		object->addKeyValue( key, value );
	}

	if( state == INSIDE_OBJECT_LONG_VALUE ) {
		addLongValue();
	}

	key = token.getValue();
	value = std::string();
	state = STATE::INSIDE_OBJECT_LONG_VALUE;
}

void Parser::addLongValue() {
	if( value.empty() ) {
		throw UnexpectedTokenException("Empty long value!");
	}

	object->addKeyValue( key ,value );
}

void Parser::onMembers() {
	if( state == STATE::INSIDE_OBJECT_VALUE ) {
		object->addKeyValue( key, value );
	}

	if( state == STATE::INSIDE_OBJECT_LONG_VALUE ) {
		addLongValue();
	}

	state = STATE::INSIDE_OBJECT_MEMBER;
}

void Parser::onUse() {
	if( state == STATE::INSIDE_OBJECT_VALUE ) {
		object->addKeyValue( key, value );
	}

	if( state == STATE::INSIDE_OBJECT_LONG_VALUE ) {
		addLongValue();
	}

	state = STATE::INSIDE_OBJECT_USE;
}

void Parser::onValueSeparator() {
	if( state == STATE::INSIDE_OBJECT_VALUE ) {
		value += token.getValue() + " ";
		state = STATE::INSIDE_OBJECT_KEY;
		return;
	}

	if( state == STATE::INSIDE_OBJECT_NEXT_MEMBER ) {
		state = STATE::INSIDE_OBJECT_MEMBER;
		return;
	}

	if( state == STATE::INSIDE_OBJECT_NEXT_USE ) {
		state = STATE::INSIDE_OBJECT_USE;
		return;
	}
}

void Parser::onKeyValueSeparator() {
	if( state == STATE::INSIDE_OBJECT_LONG_VALUE ) {
		value += token.getValue();
		return;
	}
	state = STATE::GLOBAL_SCOPE_VALUE;
}

void Parser::printStatus() const
{
	std::cout<<"Parser status"<<std::endl;
	std::cout<<"Current file: "<<file<<std::endl;
	std::cout<<"Current row: "<<row<<std::endl;
}
