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


std::vector<Token> Lexer::parse(str_itr begin, str_itr end, std::string curr_file) const {
	std::vector<Token> tokens;
	tokens.clear();
	int current_row = 1;
	while( begin != end )
	{
		std::string curr_token;
		while( begin != end && isWhitespace( *begin ) )
		{
			if( *begin == '\n' ) ++current_row;
			++begin;
		}
		auto curr = begin;

		if( !isSingleCharToken(*curr) ) {

			while( curr != end && !isStringEnd(*curr) )
			{
				++curr;
			}

			curr_token = std::string(begin, curr);
			begin = curr;
		}
		else {
			curr_token = std::string(1, *curr );
			++begin;
		}

		if( isComment(curr_token) ) {
			begin = eraseComment(begin, end);
			curr = begin;
			++current_row;
			continue;
		}


		auto type = getTokenType(curr_token);
		tokens.push_back( Token(curr_token, curr_file, type, current_row) );

	}
	return tokens;
}

bool Lexer::isSingleCharToken(const char c) const
{
	std::string token(1,c);
	return getTokenType( token ) != Token::STRING;
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
	return Token::STRING;
}

std::string::iterator Lexer::eraseComment(str_itr begin, str_itr end) const
{
	while( begin != end && *begin != '\n' )
	{
		++begin;
	}
	return begin;
}

