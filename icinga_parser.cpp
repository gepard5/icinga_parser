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

	parser.printInfo();

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
	std::cout<<"File: "<<file<<std::endl;
	parseFile( file );
}

void IcingaParser::parseFile( const std::string& file )
{
	FileSource source;
	source.openFile( file );
	parser.parseSource( source, lexer );
}
