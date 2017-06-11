/*
 -----------------------------------------------------------------------------
    This file is part of picdb.
    Copyright (C) 2014  Dirk Stolle

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

#include "escaping.hpp"

std::string escape(const std::string& original)
{
  std::string result(original);
  escapeInPlace(result);
  return result;
}

void escapeInPlace(std::string& message)
{
  std::string::size_type pos = 0;

  //replace backslash by double backslash
  pos = message.find('\\', 0);
  while (pos != std::string::npos)
  {
    message.replace(pos, 1, "\\\\");
    pos = message.find('\\', pos+2);
  }//while

  //replace NUL character by \0
  pos = message.find('\0', 0);
  while (pos != std::string::npos)
  {
    message.replace(pos, 1, "\\0");
    pos = message.find('\0', pos+2);
  }//while
}

std::string unescape(const std::string& escaped)
{
  std::string result(escaped);
  unescapeInPlace(result);
  return result;
}

void unescapeInPlace(std::string& message)
{
  std::string::size_type pos = 0;
  const std::string cNUL = std::string(1, '\0');

  //replace \0 by NUL character
  pos = message.find("\\0", 0);
  while (pos != std::string::npos)
  {
    message.replace(pos, 2, cNUL);
    pos = message.find("\\0", pos+1);
  }//while

  //replace double backslash by single backslash
  pos = message.find("\\\\", 0);
  while (pos != std::string::npos)
  {
    message.replace(pos, 2, "\\");
    pos = message.find("\\\\", pos+1);
  }//while
}
