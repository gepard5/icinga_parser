/*
 * =====================================================================================
 *
 *       Filename:  lexer.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  17.04.2017 22:51:
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
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


class Lexer {
	public:
		typedef std::set<std::string> StringSet;
		typedef std::string::iterator str_itr;

		std::vector<Token> parse(str_itr, str_itr, std::string) const;
		Token::TYPE getTokenType( const std::string& ) const;

		void addComment(const char c)
		{ comments.insert(c); }

		void addIcingaObject(const std::string& i)
		{ icinga_objects.insert(i); }

		void addDefines(const std::string& d)
		{ defines.insert(d); }

		void addValueSeparators(const std::string& v)
		{ value_separators.insert(v); }

		void addKeyValueSeparators(const std::string& k)
		{ key_value_separators.insert(k); }

		void addObjectStart(const std::string& o)
		{ object_start.insert(o); }

		void addObjectEnd(const std::string& o)
		{ object_end.insert(o); }

		void addObjectMembers(const std::string& o)
		{ object_members.insert(o); }

		void addObjectUses(const std::string& o)
		{ object_uses.insert(o); }

		void addLongValue(const std::string& o)
		{ long_value.insert(o); }

		void addStringEnd(const char s)
		{ string_end.insert(s); }

		void addWhitespace(const char s)
		{ whitespace.insert(s); }

		bool isComment(const std::string& c) const
		{ return comments.count( c.front() ) == 1; }

		bool isIcingaObject(const std::string& c) const
		{ return icinga_objects.count(c) == 1; }

		bool isDefine(const std::string& c) const
		{ return defines.count(c) == 1; }

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

		bool isStringEnd(const char c) const
		{ return string_end.count(c) == 1; }

		bool isWhitespace(const char c) const
		{ return whitespace.count(c) == 1; }

		bool isSingleCharToken(const char c) const;

	private:
		std::string::iterator eraseComment( str_itr, str_itr ) const;

		StringSet icinga_objects;
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
