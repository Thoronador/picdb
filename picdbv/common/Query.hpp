/*
 -----------------------------------------------------------------------------
    This file is part of picdbv.
    Copyright (C) 2011, 2012, 2014  Thoronador

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
#include "../data/PicData.hpp"

/** \brief structure to hold criteria for a DB query
 * \remarks The different criteria are AND-ed together, i.e. a picture has to
 *          fulfill every criterion to match the query.
 */
struct Query
{
  std::vector<std::string> plusWho; /**< list of people or objects on the picture */
  std::vector<std::string> plusTags; /**< list of tags that a picture should have to meet the query criteria */
  std::vector<std::string> nameSubstrings; /**< list of substrings that have to occur in a picture's name */
  std::string isArtist; /**< name of the artist (ignored, if empty) */


  /** \brief default constructor */
  Query();

  /** \brief resets/clears all data members of the query structure
   *
   * \remarks
   *    Be aware that a cleared query's criteria (i.e. none at all) will be
   *    fulfilled by any possible data set. (See fulfilledBy().)
   */
  void clear();


  /** \brief sets query criteria as specified by the given string
   *
   * \param queryString the string holding the criteria
   */
  void fromString(std::string queryString);


  /** \brief returns true, if the provided data set fulfills all criteria specified for this query
   *
   * \param pic the data set to be checked
   */
  bool fulfilledBy(const PicData& pic) const;
}; //struct

#endif // QUERY_HPP
