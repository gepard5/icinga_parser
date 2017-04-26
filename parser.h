/*
 * =====================================================================================
 *
 *       Filename:  parser.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  26.04.2017 21:22:48
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>

class Parser {
	public:
		enum STATE {
			GLOBAL_SCOPE,
			GLOBAL_SCOPE_KEY,
			GLOBAL_SCOPE_VALUE,
			DEFINE,
			DEFINED_OBJECT,
			INSIDE_OBJECT,
			INSIDE_OBJECT_KEY,
			INSIDE_OBJECT_LONG_VALUE,
			INSIDE_OBJECT_USE,
			INSIDE_OBJECT_NEXT_USE,
		}
		void parse();

	private:
		Lexer lexer;
		std::map<STATE, std::set<STATE> > expected_token;
		
};
