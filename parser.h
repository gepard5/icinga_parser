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

class Parser {
	public:
		Parser() { initParser(); }
		Parser(std::list<Host>& h, std::list<Hostgroup>& hg, std::list<Service>& s,
				std::list<Servicegroup>& sg, std::list<Contact>& c, std::list<Command>& cmd,
				std::list<Commandgroup>& cmdg, std::list<Timeperiod>& t, GlobalProperties& gp ) :
			global_object(gp), hosts(h), hostgroups(hg), services(s), servicegroups(sg), contacts(c),
			timeperiods(t), commands(cmd),commandgroups(cmdg) { initParser(); }
		void parseSource( Source& source, Lexer& lexer );
		void test();
		void printInfo() const;

	private:
		enum STATE {
			GLOBAL_SCOPE,
			GLOBAL_SCOPE_KEY,
			GLOBAL_SCOPE_VALUE,
			DEFINE,
			DEFINED_OBJECT,
			INSIDE_OBJECT,
			INSIDE_OBJECT_KEY,
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
		std::string key, value;
		std::string file;
		IcingaObject *object;
		GlobalProperties global_object;
		Token token;
		std::list<Host> hosts;
		std::list<Hostgroup> hostgroups;
		std::list<Service> services;
		std::list<Servicegroup> servicegroups;
		std::list<Contact> contacts;
		std::list<Timeperiod> timeperiods;
		std::list<Command> commands;
		std::list<Commandgroup> commandgroups;
		std::map<STATE, std::set<Token::TYPE> > expected_tokens;
		std::map<Token::TYPE, std::function<void()> > semantic_actions;
};
