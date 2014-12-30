/*
 -----------------------------------------------------------------------------
    This file is part of the picdb test suite.
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

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cctype> //for std::isprint()
#include "../../common/escaping.hpp"

/* replaces non-printable characters in the given string by [int-value] */
std::string make_printable(std::string str)
{
  std::string::size_type i = 0;
  while (i<str.size())
  {
    if (!std::isprint(str[i]))
    {
      std::stringstream strstr;
      strstr << "[" << (int) str[i] << "]";
      str.replace(i, 1, strstr.str());
    }
    ++i;
  } //while
  return str;
}

/** \brief test case for the (un-)escape functionality
 *
 * \return Returns zero, if the test succeeded. Returns non-zero, if the test failed.
 */
int main()
{
  std::string str1;
  std::vector<std::string> test_strings;
  test_strings.push_back("");
  test_strings.push_back("abcdefghijklmnopqrstuvwxyz0123456789");
  test_strings.push_back("this is a test");
  // one NUL character
  str1 = "a";
  str1[0] = '\0';
  test_strings.push_back(str1);
  //one backslash
  test_strings.push_back("\\");
  //five backslashes
  test_strings.push_back("\\\\\\\\\\\\");
  //five NUL characters
  str1 = "12345";
  str1[0] = '\0'; str1[1] = '\0'; str1[2] = '\0'; str1[3] = '\0'; str1[4] = '\0';
  test_strings.push_back(str1);
  //five NUL characters and four backshashes between them, i.e. nine characters, altering between NUL and backslash
  str1 = "1\\2\\3\\4\\5";
  str1[0] = '\0'; str1[2] = '\0'; str1[4] = '\0'; str1[6] = '\0'; str1[8] = '\0';
  test_strings.push_back(str1);
  //C++ single character escape sequences
  test_strings.push_back("\'\"\?\a\b\f\n\r\t\v");
  //C++ single character escape sequences
  test_strings.push_back("\\\'\\\"\\\?\\a\\b\\f\\n\\r\\t\\v");

  /***************************
   * test all 256 characters *
   ***************************/

  //first 16 characters
  str1 = "\x01\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f";
  str1[0] = '\0';
  test_strings.push_back(str1);
  //next 16 characters
  test_strings.push_back("\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f");
  //next 16 characters
  test_strings.push_back("\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f");
  //next 16 characters
  test_strings.push_back("\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3a\x3b\x3c\x3d\x3e\x3f");
  //next 16 characters
  test_strings.push_back("\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f");
  //next 16 characters
  test_strings.push_back("\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5a\x5b\x5c\x5d\x5e\x5f");
  //next 16 characters
  test_strings.push_back("\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c\x6d\x6e\x6f");
  //next 16 characters
  test_strings.push_back("\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7a\x7b\x7c\x7d\x7e\x7f");
  //next 16 characters
  test_strings.push_back("\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8a\x8b\x8c\x8d\x8e\x8f");
  //next 16 characters
  test_strings.push_back("\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9a\x9b\x9c\x9d\x9e\x9f");
  //next 16 characters
  test_strings.push_back("\xa0\xa1\xa2\xa3\xa4\xa5\xa6\xa7\xa8\xa9\xaa\xab\xac\xad\xae\xaf");
  //next 16 characters
  test_strings.push_back("\xb0\xb1\xb2\xb3\xb4\xb5\xb6\xb7\xb8\xb9\xba\xbb\xbc\xbd\xbe\xbf");
  //next 16 characters
  test_strings.push_back("\xc0\xc1\xc2\xc3\xc4\xc5\xc6\xc7\xc8\xc9\xca\xcb\xcc\xcd\xce\xcf");
  //next 16 characters
  test_strings.push_back("\xd0\xd1\xd2\xd3\xd4\xd5\xd6\xd7\xd8\xd9\xda\xdb\xdc\xdd\xde\xdf");
  //next 16 characters
  test_strings.push_back("\xe0\xe1\xe2\xe3\xe4\xe5\xe6\xe7\xe8\xe9\xea\xeb\xec\xed\xee\xef");
  //last 16 characters
  test_strings.push_back("\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9\xfa\xfb\xfc\xfd\xfe\xff");

  //iterate over all strings and check, if they are (un-)escaped properly
  unsigned int i;
  for (i=0; i<test_strings.size(); ++i)
  {
    const std::string esc = escape(test_strings[i]);
    const std::string unesc = unescape(esc);
    if (i!=0)
      std::cout << std::endl;
    std::cout << "original:    " << make_printable(test_strings[i]) << std::endl
              << "escaped:     " << make_printable(esc) << std::endl
              << "unescaped:   " << make_printable(unesc) << std::endl;
    const bool match = (test_strings[i]==unesc);
    std::cout << "orig==unesc: " << match << std::endl;
    //check, if unescaped escaped string matches original
    if (!match)
      return 1+i;
  }//for
  //success
  return 0;
}
