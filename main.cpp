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

#include <boost/program_options.hpp>

#include "icinga_parser.h"

namespace po = boost::program_options;

int main( int argc, char* argv[] )
{
	po::variables_map vm;
	po::options_description desc("Allowed options");
	try{
		desc.add_options()
			("help,h",					"produce help message")
			("visualize,v",				"show dependepcies on a screen")
			("cfg_only,c",				"parse only files with *cfg suffix")
			("location,l", po::value<std::string>(),				"folder to parse")
			("print_info,p",				"print all parsed objects")
		;

		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);
	}
	catch( po::error& e )
	{
		std::cout<<e.what()<<std::endl;
		std::cout<<desc<<std::endl;
		return 0;
	}
	std::string directory;
	IcingaParser ip;

	if( vm.count("help" )  ){
			std::cout<<desc<<std::endl;
			return 0;
	}

	if( vm.count("location") ) {
		directory = vm["location"].as<std::string>();
	}
	else {
		std::cout<<desc<<std::endl;
		return 0;
	}

	if( vm.count("cfg_only") ) {
		ip.setCfgOnly(true);
	}

	try{
	ip.parseIcinga(directory);
	}
	catch( std::logic_error& e ) {
		std::cout<<e.what()<<std::endl;
		ip.printStatus();
		return 0;
	}

	if( vm.count("print_info") ) {
		ip.printInfo();
	}

	if( vm.count("visualize") ) {
		try{
		ip.drawAll();
		}
		catch( std::logic_error& e ) {
			std::cout<<e.what()<<std::endl;
			return 0;
		}
	}
}
