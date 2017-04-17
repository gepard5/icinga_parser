/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  17.04.2017 16:07:40
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
#include <string>
#include <vector>
#include <set>

class Token {
	public:
		enum TYPE {
			ICINGA_OBJECT,
			DEFINE,
			OBJECT_START,
			OBJECT_END,
			KEY_VALUE_SEPARATOR,
			VALUE_SEPARATOR,
			STRING,
			COMMENT
		};


		Token(const std::string& v, const std::string& f, TYPE t, int r ) :
		 type(t), value(v), file(f), row(r)	{}

		TYPE getType() const
		{ return type; }

		std::string getValue() const
		{ return value; }

		std::string getFile() const
		{ return file; }

		int getRow() const
		{ return row; }
	private:
		TYPE type;
		std::string value;
		std::string file;
		int row;
};


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
		std::set<char> comments;
		std::set<char> whitespace;
		std::set<char> string_end;
};

std::vector<Token> Lexer::parse(str_itr begin, str_itr end, std::string curr_file) const {
	std::vector<Token> tokens;
	tokens.clear();
	int current_row = 1;
	auto curr = begin;
	while( begin != end )
	{
		std::string curr_token;
		if( !isSingleCharToken(*curr) ) {

			while( begin != end && isWhitespace( *begin ) )
			{
				++begin;
			}

			while( curr != end && !isStringEnd(*curr) )
			{
				++curr;
			}

			curr_token = std::string(begin, curr);
			begin = curr;
		}
		else {
			curr_token = std::string(1, *curr );
			++begin;
		}
		std::cout<<"Current token: "<<curr_token<<std::endl;
		auto type = getTokenType(curr_token);

		if( isComment(curr_token) ) {
			begin = eraseComment(begin, end);
			curr = begin;
			++current_row;
			continue;
		}

		while( curr != end && isWhitespace( *curr ) )
		{
			if( *curr == '\n' ) ++current_row;
			++curr;
		}

		curr = curr == end ? curr : std::next(curr);

		tokens.push_back( Token(curr_token, curr_file, type, current_row) );
	}

	return tokens;
}

bool Lexer::isSingleCharToken(const char c) const
{
	std::string token(1,c);
	return isIcingaObject( token )
		|| isDefine( token )
		|| isObjectStart( token )
		|| isObjectEnd( token )
		|| isValueSeparator( token );
}

Token::TYPE Lexer::getTokenType( const std::string& token ) const
{
	if( isComment(token) )			return Token::COMMENT;
	if( isIcingaObject(token) )		return Token::ICINGA_OBJECT;
	if( isDefine(token) ) 			return Token::DEFINE;
	if( isObjectStart(token) )		return Token::OBJECT_START;
	if( isObjectEnd(token) )		return Token::OBJECT_END;
	if(isValueSeparator(token)) 	return Token::VALUE_SEPARATOR;
	if(isKeyValueSeparator(token)) 	return Token::KEY_VALUE_SEPARATOR;
	return Token::STRING;
}

std::string::iterator Lexer::eraseComment(str_itr begin, str_itr end) const
{
	while( *begin != '\n' )
	{
		++begin;
	}
	return begin;
}

int main()
{
	Lexer lexer;
	lexer.addDefines("define");
	lexer.addIcingaObject("host");
	lexer.addIcingaObject("hostgroup");
	lexer.addIcingaObject("service");
	lexer.addIcingaObject("servicegroup");
	lexer.addIcingaObject("contact");
	lexer.addIcingaObject("command");
	lexer.addIcingaObject("commandgroup");
	lexer.addObjectStart("{");
	lexer.addObjectEnd("}");
	lexer.addWhitespace(' ');
	lexer.addWhitespace('\n');
	lexer.addWhitespace('\t');
	lexer.addValueSeparators(",");
	lexer.addKeyValueSeparators("=");
	lexer.addStringEnd(' ');
	lexer.addStringEnd('{');
	lexer.addStringEnd('}');

	std::string test = "{define     host{     par1     par2}";

	auto result = lexer.parse(test.begin(), test.end(), "file" );

	std::cout<<"Result is: "<<result.size()<<std::endl;
}
