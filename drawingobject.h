/*
 * =====================================================================================
 *
 *       Filename:  drawingobject.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  21.05.2017 10:51:40
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef DRAWING_OBJECT
#define DRAWING_OBJECT

#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class DrawingObject {
	public:
		void setPosition(float x, float y)
		{ rect.setPosition(x, y); text.setPosition(x,y);  }

		void setText( const std::string& s )
		{ text.setString(s); }

		void setFont( const sf::Font& f )
		{ text.setFont(f); }

		void setSize( const sf::Vector2f s )
		{ rect.setSize( s ); }

		void setFillColor( sf::Color c )
		{ rect.setFillColor(c); text.setColor(sf::Color::Black); }

		void setCharacterSize( int size )
		{ text.setCharacterSize( size ); }

		sf::Color getColor() const
		{ return rect.getFillColor(); }

		sf::Text& getText()
		{ return text; }

		sf::RectangleShape getRect() const
		{ return rect; }

		sf::Vector2f getPosition() const
		{ return rect.getPosition(); }

		sf::Vector2f getSize() const
		{ return rect.getSize(); }

	protected:
		sf::Text text;
		sf::RectangleShape rect;
};

#endif
