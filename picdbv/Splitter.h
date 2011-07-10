/*
 -----------------------------------------------------------------------------
    This file is part of the Thoronador's random stuff.
    Copyright (C) 2011 thoronador

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------------
*/

#ifndef SPLITTER_H
#define SPLITTER_H

#include <string>
#include <set>
#include <vector>
#include "../common/DirectoryFileList.h"

struct TwoStrings
{
  std::string Name;
  std::string Artist;
};

class Splitter
{
  public:
    /* constructor */
    Splitter();

    /* empty copy constructor */
    Splitter(const Splitter& op) {}

    /* destructor */
    virtual ~Splitter();

    static std::vector<TwoStrings> splitFileNames(const std::vector<FileEntry>& files);

    /* splits the given string into pieces, treating space characters as the
       separator for values, and returns the pieces as a set

       parameters:
           line - string containing the space-separated values
    */
    static std::set<std::string> splitAtSpace(std::string line);

    static const std::string cSplitString;
    static const std::string cUnknownArtist;
}; //class

#endif // SPLITTER_H
