/*
 -----------------------------------------------------------------------------
    This file is part of picdbv.
    Copyright (C) 2011, 2012, 2014, 2015  Dirk Stolle

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

#ifndef PICDATA_HPP
#define PICDATA_HPP

#include <set>
#include <string>
#include "../../libstriezel/hash/sha256/sha256.hpp"

/** \brief Structure that holds information about a certain picture
 */
struct PicData
{
  std::string name; /**< name or title of the picture */
  std::string artist; /**< name of artist/photographer who created the picture */
  std::set<std::string> who; /**< set of persons or objects in the picture */
  std::set<std::string> tags; /**< tag set for the picture */
  SHA256::MessageDigest hash_sha256; /**< SHA-256 hash of the image file */

  /** \brief default constructor */
  PicData();

  /** \brief writes the structure's data to the standard output */
  void show() const;

  /** \brief clears all data members */
  void clear();


  /** \brief "merges" data from other PicData structure into this one, but the SHA-256 hash is not touched
   *
   * \param other data of the other structure
   */
  void mergeWith(const PicData& other);


  //constants
  static const std::string cEmptyVector;/**< constant that indicates an empty vector/set */
  static const std::string cNoTags; /**< constant that means "no tags" */


  /** \brief equality operator
   *
   * \param other  another PicData instance against which this one will be compared
   * \return Returns true, if both instances have the same content.
   */
  bool operator==(const PicData& other) const;


  /** \brief inequality operator
   *
   * \param other  another PicData instance against which this one will be compared
   * \return Returns true, if both instances have different content.
   */
  bool operator!=(const PicData& other) const;
}; //struct

#endif // PICDATA_HPP_INCLUDED
