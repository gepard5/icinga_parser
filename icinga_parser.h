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
#include "visualizer.h"

#include <list>
#include <functional>


class IcingaParser {
public:
	IcingaParser();
	~IcingaParser();
	void parseIcinga( const std::string& p );
	void printInfo() const;
	void drawAll();
	void printStatus() const
	{ parser->printStatus(); }

	void showExpectedTokens()
	{ parser->showExpectedTokens(); }

	void setCfgOnly( bool c )
	{ cfg_only = c; }

private:
	void parseDirectory( const std::string& directory );
	void parseFile( const std::string& file );
	void processDirectory(const std::string& directory);
	void processFile(const std::string& file);
	void processEntity(struct dirent* entity);
	void createHostsMap();

	void parseDependencies();

	GlobalProperties global_object;
	std::string path;
	std::map<std::string, IcingaObject*> objects_map;
	std::list<IcingaObject*> hosts;
	std::list<IcingaObject*> hostgroups;
	std::list<IcingaObject*> services;
	std::list<IcingaObject*> servicegroups;
	std::list<IcingaObject*> contacts;
	std::list<IcingaObject*> timeperiods;
	std::list<IcingaObject*> commands;
	std::list<IcingaObject*> commandgroups;
	Lexer lexer;
	Parser *parser;
	bool cfg_only;
};


#endif
