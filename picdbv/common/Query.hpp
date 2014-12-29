/*
 -----------------------------------------------------------------------------
    This file is part of the Thoronador's random stuff.
    Copyright (C) 2011, 2012  Thoronador

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

#ifndef QUERY_HPP
#define QUERY_HPP

#include <string>
#include <vector>
#include "PicData.hpp"

//structure to hold criteria for a DB query
struct Query
{
  std::vector<std::string> plusWho;
  std::vector<std::string> plusTags;
  std::vector<std::string> nameSubstrings;
  std::string isArtist;

  /*resets/clears all data members of the query structure

    remarks:
        Be aware that a cleared query's criteria (i.e. none at all) will be
        fulfilled by any possible data set. (See fulfilledBy().)
  */
  void clear();

  /* sets query criteria as specified by the given string

     parameters:
         queryString - the string holding the criteria
  */
  void fromString(std::string queryString);

  /* returns true, if the provided data set fulfills all criteria specified
     for this query

     parameters:
         pic - the data set to be checked
  */
  bool fulfilledBy(const PicData& pic) const;
}; //struct

#endif // QUERY_HPP
