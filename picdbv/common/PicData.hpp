/*
 -----------------------------------------------------------------------------
    This file is part of the Thoronador's random stuff.
    Copyright (C) 2011, 2012 thoronador

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
#include "../../libthoro/hashfunctions/sha-256.h"

struct PicData
{
  std::string name, artist;
  std::set<std::string> who;
  std::set<std::string> tags;
  SHA256::MessageDigest hash_sha256;

  /* writes the structure's data to the standard output */
  void show() const;

  /* clears all data members */
  void clear();

  /* "merges" data from other PicData structure into this one, but the SHA-256
     hash is not touched
  */
  void mergeWith(const PicData& other);

  //constants
  static const std::string cEmptyVector;
  static const std::string cNoTags;
}; //struct

#endif // PICDATA_HPP_INCLUDED
