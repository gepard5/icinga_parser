/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  Parsing icinga files and showing connections between them
 *
 *        Version:  1.0
 *        Created:  17.04.2017 16:07:40
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Michał Glinka
 *   Organization:  Politechnika Warszawska
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>

#include "lexer.h"
#include "token.h"


inline std::string read_from_file(char const* infile)
{
    std::ifstream instream(infile);
    if (!instream.is_open()) {
        std::cerr << "Couldn't open file: " << infile << std::endl;
        exit(-1);
    }
    instream.unsetf(std::ios::skipws);      // No white space skipping!
    return std::string(std::istreambuf_iterator<char>(instream.rdbuf()),
                       std::istreambuf_iterator<char>());
}


int main( int argc, char* argv[] )
{
	if( argc != 2 ) {
		std::cout << "Wrong usage of program \n ";
		std::cout << "./icinaParser file_to_parse\n";
		return 0;
	}

	Lexer lexer;
	lexer.addComment('#');
	lexer.addComment(';');
	lexer.addDefines("define");
	lexer.addIcingaObject("host");
	lexer.addIcingaObject("hostgroup");
	lexer.addIcingaObject("service");
	lexer.addIcingaObject("servicegroup");
	lexer.addIcingaObject("contact");
	lexer.addIcingaObject("command");
	lexer.addIcingaObject("commandgroup");
	lexer.addObjectStart("{");
	lexer.addObjectEnd("}");
	lexer.addWhitespace(' ');
	lexer.addWhitespace('\n');
	lexer.addWhitespace('\t');
	lexer.addValueSeparators(",");
	lexer.addKeyValueSeparators("=");
	lexer.addObjectUses("use");
	lexer.addObjectMembers("members");
	lexer.addLongValue("alias");
	lexer.addLongValue("service_description");
	lexer.addStringEnd(' ');
	lexer.addStringEnd('{');
	lexer.addStringEnd('}');
	lexer.addStringEnd('\t');
	lexer.addStringEnd('\n');
	lexer.addStringEnd('=');
	lexer.addStringEnd(',');
	lexer.addStringEnd('#');
	lexer.addStringEnd(';');

	auto test = read_from_file( argv[1] );

	auto result = lexer.parse(test.begin(), test.end(), argv[1] );

	std::cout<<"Result is: "<<result.size()<<std::endl;
	for( auto& a : result )
		a.printInfo();
}
