/*
 -----------------------------------------------------------------------------
    This file is part of picdb.
    Copyright (C) 2011, 2012, 2013, 2014  Thoronador

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

#include "Database.hpp"
#include <iostream>
#include <algorithm>
#include "../../libthoro/common/StringUtils.h"
#include "../common/Query.hpp"
#include "../common/WhoStatRecord.hpp"

const std::string Database::cFilePrefix = "file";
const std::string Database::cNamePrefix = "name";
const std::string Database::cArtistPrefix = "artist";
const std::string Database::cWhoPrefix = "who";
const std::string Database::cTagPrefix = "tags";
const std::string Database::cHashPrefix = "sha256";


Database::~Database()
{
  //clearAllData();
}

void Database::saveFileEntry(std::ofstream& output, const std::string& fileName, const PicData& data)
{
  std::set<std::string>::const_iterator set_i;
  //filename
  output<<cFilePrefix<<":"<<fileName<<"\n";
  //pic name
  output<<cNamePrefix<<":"<<data.name<<"\n";
  //artist's/photographer's name
  output<<cArtistPrefix<<":"<<data.artist<<"\n";
  //people on pic
  output<<cWhoPrefix<<":";
  if (data.who.empty())
  {
    output << PicData::cEmptyVector<<"\n";
  }
  else
  {
    for (set_i=data.who.begin(); set_i!=data.who.end(); ++set_i)
    {
      output << *set_i+" ";
    }//for
    output << "\n";
  }//else branch
  //tags
  output<<cTagPrefix<<":";
  if (data.tags.size()==0)
  {
    output << PicData::cNoTags<<"\n";
  }
  else
  {
    for (set_i=data.tags.begin(); set_i!=data.tags.end(); ++set_i)
    {
      output << *set_i +" ";
    }//for
    output << "\n";
  }//else branch
  //SHA-256 hash
  if (!(data.hash_sha256.isNull()))
  {
    output << cHashPrefix << ":"<<data.hash_sha256.toHexString()<<"\n";
  }
}

std::vector<std::string> Database::getQueryResultUnion(const std::vector<std::string>& query_one, const std::vector<std::string>& query_two)
{
  std::vector<std::string> result;
  std::vector<std::string>::const_iterator one_iter, two_iter;
  one_iter = query_one.begin();
  two_iter = query_two.begin();
  while (one_iter!=query_one.end() and two_iter!=query_two.end())
  {
    if (*one_iter<*two_iter)
    {
      result.push_back(*one_iter);
      ++one_iter;
    }
    else if (*one_iter>*two_iter)
    {
      result.push_back(*two_iter);
      ++two_iter;
    }
    else
    {
      result.push_back(*one_iter);
      ++one_iter;
      ++two_iter;
    }
  }//while
  //check for remainder
  if (one_iter!=query_one.end())
  {
    //set end iter
    two_iter = query_one.end();
    //start iter is already known: one_iter
  }
  else if (two_iter!=query_two.end())
  {
    //set start iterator
    one_iter = two_iter;
    //set end iterator
    two_iter = query_two.end();
  }
  else
  {
    return result;
  }
  //copy remaining stuff
  while (one_iter!=two_iter)
  {
    result.push_back(*one_iter);
    ++one_iter;
  }//while
  return result;
}

std::vector<std::string> Database::getQueryResultIntersection(const std::vector<std::string>& query_one, const std::vector<std::string>& query_two)
{
  std::vector<std::string> result;
  std::vector<std::string>::const_iterator one_iter, two_iter;
  one_iter = query_one.begin();
  two_iter = query_two.begin();
  while (one_iter!=query_one.end() and two_iter!=query_two.end())
  {
    if (*one_iter<*two_iter)
    {
      ++one_iter;
    }
    else if (*one_iter>*two_iter)
    {
      ++two_iter;
    }
    else
    {
      result.push_back(*one_iter);
      ++one_iter;
      ++two_iter;
    }
  }//while
  return result;
}
