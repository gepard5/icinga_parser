/*
 * =====================================================================================
 *
 *       Filename:  stringSource.cpp
 *
 *    Description:  Parsing icinga configuration and showing connections between its objects
 *
 *        Version:  1.0
 *        Created:  19.04.2017 22:01:50
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Michał Glinka
 *   Organization:  Politechnika Warszawska
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "stringSource.h"

char StringSource::getChar() {
	return curr == value.end() ? EOF : *(curr++) ;
}

char StringSource::peekChar() {
	return curr == value.end() ? EOF : *curr ;
}

void StringSource::setString( const std::string& s) {
	value = s;
	curr = value.begin(); 
}
