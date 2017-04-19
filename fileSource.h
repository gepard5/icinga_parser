/*
 * =====================================================================================
 *
 *       Filename:  fileSource.h
 *
 *    Description:  Parsing icinga configuration and showing connections between its objects
 *
 *        Version:  1.0
 *        Created:  19.04.2017 20:56:03
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Michał Glinka
 *   Organization:  Politechnika Warszawska
 *
 * =====================================================================================
 */

#ifndef FILESOURCE_H
#define FILESOURCE_H

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

#include "source.h"


class FileSource : public Source {
	public:
		FileSource() {}
		~FileSource() {}
		void openFile( const std::string& );

		std::string getFile() const
		{ return file; }

		char getChar();
		char peekChar();
		int getRow() const
		{ return row; }

		int getColumn() const
		{ return column; }
	private:
		std::string file;
		std::ifstream instream;
		int row, column;
};

#endif
