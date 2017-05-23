/*
 * =====================================================================================
 *
 *       Filename:  icinga_parser.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01.05.2017 11:58:26
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "icinga_parser.h"
#include "icinga_object.h"
#include "fileSource.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


bool hasEnding (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

IcingaParser::IcingaParser()
	: cfg_only( false )
{
	IcingaObjectLists l;
	l.hosts = &hosts;
	l.hostgroups = &hostgroups;
	l.services = &services;
	l.servicegroups = &servicegroups;
	l.contacts = &contacts;
	l.timeperiods = &timeperiods;
	l.commands = &commands;
	l.commandgroups = &commandgroups;
	parser = new Parser(l, global_object);
}

IcingaParser::~IcingaParser()
{
	while(!hosts.empty()) { delete hosts.front(); hosts.pop_front(); }
	while(!services.empty()) { delete services.front(); services.pop_front(); }
	while(!contacts.empty()) { delete contacts.front(); contacts.pop_front(); }
	while(!timeperiods.empty()) { delete timeperiods.front(); timeperiods.pop_front(); }
	while(!commands.empty()) { delete commands.front(); commands.pop_front(); }
	while(!commandgroups.empty()) { delete commandgroups.front(); commandgroups.pop_front(); }
	delete parser;
}

void IcingaParser::parseIcinga( const std::string& directory ) {
	path = directory;
    auto dir = opendir(path.c_str());
    path = path + "/";
    if(NULL == dir)
    {
        std::cout << "could not open directory: " << path.c_str() << std::endl;
        return;
    }
    auto entity = readdir(dir);

    while(entity != NULL)
    {
        processEntity(entity);
        entity = readdir(dir);
    }

    closedir(dir);
}

void IcingaParser::processDirectory(const std::string& directory)
{
    std::string dirToOpen = path + directory;
    auto dir = opendir(dirToOpen.c_str());

    path = dirToOpen + "/";

    if(NULL == dir)
    {
        std::cout << "could not open directory: " << dirToOpen.c_str() << std::endl;
        return;
    }

    auto entity = readdir(dir);

    while(entity != NULL)
    {
        processEntity(entity);
        entity = readdir(dir);
    }

    path.resize(path.length() - 1 - directory.length());
    closedir(dir);
}

void IcingaParser::processEntity(struct dirent* entity)
{
    if(entity->d_type == DT_DIR)
    {
        if(entity->d_name[0] == '.')
        {
            return;
        }

        processDirectory(std::string(entity->d_name));
        return;
    }

    if(entity->d_type == DT_REG)
    {
        processFile(path + std::string(entity->d_name));
        return;
    }
}

void IcingaParser::processFile(const std::string& file)
{
	parseFile( file );
}

void IcingaParser::parseFile( const std::string& file )
{
	FileSource source;
	if( cfg_only && !hasEnding( file, ".cfg" ) )
		return;
	source.openFile( file );
	parser->parseSource( source, lexer );
}

void IcingaParser::drawAll()
{
	parseDependencies();
	Visualizer v( &hosts, &services, objects_map);
	v.drawAll();
}

void IcingaParser::parseDependencies()
{
	hosts.insert( hosts.end(), hostgroups.begin(), hostgroups.end() );
	services.insert( services.end(), servicegroups.begin(), servicegroups.end() );
	createHostsMap();

	for( auto hg : hostgroups )
	{
		auto members = hg->getMembers();
		for( auto& m : members )
		{
			if( m.empty() || objects_map.count(m) == 0 )
				throw UnexpectedTemplateName( m );
			objects_map[m]->addUse( hg->getName() );
		}
	}

	for( auto sg : servicegroups )
	{
		auto members = sg->getMembers();
		for( auto& m : members )
		{
			if( m.empty() || objects_map.count(m) == 0 )
				throw UnexpectedTemplateName( m );
			objects_map[m]->addUse( sg->getName() );
		}
	}

	for( auto s : services )
	{
		std::string name = ((Service*)s)->getHostName();
		if( !name.empty() ) {
			if( objects_map.count( name ) == 0 )
			   throw UnexpectedTemplateName( name );
			objects_map[name]->addObject( s );
			s->addObject( objects_map[name] );
		}
	}
}

void IcingaParser::createHostsMap()
{
	for( auto& h : hosts )
	{
		std::string name = h->getName();
		if( !name.empty() )
			objects_map[ name ] = h;
	}

	for( auto& s : services )
	{
		std::string name = s->getName();
		if( !name.empty() )
			objects_map[ name ] = s;
	}

}

void IcingaParser::printInfo() const
{
	for( auto h : hosts )
	{
		h->printInfo();
	}
	for( auto s : services )
	{
		s->printInfo();
	}
}
