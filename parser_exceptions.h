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

#include <exception>
#include <stdexcept>

class UnexpectedTokenException : public std::logic_error {
	public:
	UnexpectedTokenException( const std::string& s ) : std::logic_error( s ) {}

};
