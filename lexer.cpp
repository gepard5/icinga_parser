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


void Lexer::initializeLexer() {
	addComment('#');
	addComment(';');
	addDefines("define");
	addIcingaObject("host");
	addIcingaObject("hostgroup");
	addIcingaObject("service");
	addIcingaObject("servicegroup");
	addIcingaObject("contact");
	addIcingaObject("command");
	addIcingaObject("commandgroup");
	addObjectStart("{");
	addObjectEnd("}");
	addWhitespace(' ');
	addWhitespace('\n');
	addWhitespace('\t');
	addValueSeparators(",");
	addKeyValueSeparators("=");
	addObjectUses("use");
	addObjectMembers("members");
	addLongValue("alias");
	addLongValue("service_description");
	addStringEnd(' ');
	addStringEnd('{');
	addStringEnd('}');
	addStringEnd('\t');
	addStringEnd('\n');
	addStringEnd('=');
	addStringEnd(',');
	addStringEnd('#');
	addStringEnd(';');
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
		source.getChar();
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
	if( isIcingaObject(token) )		return Token::ICINGA_OBJECT;
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

