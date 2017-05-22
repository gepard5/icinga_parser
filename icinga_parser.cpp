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

bool in_range( int a, int b, int c )
{
	return a > b && a < c;
}

bool hasEnding (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

IcingaParser::~IcingaParser()
{
	for( auto o : hosts )
	{
		delete o;
	}
	for( auto o : hostgroups )
	{
		delete o;
	}
	for( auto o : services )
	{
		delete o;
	}
	for( auto o : servicegroups )
	{
		delete o;
	}
	for( auto o : contacts )
	{
		delete o;
	}
	for( auto o : timeperiods )
	{
		delete o;
	}
	for( auto o : commands )
	{
		delete o;
	}
	for( auto o : commandgroups )
	{
		delete o;
	}
}

void IcingaParser::init()
{
		if( !font.loadFromFile( "arial.ttf" ) ) {
			throw UnexpectedTemplateName( "s" );
		}

		for( auto& b : buttons )
		{
			b.setFont( font );
			b.setSize( sf::Vector2f( 150, 50 ) );
			b.setFillColor( sf::Color::Yellow );
		}

		main_list = &hosts;
		secondary_list = &services;

		buttons[0].setPosition( 25, BUTTON_LEVEL );
		buttons[0].setButtonAction( [&](){ this->setHostsAsMain();  });
		buttons[0].setText( "Hosts" );
		buttons[1].setPosition( 225, BUTTON_LEVEL );
		buttons[1].setButtonAction( [&](){ this->setServicesAsMain();  });
		buttons[1].setText( "Services" );
		buttons[2].setPosition( 425, BUTTON_LEVEL );
		buttons[2].setButtonAction( [&](){ this->resetAll();  });
		buttons[2].setText( "Reset" );
		buttons[3].setPosition( 625, BUTTON_LEVEL );
		buttons[3].setButtonAction( [&](){ this->exitWindow();  });
		buttons[3].setText( "Exit" );
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
	parseFile( file );
}

void IcingaParser::parseFile( const std::string& file )
{
	FileSource source;
	if( cfg_only && !hasEnding( file, ".cfg" ) )
		return;
	source.openFile( file );
	parser.parseSource( source, lexer );
}

void IcingaParser::parseDependencies()
{
	hosts.insert( hosts.end(), hostgroups.begin(), hostgroups.end() );
	services.insert( services.end(), servicegroups.begin(), servicegroups.end() );
	createHostsMap();

	for( auto& s : services )
	{
		std::string name = s->getName();
		if( !name.empty() ) {
			if( hosts_map.count( name ) == 0 )
			   throw UnexpectedTemplateName( name );
			hosts_map[name]->addObject( s );
			s->addObject( hosts_map[name] );
		}
	}
}

void IcingaParser::drawAll()
{
	sf::RenderWindow window(sf::VideoMode(800, 800), "Planar visualization");
	window_ptr = &window;

	parseDependencies();
	setAllVisible();
	setAllWhite();
	float x = 100, y = 100, radius = 30;
	float xshift = 15, yshift = 7;
	for( auto h : *main_list )
	{
		x = 100;
		h->setFont(font);
		h->setSize( sf::Vector2f(40,50) );
		h->setText(std::to_string(h->getID()));
		h->setPosition(x,y);
		if( h->isVisible() ) {
			window.draw(h->getRect());
			window.draw(h->getText());
		}
		auto objects = h->getObjects();
		for( auto s : objects )
		{
			if( !h->isVisible() )
				continue;
			x += 2*radius;
			s->setPosition( x, y );
			sf::Text text;
			text.setFont(font);
			text.setString(std::to_string(s->getID()));
			s->setSize( sf::Vector2f(40,50) );
			text.setPosition( x+xshift, y+yshift );
			window.draw(s->getRect());
			window.draw(s->getText());
		}
		y += 2*radius;
	}

	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
				window.close();

			if( event.type == sf::Event::MouseButtonPressed ) {
				auto current_button = getButtonClicked(event.mouseButton.x, event.mouseButton.y);
				if( current_button ) {
					current_button->makeAction();
				}

				auto current_object = getIntersectionObject(event.mouseButton.x, event.mouseButton.y );
				if( current_object ) {
					if( event.mouseButton.button == sf::Mouse::Left )
						showDependencies( current_object );
					if( event.mouseButton.button == sf::Mouse::Right )
						current_object->printInfo();
				}
			}
		}

		window.clear();

		float x = 100, y = 100, radius = 30;
		float xshift = 15, yshift = 7;
		for( auto h : *main_list )
		{
			x = 100;
			h->setFont(font);
			h->setSize( sf::Vector2f(40,50) );
			h->setText(std::to_string(h->getID()));
			h->setPosition(x,y);
			if( h->isVisible() ) {
				window.draw(h->getRect());
				window.draw(h->getText());
			}
			auto objects = h->getObjects();
			for( auto s : objects )
			{
				if( !h->isVisible() )
					continue;
				x += 2*radius;
				s->setPosition( x, y );
				sf::Text text;
				text.setFont(font);
				text.setString(std::to_string(s->getID()));
				s->setSize( sf::Vector2f(40,50) );
				text.setPosition( x+xshift, y+yshift );
				window.draw(s->getRect());
				window.draw(s->getText());
			}
			y += 2*radius;
		}

		for( auto& b : buttons )
		{
			window.draw( b.getRect() );
			window.draw( b.getText() );
		}

		window.display();
	}
}

