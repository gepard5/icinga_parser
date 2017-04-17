/*
 * =====================================================================================
 *
 *       Filename:  token.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  17.04.2017 22:51:10
 *       Revision:  
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "token.h"
#include <stdlib.h>

void Token::printInfo() const
{
	std::cout<<"Value: " << value << std::endl;
	std::cout<<"Token type: " << type << std::endl;
	std::cout<<"File: " << file << std::endl;
	std::cout<<"Row: " << row << std::endl;
}
