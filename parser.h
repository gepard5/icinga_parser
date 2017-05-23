/*
 * =====================================================================================
 *
 *       Filename:  parser.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  26.04.2017 21:22:48
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>

#include <map>
#include <list>
#include <functional>
#include "lexer.h"
#include "parser_exceptions.h"
#include "icinga_object.h"
#include "token.h"

struct IcingaObjectLists {
	std::list<IcingaObject*> *hosts;
	std::list<IcingaObject*> *hostgroups;
	std::list<IcingaObject*> *services;
	std::list<IcingaObject*> *servicegroups;
	std::list<IcingaObject*> *contacts;
	std::list<IcingaObject*> *timeperiods;
	std::list<IcingaObject*> *commands;
	std::list<IcingaObject*> *commandgroups;
};

class Parser {
	public:
		Parser(IcingaObjectLists l, GlobalProperties& gp ) :
			global_object(gp), hosts(*l.hosts), hostgroups(*l.hostgroups),
			services(*l.services), servicegroups(*l.servicegroups), contacts(*l.contacts),
			timeperiods(*l.timeperiods), commands(*l.commands),
			commandgroups(*l.commandgroups) { initParser(); }
		void parseSource( Source& source, Lexer& lexer );
		void test();
		void printInfo() const;
		void printStatus() const;

	private:
		enum STATE {
			GLOBAL_SCOPE,
			GLOBAL_SCOPE_KEY,
			GLOBAL_SCOPE_VALUE,
			DEFINE,
			DEFINED_OBJECT,
			INSIDE_OBJECT,
			INSIDE_OBJECT_KEY,
			INSIDE_OBJECT_VALUE,
			INSIDE_OBJECT_LONG_VALUE,
			INSIDE_OBJECT_USE,
			INSIDE_OBJECT_NEXT_USE,
			INSIDE_OBJECT_MEMBER,
			INSIDE_OBJECT_NEXT_MEMBER
		};

		void initParser();

		void onString();
		void onDefine();
		void onHost();
		void onHostgroup();
		void onService();
		void onServicegroup();
		void onCommand();
		void onContact();
		void onCommandgroup();
		void onTimeperiod();
		void onObjectStart();
		void onObjectEnd();
		void onComment();
		void onLongValue();
		void onMembers();
		void onUse();
		void onValueSeparator();
		void onKeyValueSeparator();

		void addLongValue();
		bool isTokenTypeExpected(const Token::TYPE& type);

		STATE state;
		int row;
		int object_id;
		std::string key, value;
		std::string file;
		IcingaObject *object;
		GlobalProperties global_object;
		Token token;
		std::list<IcingaObject*>& hosts;
		std::list<IcingaObject*>& hostgroups;
		std::list<IcingaObject*>& services;
		std::list<IcingaObject*>& servicegroups;
		std::list<IcingaObject*>& contacts;
		std::list<IcingaObject*>& timeperiods;
		std::list<IcingaObject*>& commands;
		std::list<IcingaObject*>& commandgroups;
		std::map<STATE, std::set<Token::TYPE> > expected_tokens;
		std::map<Token::TYPE, std::function<void()> > semantic_actions;
};

