/*
 -------------------------------------------------------------------------------
    This file is part of picdbv.
    Copyright (C) 2015  Dirk Stolle

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
 -------------------------------------------------------------------------------
*/

#include "functions.hpp"
#include "../../libstriezel/common/StringUtils.hpp"

int extractStatusCodeFromResponse(const std::string& response)
{
  const std::string::size_type len = response.size();
  //valid response should be four (and even more) characters long
  if (len<4)
    return 0;
  //fourth character is space, because status codes are three digits long
  if (response[3] != ' ')
    return 0;
  int result = 0;
  if (stringToInt(response.substr(0, 3), result))
    return result;
  //conversion failed, return zero
  return 0;
}