void IcingaParser::setObjectsProperties()
{
	float x = 100, y = 100, radius = 30;
	float xshift = 15, yshift = 7;
	for( auto h : *main_list )
	{
		x = 100;
		h->setFont(font);
		h->setSize( sf::Vector2f(40,50) );
		h->setText(std::to_string(h->getID()));
		h->setPosition(x,y);
		auto objects = h->getObjects();
		for( auto s : objects )
		{
			x += 2*radius;
			s->setPosition( x, y );
			sf::Text text;
			text.setFont(font);
			text.setString(std::to_string(s->getID()));
			s->setSize( sf::Vector2f(40,50) );
			text.setPosition( x+xshift, y+yshift );
		}
		y += 2*radius;
	}
}

void IcingaParser::setAllVisible()
{
	for( auto h : *main_list )
	{
		h->setVisible( true );
	}
	for( auto s : *secondary_list )
	{
		s->setVisible( true );
	}
}

void IcingaParser::setAllInvisible()
{
	for( auto h : *main_list )
	{
		h->setVisible( false );
	}
}

void IcingaParser::createHostsMap()
{
	for( auto& h : hosts )
	{
		std::string name = h->getName();
		if( !name.empty() )
			hosts_map[ name ] = h;
	}
}

IcingaObject* IcingaParser::getIntersectionObject( float x, float y )
{
	for( auto& h : hosts )
	{
		if( !h->isVisible() )
			continue;
		auto top_left = h->getPosition();
		auto bottom_right = h->getPosition() + h->getSize();
		if( in_range(x, top_left.x, bottom_right.x) && in_range( y, top_left.y, bottom_right.y)  ){
			return h;
		}
	}

	for( auto& s : services )
	{
		if( !s->isVisible() )
			continue;
		auto top_left = s->getPosition();
		auto bottom_right = s->getPosition() + s->getSize();
		if( in_range(x, top_left.x, bottom_right.x) && in_range( y, top_left.y, bottom_right.y ) )
			return s;
	}

	return nullptr;
}

Button *IcingaParser::getButtonClicked( float x, float y )
{
	if( y < BUTTON_LEVEL )
		return nullptr;

	for( auto& b : buttons )
	{
		if( x > b.getPosition().x && x < (b.getPosition().x + b.getSize().x) ) {
			return &b;
		}
	}

	return nullptr;
}

void IcingaParser::showDependencies( IcingaObject* object )
{
	setAllInvisible();
	setAllWhite();
	object->setFillColor( sf::Color::Green );
	auto object_uses = object->getUses();
	for( auto& o : object_uses )
	{
		if( hosts_map.count( o ) == 0 )
			throw UnexpectedTemplateName( o );
		hosts_map[o]->setVisible(true);
		hosts_map[o]->setFillColor( sf::Color::Red );
	}

	bool color_changed = true;
	while( color_changed )
	{
		color_changed = false;
		for( auto& h : hosts )
		{
			color_changed |= setObjectDependency(h);
		}
	}
	object->setVisible( true );
}

void IcingaParser::setAllWhite()
{
	for( auto h : hosts )
		h->setFillColor( sf::Color::White );
	for( auto s : services )
		s->setFillColor( sf::Color::White );
}

bool IcingaParser::setObjectDependency( IcingaObject *object )
{
	bool color_changed = false;
	if( object->getColor() == sf::Color::White ){
		auto object_uses = object->getUses();
		for( auto& o : object_uses )
		{
			if( hosts_map.count( o ) == 0 )
				throw UnexpectedTemplateName( o );
			if( hosts_map[o]->getColor() == sf::Color::Green ) {
				object->setFillColor( sf::Color::Yellow );
				object->setVisible( true );
				color_changed = true;
				break;
			}
			if( hosts_map[o]->getColor() == sf::Color::Yellow
					|| hosts_map[o]->getColor() == sf::Color::Cyan ) {
				object->setFillColor( sf::Color::Cyan );
				object->setVisible( true );
				color_changed = true;
				break;
			}
		}
	}
	else {
		auto object_uses = object->getUses();
		for( auto& o : object_uses )
		{
			if( hosts_map.count( o ) == 0 )
				throw UnexpectedTemplateName( o );
			if( hosts_map[o]->getColor() != sf::Color::White )
				continue;
			hosts_map[o]->setVisible(true);
			hosts_map[o]->setFillColor( sf::Color::Blue );
			color_changed = true;
		}
	}
	return color_changed;
}

void IcingaParser::resetAll()
{
	setAllVisible();
	setAllWhite();
}

void IcingaParser::exitWindow()
{
	window_ptr->close();
}

void IcingaParser::setHostsAsMain()
{
	main_list = &hosts;
}

void IcingaParser::setServicesAsMain()
{
	main_list = &services;
}
