/*
 -----------------------------------------------------------------------------
    This file is part of the picdb test suite.
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

#include <iostream>
#include <map>
#include "../../../daemon/functions.hpp"

int main()
{
  std::map<std::string, int> test_cases;
  //valid test cases
  test_cases["100 Continue"] = 100;
  test_cases["200 OK"] = 200;
  test_cases["204 No content here"] = 204;
  test_cases["400 Bad Request"] = 400;
  test_cases["413 Request Entity Too Large"] = 413;
  test_cases["500 Error"] = 500;
  test_cases["999 foobar"] = 999;

  //invalid responses -> should all return zero
  test_cases[""] = 0;
  test_cases["1"] = 0;
  test_cases["12"] = 0;
  test_cases["123"] = 0;
  test_cases["1234"] = 0;
  test_cases["12345"] = 0;
  test_cases["ABC blabla"] = 0;
  test_cases["200OK"] = 0;
  test_cases["Something"] = 0;
  test_cases["stomp"] = 0;

  std::map<std::string, int>::const_iterator iter = test_cases.begin();
  while (iter != test_cases.end())
  {
    const int code = extractStatusCodeFromResponse(iter->first);
    if (code != iter->second)
    {
      std::cout << "Error: extractStatusCodeFromResponse() returned inexpected result.\n"
                << "  Input: \"" << iter->first << "\"\n"
                << "  Returned code: " << code << "\n"
                << "  Expected code: "  << iter->second << "\n";
      return 1;
    }//if
    ++iter;
  } //while
  return 0;
}
