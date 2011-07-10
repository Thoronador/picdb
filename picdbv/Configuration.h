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

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>

struct Configuration
{
  //path to the directory that contains the files
  std::string Directory;

  //string containing the path to the database file
  std::string DB_File;


  /* sets predefined values for all data members. The application should call
     this once before accessing the data members of this structure. */
  void initialiseValues();

  /* Tries to load configuration from the file FileName. Returns true on success,
     false otherwise. If it fails, the configuration will be reset to some
     predefined values.

     parameters:
         FileName - the file that will be used to load the configuration
  */
  bool loadFromFile(const std::string& FileName);

  /* Tries to save the configuration to the file FileName. Returns true on
     success, false otherwise.

     parameters:
         FileName - name of the file that will be used to save the configuration
  */
  bool saveToFile(const std::string& FileName) const;

  /* character that indicates comment lines*/
  static const char cCommentCharacter;
};//struct

#endif //CONFIGURATION_H
