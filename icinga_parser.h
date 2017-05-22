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
#include "drawingobject.h"

#include <list>
#include <functional>

class Button : public DrawingObject {
	public:
		void setButtonAction(std::function<void()> f)
		{ button_action = f; }

		void makeAction()
		{ button_action(); }

	protected:
		std::function<void()> button_action;
};

class IcingaParser {
public:
	IcingaParser() :
		parser( hosts, hostgroups, services,
				servicegroups, contacts, commands, commandgroups, timeperiods, global_object ) {init();  }
	~IcingaParser();
	void init();
	void parseIcinga( const std::string& p );
	void parseDirectory( const std::string& directory );
	void parseFile( const std::string& file );
	void processDirectory(const std::string& directory);
	void processFile(const std::string& file);
	void processEntity(struct dirent* entity);
	void drawAll();
	void setAllVisible();
	void setAllInvisible();
	void setAllWhite();

private:
	void createHostsMap();
	IcingaObject* getIntersectionObject(float x, float y);
	void showDependencies( IcingaObject* );
	bool setObjectDependency( IcingaObject* );
	void setObjectsProperties();
	Button *getButtonClicked( float, float );

	void parseDependencies();

	void setHostsAsMain();
	void setServicesAsMain();
	void resetAll();
	void exitWindow();

	Button buttons[4];

	GlobalProperties global_object;
	std::string path;
	std::map<std::string, IcingaObject*> hosts_map;
	std::list<IcingaObject*> hosts;
	std::list<IcingaObject*> hostgroups;
	std::list<IcingaObject*> services;
	std::list<IcingaObject*> servicegroups;
	std::list<IcingaObject*> contacts;
	std::list<IcingaObject*> timeperiods;
	std::list<IcingaObject*> commands;
	std::list<IcingaObject*> commandgroups;
	Lexer lexer;
	Parser parser;

	std::list<IcingaObject*> *main_list;
	std::list<IcingaObject*> *secondary_list;

	const int BUTTON_LEVEL = 525;
	sf::Font font;
	sf::RenderWindow *window_ptr;

	bool cfg_only;
};


#endif
