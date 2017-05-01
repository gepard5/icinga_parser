/*
 * =====================================================================================
 *
 *       Filename:  icinga_object.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  26.04.2017 21:23:02
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef ICINGA_OBJECT
#define ICINGA_OBJECT

#include <stdlib.h>
#include <string>
#include <map>
#include <set>


class IcingaObject {
	public:
		virtual void addKeyValue( const std::string& key, const std::string& value);
		virtual void addUse( const std::string& value );
		virtual void addMember( const std::string& member );
		virtual void printInfo() const;
	protected:
		std::map< std::string, std::string> properties;
		std::set< std::string > use;
		std::set< std::string > members;
};

class Host : public IcingaObject {
};

class Hostgroup : public IcingaObject {
};

class Service : public IcingaObject {
};

class Servicegroup : public IcingaObject {
};

class Command : public IcingaObject {
};

class Commandgroup : public IcingaObject {
};

class Timeperiod : public IcingaObject {
};

class Contact : public IcingaObject {
};

class GlobalProperties : public IcingaObject {
};

#endif
