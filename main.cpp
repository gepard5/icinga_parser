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
#include <fstream>
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
			MEMBERS,
			USE,
			KEY_VALUE_SEPARATOR,
			VALUE_SEPARATOR,
			LONG_VALUE,
			STRING,
			COMMENT
		};


		Token(const std::string& v, const std::string& f, TYPE t, int r ) :
		 type(t), value(v), file(f), row(r)	{}

		void printInfo() const;

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

void Token::printInfo() const
{
	std::cout<<"Value: " << value << std::endl;
	std::cout<<"Token type: " << type << std::endl;
	std::cout<<"File: " << file << std::endl;
	std::cout<<"Row: " << row << std::endl;
}


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

std::vector<Token> Lexer::parse(str_itr begin, str_itr end, std::string curr_file) const {
	std::vector<Token> tokens;
	tokens.clear();
	int current_row = 1;
	while( begin != end )
	{
		std::string curr_token;
		while( begin != end && isWhitespace( *begin ) )
		{
			if( *begin == '\n' ) ++current_row;
			++begin;
		}
		auto curr = begin;

		if( !isSingleCharToken(*curr) ) {

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

		if( isComment(curr_token) ) {
			begin = eraseComment(begin, end);
			curr = begin;
			++current_row;
			continue;
		}


		auto type = getTokenType(curr_token);
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
	if( isValueSeparator(token) ) 	return Token::VALUE_SEPARATOR;
	if( isKeyValueSeparator(token)) return Token::KEY_VALUE_SEPARATOR;
	if( isObjectMember(token) ) 	return Token::MEMBERS;
	if( isObjectUse(token) ) 		return Token::USE;
	if( isLongValue(token) )		return Token::LONG_VALUE;
	return Token::STRING;
}

std::string::iterator Lexer::eraseComment(str_itr begin, str_itr end) const
{
	while( begin != end && *begin != '\n' )
	{
		++begin;
	}
	return begin;
}

inline std::string read_from_file(char const* infile)
{
    std::ifstream instream(infile);
    if (!instream.is_open()) {
        std::cerr << "Couldn't open file: " << infile << std::endl;
        exit(-1);
    }
    instream.unsetf(std::ios::skipws);      // No white space skipping!
    return std::string(std::istreambuf_iterator<char>(instream.rdbuf()),
                       std::istreambuf_iterator<char>());
}

int main( int argc, char* argv[] )
{
	Lexer lexer;
	lexer.addComment('#');
	lexer.addComment(';');
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
	lexer.addObjectUses("use");
	lexer.addObjectMembers("members");
	lexer.addLongValue("alias");
	lexer.addLongValue("service_description");
	lexer.addStringEnd(' ');
	lexer.addStringEnd('{');
	lexer.addStringEnd('}');
	lexer.addStringEnd('\t');
	lexer.addStringEnd('\n');
	lexer.addStringEnd('=');
	lexer.addStringEnd(',');

	auto test = read_from_file( argc == 1 ? "test" :  argv[1] );

	auto result = lexer.parse(test.begin(), test.end(), "file" );

	std::cout<<"Result is: "<<result.size()<<std::endl;
	for( auto& a : result )
		a.printInfo();
}
