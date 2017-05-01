/*
 * =====================================================================================
 *
 *       Filename:  icinga_parser.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01.05.2017 01:06:26
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef ICINGA_PARSER
#define ICINGA_PARSER

#include "parser.h"
#include "dirent.h"

#include <list>

class IcingaParser {
public:
	IcingaParser() :
		parser( hosts, hostgroups, services,
				servicegroups, contacts, commands, commandgroups, timeperiods, global_object ) {}
	void parseIcinga( const std::string& p );
	void parseDirectory( const std::string& directory );
	void parseFile( const std::string& file );
	void processDirectory(const std::string& directory);
	void processFile(const std::string& file);
	void processEntity(struct dirent* entity);

private:
	GlobalProperties global_object;
	std::string path;
	std::list<Host> hosts;
	std::list<Hostgroup> hostgroups;
	std::list<Service> services;
	std::list<Servicegroup> servicegroups;
	std::list<Contact> contacts;
	std::list<Timeperiod> timeperiods;
	std::list<Command> commands;
	std::list<Commandgroup> commandgroups;
	Lexer lexer;
	Parser parser;
};

#endif
