/*
 * =====================================================================================
 *
 *       Filename:  token.h
 *
 *    Description:  Parsing icinga configuration and showing connections between its objects
 *
 *        Version:  1.0
 *        Created:  17.04.2017 22:50:44
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author: Michał Glinka
 *   Organization: Politechnika Warszawska
 *
 * =====================================================================================
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>

#include <map>

class Token {
	public:
		enum TYPE {
			ICINGA_OBJECT,							//typ obiektu icinga: 
													//"host", "hostgroup", "service", 
													//"servicegroup", "command", "commandgroup", "contact"
			DEFINE,									//"define"
			OBJECT_START,							//początek definicji obiektu: "{"
			OBJECT_END,								//koniec definicji obiektu: "}"
			MEMBERS,								//nazwy członków grupy: "members"
			USE,									//nazwy używanych zależności: "use"
			KEY_VALUE_SEPARATOR,					//rodziela klucz i wartość globalnego ustawienia: "="
			VALUE_SEPARATOR,						//rodziela ciąg wartości : ","
			LONG_VALUE,								//po tym tokenie wartością są wszystkie następne stringi w linii: 
													//"alias", "service_description"
			COMMENT,								//komentarz: ( # | ; )[^\n]*
			STRING,									//ciągi znaków
			END_OF_FILE								//koniec pliku
		};


		Token(const std::string& v, TYPE t, int r, int c ) :
		 type(t), value(v), row(r), column(c)	{}

		void printInfo() const;

		TYPE getType() const
		{ return type; }

		std::string getValue() const
		{ return value; }

		std::pair<int, int> getPosition() const
		{ return std::make_pair(row, column); }
	private:
		TYPE type;
		std::string value;
		int row, column;
		static std::map<TYPE, std::string> token_types;
};

#endif

