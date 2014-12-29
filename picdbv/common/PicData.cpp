/*
 -----------------------------------------------------------------------------
    This file is part of picdbv.
    Copyright (C) 2011, 2012, 2013  Thoronador

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

#include "PicData.hpp"
#include <iostream>
#include "../PicDataBase.h"
#include "Splitter.hpp"

void mergeSets(std::set<std::string>& dest, const std::set<std::string>& other)
{
  std::set<std::string>::const_iterator dest_iter, other_iter;
  dest_iter = dest.begin();
  other_iter = other.begin();
  while (dest_iter!=dest.end() and other_iter!=other.end())
  {
    if (*dest_iter<*other_iter)
    {
      ++dest_iter;
    }
    else if (*dest_iter>*other_iter)
    {
      dest.insert(*other_iter);
      ++other_iter;
    }
    else
    {
      ++dest_iter;
      ++other_iter;
    }
  }//while
  //check for remainder
  while (other_iter!=other.end())
  {
    dest.insert(*other_iter);
    ++other_iter;
  }
}


const std::string PicData::cEmptyVector = "(empty)";
const std::string PicData::cNoTags = "(none)";


PicData::PicData()
: name(""),
  artist(""),
  who(std::set<std::string>()),
  tags(std::set<std::string>()),
  hash_sha256(SHA256::MessageDigest())
{
}

void PicData::show() const
{
  std::cout << "  name: "<<name<<"\n  artist: "<<artist <<"\n  who: ";
  if (who.size()==0)
  {
    std::cout <<cEmptyVector;
  }
  else
  { //list all
    std::set<std::string>::const_iterator iter;
    for (iter=who.begin(); iter!=who.end(); ++iter)
    {
      std::cout << *iter <<" ";
    }//for
  } //else-branch
  std::cout <<"\n";
  //tags
  std::cout << "  " << DataBase::cTagPrefix<<": ";
  if (tags.size()==0)
  {
    std::cout <<cNoTags;
  }
  else
  { //list all
    std::set<std::string>::const_iterator iter;
    for (iter=tags.begin(); iter!=tags.end(); ++iter)
    {
      std::cout << *iter <<" ";
    }//for
  } //else-branch
  std::cout <<"\n";
}

void PicData::clear()
{
  name.clear();
  artist.clear();
  who.clear();
  tags.clear();
  hash_sha256.setToNull();
}

void PicData::mergeWith(const PicData& other)
{
  mergeSets(who, other.who);
  mergeSets(tags, other.tags);
  //only get the other name, if this one is empty
  if (name.empty() and not other.name.empty())
  {
    name = other.name;
  }
  //only get the other artist, if this one is empty
  if ((artist.empty() or (artist==Splitter::cUnknownArtist))
    and (!other.artist.empty() and (other.artist!=Splitter::cUnknownArtist)))
  {
    artist = other.artist;
  }

  //hrmpf;
  #warning Incomplete!
}
