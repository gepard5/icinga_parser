/*
 * =====================================================================================
 *
 *       Filename:  token.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  17.04.2017 22:50:44
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>

class Token {
	public:
		enum TYPE {
			ICINGA_OBJECT,
			DEFINE,
			OBJECT_START,
			OBJECT_END,
			MEMBERS,
			USE,
			KEY_VALUE_SEPARATOR,
			VALUE_SEPARATOR,
			LONG_VALUE,
			STRING,
			COMMENT
		};


		Token(const std::string& v, const std::string& f, TYPE t, int r ) :
		 type(t), value(v), file(f), row(r)	{}

		void printInfo() const;

		TYPE getType() const
		{ return type; }

		std::string getValue() const
		{ return value; }

		std::string getFile() const
		{ return file; }

		int getRow() const
		{ return row; }
	private:
		TYPE type;
		std::string value;
		std::string file;
		int row;
};

#endif

