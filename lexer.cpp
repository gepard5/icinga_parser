/*
 * =====================================================================================
 *
 *       Filename:  lexer.cpp
 *
 *    Description:  Parsing icinga configuration and showing connections between its objects
 *
 *        Version:  1.0
 *        Created:  17.04.2017 22:51:59
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Michał Glinka
 *   Organization:  Politechnika Warszawska
 *
 * =====================================================================================
 */

#include "lexer.h"


Lexer::Lexer() {
	icinga_objects = { "host", "hostgroup", "service", "servicegroup", "contact",
		"command", "commandgroup" };
	host = { "host" };
	hostgroup = { "hostgroup" };
	service = { "service" };
	servicegroup = { "servicegroup" };
	contact = { "contact" };
	command = { "command" };
	commandgroup = { "commandgroup" };
	timeperiod = { "timeperiod" };
	defines = { "define" };
	value_separators = { "," };
	key_value_separators = { "=" };
	object_start = { "{" };
	object_end = { "}" };
	object_members = { "members" };
	object_uses = { "use" };
	long_value = { "alias", "service_description", "check_command" };
	comments = { '#', ';' };
	whitespace = { ' ', '\t', '\n' };
	string_end = { ' ', '{', '}', '\t', '\n', '=', ',', '#', ';' };
}

Token Lexer::getNextToken(Source& source) const {
	std::string curr_token;
	//skip white characters
	while( source.peekChar() != EOF && isWhitespace( source.peekChar() ) ) {
		source.getChar();
	}
	curr_token.push_back( source.getChar() );
	if( !isSingleCharToken( curr_token ) ) {
		while( source.peekChar() != EOF && !isStringEnd( source.peekChar() ) ) {
			curr_token.push_back( source.getChar() );
		}
	}

	auto type = getTokenType( curr_token );
	if( type == Token::COMMENT ) {
		while( source.peekChar() != EOF && source.peekChar() != '\n' ) {
			curr_token.push_back( source.getChar() );
		}
	}

	return Token( curr_token, type, source.getRow(), source.getColumn() );
}

bool Lexer::isSingleCharToken(const std::string& s) const
{
	return getTokenType( s ) != Token::STRING;
}

Token::TYPE Lexer::getTokenType( const std::string& token ) const
{
	if( isComment(token) )			return Token::COMMENT;
	if( isHost(token) )				return Token::HOST;
	if( isHostgroup(token) )		return Token::HOSTGROUP;
	if( isService(token) )			return Token::SERVICE;
	if( isServicegroup(token) )		return Token::SERVICEGROUP;
	if( isContact(token) )			return Token::CONTACT;
	if( isCommand(token) )			return Token::COMMAND;
	if( isCommandgroup(token) )		return Token::COMMANDGROUP;
	if( isTimeperiod(token) )		return Token::TIMEPERIOD;
	if( isDefine(token) ) 			return Token::DEFINE;
	if( isObjectStart(token) )		return Token::OBJECT_START;
	if( isObjectEnd(token) )		return Token::OBJECT_END;
	if( isValueSeparator(token) ) 	return Token::VALUE_SEPARATOR;
	if( isKeyValueSeparator(token)) return Token::KEY_VALUE_SEPARATOR;
	if( isObjectMember(token) ) 	return Token::MEMBERS;
	if( isObjectUse(token) ) 		return Token::USE;
	if( isLongValue(token) )		return Token::LONG_VALUE;
	if( isEOF( token ) )			return Token::END_OF_FILE;
	return Token::STRING;
}

