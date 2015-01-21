/*
 -----------------------------------------------------------------------------
    This file is part of picdb.
    Copyright (C) 2011, 2014, 2015  Thoronador

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

#ifndef SPLITTER_HPP
#define SPLITTER_HPP

#include <string>
#include <set>
#include <vector>
#include "../../libthoro/common/DirectoryFileList.h"

struct TwoStrings
{
  std::string Name;
  std::string Artist;

  /** \brief constructor */
  TwoStrings();
};

class Splitter
{
  public:
    /** \brief constructor */
    Splitter();

    /** \brief empty copy constructor */
    Splitter(const Splitter& op) {}

    /* destructor */
    virtual ~Splitter();

    static std::vector<TwoStrings> splitFileNames(const std::vector<FileEntry>& files);

    /** \brief splits the given string into pieces, treating space characters as the
     * separator for values, and returns the pieces as a set
     *
     * \param line  string containing the space-separated values
     */
    static std::set<std::string> splitAtSpace(std::string line);


    /** \brief splits the given string into pieces, treating space characters as the
     * separator for values, and returns the pieces as a vector
     *
     * \param line  string containing the space-separated values
     */
    static std::vector<std::string> splitAtSpaceVector(std::string line);


    /** \brief splits the given string into pieces, treating a given character
     * as the separator for values, and returns the pieces as a vector
     *
     * \param line  string containing the separated values
     * \param separator  character that serves as separator between elements
     */
    static std::vector<std::string> splitAtSeparator(std::string line, const char separator);

    static const std::string cSplitString;
    static const std::string cUnknownArtist;
}; //class

#endif // SPLITTER_HPP
