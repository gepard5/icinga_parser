/*
 * =====================================================================================
 *
 *       Filename:  visualizer.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  23.05.2017 16:50:37
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <list>
#include <functional>

#include "drawingobject.h"
#include "icinga_object.h"
#include "parser_exceptions.h"

class Button : public DrawingObject {
	public:
		void setButtonAction(std::function<void()> f)
		{ button_action = f; }

		void makeAction()
		{ button_action(); }

	protected:
		std::function<void()> button_action;
};

class Visualizer {
	public:
	typedef std::list<IcingaObject*> IcingaList;
	typedef std::map<std::string, IcingaObject*> IcingaMap;
	Visualizer(IcingaList* h, IcingaList* s, IcingaMap& m);
	void drawAll();
	void setAllVisible();
	void setAllInvisible();
	void setAllWhite();

	private:
	Button *getButtonClicked( float, float );
	IcingaObject* getIntersectionObject(float x, float y);
	void showDependencies( IcingaObject* );
	bool setObjectDependency( IcingaObject* );
	void setObjectsProperties();
	void setHostsAsMain();
	void setServicesAsMain();
	void resetAll();
	void exitWindow();


	std::list<IcingaObject*> *main_list;
	std::list<IcingaObject*> *hosts;
	std::list<IcingaObject*> *services;
	std::map<std::string, IcingaObject*>& objects_map;

	Button buttons[4];
	sf::Font font;
	sf::RenderWindow *window_ptr;
	const int BUTTON_LEVEL = 625;
	const int WINDOW_HEIGHT = 700;
	const int TILE_HEIGHT = 30;
	const int FONT_SIZE = 15;
	const int TILE_WIDTH = 30;
	const int TILE_START_X = 10;
	const int TILE_START_Y = 5;
	const int TILE_INTERLINE = 5;
};
