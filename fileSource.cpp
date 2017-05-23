 /* =====================================================================================
 *
 *       Filename:  fileSource.cpp
 *
*    Description:  Parsing icinga configuration and showing connections between its objects
 *
 *        Version:  1.0
 *        Created:  19.04.2017 20:57:35
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Michał Glinka
 *   Organization:  Politechnika Warszawska
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "fileSource.h"

char FileSource::getChar() {
	char c = instream.get();
	if( c == '\n' )  {
		++row;
		column = 0;
	}
	if( c != EOF ) ++column;
	return c;
}

char FileSource::peekChar() {
	return instream.peek();
}

void FileSource::openFile( const std::string& s ) {
	file = s;
    instream = std::ifstream(file);
    if (!instream.is_open()) {
        std::cerr << "Couldn't open file: " << file << std::endl;
        exit(-1);
    }
    instream.unsetf(std::ios::skipws);      // No white space skipping!
	row = 1;
	column = 1;
}
