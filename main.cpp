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

#include "icinga_parser.h"

int main( int argc, char* argv[] )
{
	if( argc != 2 ) {
		std::cout << "Wrong usage of program \n ";
		std::cout << "./icinaParser file_to_parse\n";
		return 0;
	}

	IcingaParser ip;
	ip.parseIcinga("/home/gepard/test_directory");
}
