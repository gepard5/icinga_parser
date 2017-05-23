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
#include <list>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "drawingobject.h"

class IcingaObject : public DrawingObject  {
	public:
		virtual ~IcingaObject() {}
		virtual void addKeyValue( const std::string& key, const std::string& value);
		virtual void addUse( const std::string& value );
		virtual void addMember( const std::string& member );
		virtual void printInfo() const;

		virtual void setID(int i)
		{ id = i; }

		virtual int getID() const
		{ return id; }

		virtual std::string getName();

		virtual bool isVisible() const
		{ return visible; }

		virtual void setVisible( bool v )
		{ visible = v; }

		virtual std::set< std::string >& getUses()
		{ return use; }

		virtual std::set< std::string >& getMembers()
		{ return members; }

		void addObject( IcingaObject* s );

		std::list<IcingaObject>& getObjects()
		{ return objects; }

	protected:
		bool visible;
		int id;
		std::map< std::string, std::string> properties;
		std::set< std::string > use;
		std::set< std::string > members;
		std::list<IcingaObject> objects;
};

class Service : public IcingaObject {
	public:
		std::string getHostName();
};

class Host : public IcingaObject {
	public:
		std::string getName();
};

class Hostgroup : public Host {
	public:
		std::string getName();
};

class Servicegroup : public Service {
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
