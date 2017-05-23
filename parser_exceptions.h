/*
 * =====================================================================================
 *
 *       Filename:  parser_exceptions.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  30.04.2017 13:05:26
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef PARSER_EXCEPTIONS
#define PARSER_EXCEPTIONS

#include <exception>
#include <stdexcept>

class UnexpectedTokenException : public std::logic_error {
	public:
	UnexpectedTokenException( const std::string& s ) : 
		std::logic_error( "Unexpected Token found: " + s ) {}

};

class UnexpectedTemplateName : public std::logic_error {
	public:
		UnexpectedTemplateName( const std::string& s ) : 
			std::logic_error( "Unexpected Template Name found: " + s ) {}
};

#endif
