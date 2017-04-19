/*
 * =====================================================================================
 *
 *       Filename:  stringSource.h
 *
 *    Description:  Parsing icinga configuration and showing connections between its objects
 *
 *        Version:  1.0
 *        Created:  19.04.2017 21:58:59
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Michał Glinka
 *   Organization:  Politechnika Warszawska
 *
 * =====================================================================================
 */

#ifndef STRING_SOURCE_H
#define STRING_SOURCE_H

#include <stdlib.h>
#include <string>
#include "source.h"

class StringSource {
	public:
		StringSource() {}
		~StringSource() {}
		char getChar();
		char peekChar();
		int getRow() const 
		{ return row; }

		int getColumn() const 
		{ return column; }

		void setString(const std::string& s);

	private:
		int row, column;
		std::string::iterator curr;
		std::string value;
};

#endif
