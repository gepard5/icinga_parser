/*
 * =====================================================================================
 *
 *       Filename:  visualizer.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  23.05.2017 16:50:19
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
#include <algorithm>

#include "visualizer.h"

bool in_range( int a, int b, int c )
{
	return a > b && a < c;
}

Visualizer::Visualizer(IcingaList* h, IcingaList* s, IcingaMap& m)
		: hosts(h), services(s), objects_map(m)
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

		main_list = hosts;
}

void Visualizer::drawAll()
{
	sf::RenderWindow window(sf::VideoMode(800, 800), "Planar visualization");
	window_ptr = &window;

	setAllVisible();
	setAllWhite();
	setObjectsProperties();

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
					if( event.mouseButton.button == sf::Mouse::Left && std::find(main_list->begin(), main_list->end(), current_object) != main_list->end() )
						showDependencies( current_object );
					if( event.mouseButton.button == sf::Mouse::Right )
						current_object->printInfo();
				}
			}
		}

		window.clear();

		for( auto h : *main_list )
		{
			if( !h->isVisible() )
				continue;
			window.draw(h->getRect());
			window.draw(h->getText());
			auto objects = h->getObjects();
			for( auto& s : objects )
			{
				window.draw(s.getRect());
				window.draw(s.getText());
			}
		}

		for( auto& b : buttons )
		{
			window.draw( b.getRect() );
			window.draw( b.getText() );
		}

		window.display();
	}
}

void Visualizer::setAllVisible()
{
	for( auto h : *main_list )
	{
		h->setVisible( true );
	}
}

void Visualizer::setAllInvisible()
{
	for( auto h : *main_list )
	{
		h->setVisible( false );
	}
}

IcingaObject* Visualizer::getIntersectionObject( float x, float y )
{
	for( auto& h : *main_list )
	{
		if( !h->isVisible() )
			continue;
		auto top_left = h->getPosition();
		auto bottom_right = h->getPosition() + h->getSize();
		if( in_range(x, top_left.x, bottom_right.x) && in_range( y, top_left.y, bottom_right.y)  ){
			return h;
		}
		auto& objects = h->getObjects();
		for( auto& o : objects )
		{
			auto top_left = o.getPosition();
			auto bottom_right = o.getPosition() + o.getSize();
			if( in_range(x, top_left.x, bottom_right.x) && in_range( y, top_left.y, bottom_right.y)  ){
				return &o;
			}
		}
	}

	return nullptr;
}

Button *Visualizer::getButtonClicked( float x, float y )
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

void Visualizer::showDependencies( IcingaObject* object )
{
	setAllInvisible();
	setAllWhite();
	object->setFillColor( sf::Color::Green );
	auto object_uses = object->getUses();
	for( auto& o : object_uses )
	{
		if( objects_map.count( o ) == 0 )
			throw UnexpectedTemplateName( o );
		objects_map[o]->setVisible(true);
		objects_map[o]->setFillColor( sf::Color::Red );
	}

	bool color_changed = true;
	while( color_changed )
	{
		color_changed = false;
		for( auto& h : *main_list )
		{
			color_changed |= setObjectDependency(h);
		}
	}
	object->setVisible( true );
}

void Visualizer::setAllWhite()
{
	for( auto h : *hosts )
		h->setFillColor( sf::Color::White );
	for( auto s : *services )
		s->setFillColor( sf::Color::White );
}

bool Visualizer::setObjectDependency( IcingaObject *object )
{
	bool color_changed = false;
	if( object->getColor() == sf::Color::White ){
		auto& object_uses = object->getUses();
		for( auto& o : object_uses )
		{
			if( objects_map.count( o ) == 0 )
				throw UnexpectedTemplateName( o );
			if( objects_map[o]->getColor() == sf::Color::Green ) {
				object->setFillColor( sf::Color::Yellow );
				object->setVisible( true );
				color_changed = true;
				break;
			}
			if( objects_map[o]->getColor() == sf::Color::Yellow
					|| objects_map[o]->getColor() == sf::Color::Cyan ) {
				object->setFillColor( sf::Color::Cyan );
				object->setVisible( true );
				color_changed = true;
				break;
			}
		}
	}
	else if( object->getColor() == sf::Color::Red 
			|| object->getColor() == sf::Color::Blue ){
		auto& object_uses = object->getUses();
		for( auto& o : object_uses )
		{
			if( objects_map.count( o ) == 0 )
				throw UnexpectedTemplateName( o );
			if( objects_map[o]->getColor() != sf::Color::White )
				continue;
			objects_map[o]->setVisible(true);
			objects_map[o]->setFillColor( sf::Color::Blue );
			color_changed = true;
		}
	}
	return color_changed;
}

void Visualizer::setObjectsProperties()
{
	float x = TILE_START_X, y = TILE_START_Y;
	for( auto h : *main_list )
	{
		x = TILE_START_X;
		h->setFont(font);
		h->setFillColor(sf::Color::White);
		h->setSize( sf::Vector2f(TILE_HEIGHT,TILE_WIDTH) );
		h->setText( std::to_string(h->getID()));
		h->setCharacterSize( FONT_SIZE );
		h->setPosition(x,y);
		auto& objects = h->getObjects();
		for( auto& s : objects )
		{
			x += TILE_WIDTH + TILE_INTERLINE ;
			s.setFont(font);
			s.setPosition( x, y );
			s.setFillColor(sf::Color::White);
			s.setSize( sf::Vector2f(TILE_HEIGHT,TILE_WIDTH) );
			s.setCharacterSize( FONT_SIZE );
			s.setText( std::to_string(s.getID()));
		}
		y += TILE_HEIGHT + TILE_INTERLINE ;
	}
}


void Visualizer::resetAll()
{
	setAllVisible();
	setAllWhite();
}

void Visualizer::exitWindow()
{
	window_ptr->close();
}

void Visualizer::setHostsAsMain()
{
	main_list = hosts;
	resetAll();
	setObjectsProperties();
}

void Visualizer::setServicesAsMain()
{
	main_list = services;
	resetAll();
	setObjectsProperties();
}
