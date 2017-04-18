/*
 * =====================================================================================
 *
 *       Filename:  token.cpp
 *
 *    Description:  Parsing icinga configuration and showing connections between its objects
 *
 *        Version:  1.0
 *        Created:  17.04.2017 22:51:10
 *       Revision: 
 *       Compiler:  gcc
 *
 *         Author:  Michał Glinka
 *   Organization:  Politechnika Warszawska
 *
 * =====================================================================================
 */
#include "token.h"
#include <stdlib.h>

std::map<Token::TYPE, std::string> Token::token_types = {
	{ ICINGA_OBJECT, "icinga_object" },
	{ DEFINE, "define" },
	{ OBJECT_START, "object_start" },
	{ OBJECT_END, "object_end" },
	{ MEMBERS, "members" },
	{ USE, "use" },
	{ KEY_VALUE_SEPARATOR, "key_value_separator" },
	{ VALUE_SEPARATOR, "value_separator" },
	{ LONG_VALUE, "long_value" },
	{ STRING, "string" },
	{ COMMENT, "comment" }
};

void Token::printInfo() const
{
	std::cout<<"Value: " << value << std::endl;
	std::cout<<"Token type: " << token_types[type] << std::endl;
	std::cout<<"File: " << file << std::endl;
	std::cout<<"Row: " << row << std::endl;
}
