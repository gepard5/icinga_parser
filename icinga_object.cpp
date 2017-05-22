/*
 * =====================================================================================
 *
 *       Filename:  icinga_object.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  30.04.2017 14:56:31
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <iostream>
#include "icinga_object.h"


void IcingaObject::addKeyValue( const std::string& key, const std::string& value)
{
	properties[ key ] = value;
}

void IcingaObject::addUse( const std::string& value )
{
	use.insert( value );
}

void IcingaObject::addMember( const std::string& member )
{
	members.insert( member );
}

void IcingaObject::printInfo() const
{
	std::cout<<"Properties: "<<std::endl;
	for( const auto& p : properties ) 
		std::cout<<p.first<<" "<<p.second<<std::endl;
	std::cout<<"Uses: "<<std::endl;
	for( const auto& u : use )
		std::cout<<u<<std::endl;
	std::cout<<"Members: "<<std::endl;
	for( const auto& m : members )
		std::cout<<m<<std::endl;
}

std::string IcingaObject::getName()
{
	return properties["name"];
}

std::string Host::getName()
{
	return properties["host_name"];
}

std::string Hostgroup::getName()
{
	return properties["host_name"];
}

std::string Service::getName()
{
	return properties["host_name"];
}
