/*
 -----------------------------------------------------------------------------
    This file is part of the Thoronador's random stuff.
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

#include "Query.hpp"
#include "Splitter.h"
#include "../libthoro/common/StringUtils.h"

void Query::clear()
{
  plusWho.clear();
  plusTags.clear();
  nameSubstrings.clear();
  isArtist = "";
}

void Query::fromString(std::string queryString)
{
  clear();
  trim(queryString);
  if (queryString=="") return;
  const std::set<std::string> sv = Splitter::splitAtSpace(queryString);
  std::set<std::string>::const_iterator set_i;
  bool lineProcessed = false;
  for (set_i=sv.begin(); set_i!=sv.end(); ++set_i)
  {
    lineProcessed = false;
    //who?
    if (set_i->length()>2)
    {
      if (set_i->substr(0,2) == "w:")
      {
        plusWho.push_back(toLowerString(set_i->substr(2)));
        lineProcessed = true;
      }
    }
    //artist?
    if (set_i->length()>3 && !lineProcessed)
    {
      if (set_i->substr(0,3) == "by:")
      {
        isArtist = set_i->substr(3);
        lineProcessed = true;
      }
    }
    //tag?
    if (set_i->length()>4 && !lineProcessed)
    {
      if (set_i->substr(0,4) == "tag:")
      {
        plusTags.push_back(toLowerString(set_i->substr(4)));
        lineProcessed = true;
      }
    }
    if (!lineProcessed)
    {
      nameSubstrings.push_back(*set_i);
    }
  }//for
}

bool Query::fulfilledBy(const PicData& pic) const
{
  //check artist
  if (isArtist!="" and isArtist!=pic.artist) return false;
  //check name substrings
  unsigned int i;
  for (i=0; i<nameSubstrings.size(); i=i+1)
  {
    if (pic.name.find(nameSubstrings[i])==std::string::npos)
    {
      return false;
    }
  }//for
  //check tags
  std::set<std::string>::const_iterator j;
  bool found;
  for (i=0; i<plusTags.size(); i=i+1)
  {
    found = false;
    for (j=pic.tags.begin(); (j!=pic.tags.end()) and !found; ++j)
    {
      found = (plusTags[i] == *j);
    }//for j
    if (!found) return false;
  }//for i

  //check who
  for (i=0; i<plusWho.size(); i=i+1)
  {
    found = false;
    for (j=pic.who.begin(); (j!=pic.who.end()) and !found; ++j)
    {
      found = (plusWho[i] == *j);
    }//for
    if (!found) return false;
  }//for i

  //finished, all conditions are met
  return true;
}
