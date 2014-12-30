/*
 -----------------------------------------------------------------------------
    This file is part of picdbd.
    Copyright (C) 2014  Thoronador

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

#include "functions.hpp"
#include <unistd.h>

bool isReadable(const std::string& fileName)
{
  return (access(fileName.c_str(), R_OK)==0);
}

bool isWritable(const std::string& fileName)
{
  return (access(fileName.c_str(), W_OK)==0);
}

bool isExisting(const std::string& fileName)
{
  return (access(fileName.c_str(), F_OK)==0);
}
