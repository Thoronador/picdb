/*
 -----------------------------------------------------------------------------
    This file is part of the picdb test suite.
    Copyright (C) 2011, 2015  Dirk Stolle

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

#include <iostream>
#include <vector>
#include "../../data/PicData.hpp"
#include "../../data/Serializer.hpp"

/* This test case tests whether a PicData's content stays the same after
   serializing with Serializer::serialize() and deserializing with
   Serializer::unserialize().
*/

int main()
{
  std::vector<PicData> tests;
  // data containing only a title
  PicData pd_one;
  pd_one.name = "This is some kind of title";
  pd_one.artist = "Arthur McArty";
  tests.push_back(pd_one);
  // add a hash
  if (!pd_one.hash_sha256.fromHexString("8b508d1865b42c03defe3458bc514eb2c517e48daa36586d3b450a9f072f888f"))
  {
    std::cout << "Error: could not create SHA-256 hash from string.\n";
    return 1;
  }
  // add some tags
  pd_one.tags.insert("this");
  pd_one.tags.insert("is");
  pd_one.tags.insert("a");
  pd_one.tags.insert("list");
  pd_one.tags.insert("of");
  pd_one.tags.insert("tags");
  tests.push_back(pd_one);
  // ...and add some whos
  pd_one.who.insert("the");
  pd_one.who.insert("who");
  pd_one.who.insert("foos");
  pd_one.who.insert("the");
  pd_one.who.insert("bar");
  tests.push_back(pd_one);
  // ...and delete the tags
  pd_one.tags.clear();
  tests.push_back(pd_one);

  std::vector<PicData>::const_iterator iter = tests.begin();
  while (iter != tests.end())
  {
    const std::string serialized = Serializer::serialize(*iter);
    PicData deserialized;
    if (!Serializer::unserialize(serialized, deserialized))
    {
      std::cout << "Unable to unserialize data!\n"
                << "Data is \""<<serialized<<"\"\n.";
      return 1;
    }

    if (*iter != deserialized)
      return 1;

    ++iter;
  } //while

  return 0;
}
