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


void Parser::parseSource( Source source) {
	Token token = lexer.getNextToken();
	std::string key;
	std::string value;
	IcingaObject *object;
	STATE state;
	while( current_token.getType() != Token::END_OF_FILE ) {
		if( !isTokenTypeExpected( current_token.getType() ) ) {
			throw std::exception( "Unexpected token type");
		}
	}
}

void Parser::onStringToken() {
	if( state == STATE::GLOBAL_SCOPE ) {
		key = token.getValue();
		state = STATE::GLOBAL_SCOPE_KEY;
		return;
	}

	if( state == STATE::GLOBAL_SCOPE_VALUE ) {
		value = token.getValue();
		state = STATE::GLOBAL_SCOPE;
		object->addKeyValue( key, value );
	}

	if( state == STATE::INSIDE_OBJECT ) {
		key = token.getValue();
		state = STATE::INSIDE_OBJECT_KEY;
		return;
	}

	if( state == STATE::INSIDE_OBJECT_KEY ) {
		value = token.getValue();
		state = STATE::INSIDE_OBJECT;
		object->addKeyValue( key, value );
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

	if( state == STATE::INSIDE_OBJECT_LONG_VALUE ) {
		if( token.getRow() == row ) {
			value.concat( token.getValue() + " " );
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

void Parser::onDefineToken() {
	state = STATE::DEFINE;
}

void Parser::onHost() {
	object = new Host();
	state = STATE::DEFINED_OBJECT;
}

void Parser::onObjectStart {
	state = STATE::INSIDE_OBJECT;
}

void Parser::onObjectEnd() {
	if( state == STATE::INSIDE_OBJECT_LONG_VALUE ) {
		if( value.empty() ) {
			//throw
		}

		object->addKeyValue( key ,value );
	}

	state = STATE::GLOBAL_SCOPE;
	object = nullptr;
}

void Parser::onComment() {

}

void Parser::onLongValue() {
	key = token.getValue();
	value = std::string();
	state = STATE::INSIDE_OBJECT_LONG_VALUE;
}

