/*
 * =====================================================================================
 *
 *       Filename:  lexer.h
 *
 *    Description:  Parsing icinga configuration and showing connections between its objects
 *
 *        Version:  1.0
 *        Created:  17.04.2017 22:51:
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Michał Glinka
 *   Organization:  Politechnika Warszawska
 *
 * =====================================================================================
 */
#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>
#include <string>
#include <vector>
#include <set>

#include "token.h"
#include "source.h"


class Lexer {
	public:
		Lexer();

		Token getNextToken(Source& source) const;
		Token::TYPE getTokenType( const std::string& ) const;

	private:
		typedef std::set<std::string> StringSet;

		bool isDefine(const std::string& c) const
		{ return defines.count(c) == 1; }

		bool isComment(const std::string& c) const
		{ return comments.count( c.front() ) == 1; }

		bool isHost(const std::string& c) const
		{ return host.count(c) == 1; }

		bool isHostgroup(const std::string& c) const
		{ return hostgroup.count(c) == 1; }

		bool isService(const std::string& c) const
		{ return service.count(c) == 1; }

		bool isServicegroup(const std::string& c) const
		{ return servicegroup.count(c) == 1; }

		bool isCommand(const std::string& c) const
		{ return command.count(c) == 1; }

		bool isCommandgroup(const std::string& c) const
		{ return commandgroup.count(c) == 1; }

		bool isContact(const std::string& c) const
		{ return contact.count(c) == 1; }

		bool isTimeperiod(const std::string& c) const
		{ return timeperiod.count(c) == 1; }

		bool isValueSeparator(const std::string& c) const
		{ return value_separators.count(c) == 1; }

		bool isKeyValueSeparator(const std::string& c) const
		{ return key_value_separators.count(c) == 1; }

		bool isObjectStart(const std::string& c) const
		{ return object_start.count(c) == 1; }

		bool isObjectEnd(const std::string& c) const
		{ return object_end.count(c) == 1; }

		bool isObjectMember(const std::string& c) const
		{ return object_members.count(c) == 1; }

		bool isObjectUse(const std::string& c) const
		{ return object_uses.count(c) == 1; }

		bool isLongValue(const std::string& c) const
		{ return long_value.count(c) == 1; }

		bool isEOF(const std::string& s) const
		{ return s.front() == EOF; }

		bool isStringEnd(const char c) const
		{ return string_end.count(c) == 1; }

		bool isWhitespace(const char c) const
		{ return whitespace.count(c) == 1; }

		bool isSingleCharToken(const std::string& s) const;

		StringSet icinga_objects;
		StringSet host;
		StringSet hostgroup;
		StringSet service;
		StringSet servicegroup;
		StringSet contact;
		StringSet command;
		StringSet commandgroup;
		StringSet timeperiod;
		StringSet defines;
		StringSet value_separators;
		StringSet key_value_separators;
		StringSet object_start;
		StringSet object_end;
		StringSet object_members;
		StringSet object_uses;
		StringSet long_value;
		std::set<char> comments;
		std::set<char> whitespace;
		std::set<char> string_end;
};

#endif
