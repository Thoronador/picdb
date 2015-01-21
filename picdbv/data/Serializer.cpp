/*
 -----------------------------------------------------------------------------
    This file is part of picdb.
    Copyright (C) 2015  Thoronador

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

#include "Serializer.hpp"
#include "../common/Splitter.hpp"

std::string Serializer::serialize(const PicData& data)
{
  std::string output;
  output = data.name + "\n";
  //artist's/photographer's name
  output += data.artist + "\n";
  //people on pic
  if (data.who.empty())
  {
    output += PicData::cEmptyVector + "\n";
  }
  else
  {
    std::set<std::string>::const_iterator set_i;
    for (set_i=data.who.begin(); set_i!=data.who.end(); ++set_i)
    {
      output += *set_i + " ";
    }//for
    output += "\n";
  }//else branch
  //tags
  if (data.tags.empty())
  {
    output += PicData::cNoTags + "\n";
  }
  else
  {
    std::set<std::string>::const_iterator set_i;
    for (set_i=data.tags.begin(); set_i!=data.tags.end(); ++set_i)
    {
      output += *set_i + " ";
    }//for
    output + "\n";
  }//else branch
  //SHA-256 hash
  output += data.hash_sha256.toHexString();
  return output;
}

bool Serializer::unserialize(const std::string& serializedData, PicData& data)
{
  const std::vector<std::string> lines = Splitter::splitAtSeparator(serializedData, '\n');
  if (lines.size() != 5)
    return false;

  for (int i = 0; i < 5; ++i)
  {
    if (lines[i].empty())
      return false;
  } //for

  data.clear();
  //name
  data.name = lines[0];
  //artist's/photographer's name
  data.artist = lines[1];
  //people on pic
  data.who.clear();
  if (lines[2] != PicData::cEmptyVector)
  {
    data.who = Splitter::splitAtSpace(lines[2]);
  } //if who is not empty vector

  //tags
  if (lines[3] != PicData::cNoTags)
  {
    data.tags = Splitter::splitAtSpace(lines[3]);
  }
  //SHA-256 hash
  if (!data.hash_sha256.fromHexString(lines[4]))
    return false;

  return true;
}
