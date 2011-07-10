/*
 -----------------------------------------------------------------------------
    This file is part of the Thoronador's random stuff.
    Copyright (C) 2011 thoronador

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

#include "Splitter.h"

const std::string Splitter::cSplitString = "_by_";
const std::string Splitter::cUnknownArtist = "(unknown)";

Splitter::Splitter()
{
  //empty
}

Splitter::~Splitter()
{
  //empty
}

std::vector<TwoStrings> Splitter::splitFileNames(const std::vector<FileEntry>& files)
{
  std::vector<TwoStrings> result;
  result.clear();
  TwoStrings temp;
  std::string suffix = "";
  size_t pos = 0;
  unsigned int i;
  for (i=0; i<files.size(); i=i+1)
  {
    if (!(files.at(i).IsDirectory))
    {
      pos = files.at(i).FileName.rfind(cSplitString);
      if (pos != std::string::npos)
      {
        temp.Name = files.at(i).FileName.substr(0, pos);
        temp.Artist = files.at(i).FileName.substr(pos+cSplitString.length());
        if (temp.Artist.length()>4)
        {
          //strip file suffix
          suffix = temp.Artist.substr(temp.Artist.length()-4);
          if (suffix == ".gif" || suffix == ".jpg" || suffix == ".png")
          {
            temp.Artist = temp.Artist.substr(0, temp.Artist.length()-4);
          }//if
        }//if
      }
      else
      {
        temp.Name = files.at(i).FileName;
        temp.Artist = cUnknownArtist;
      } //else
      result.push_back(temp);
    }//if
  }//for
  return result;
}

std::set<std::string> Splitter::splitAtSpace(std::string line)
{
  std::set<std::string> result;
  result.clear();
  if (line.empty())
  {
    result.insert("");
    return result;
  }
  size_t pos =line.find(" ");
  while (pos != std::string::npos)
  {
    result.insert(line.substr(0, pos));
    line = line.substr(pos+1);
    pos = line.find(" ");
  }//while
  if (line != "")
  {
    result.insert(line);
  }
  return result;
}
