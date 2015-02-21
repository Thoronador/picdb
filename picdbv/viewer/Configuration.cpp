/*
 -----------------------------------------------------------------------------
    This file is part of picdbv.
    Copyright (C) 2011, 2014  Thoronador

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

#include "Configuration.hpp"
#include <fstream>
#include <iostream>
#include "../../libthoro/common/DirectoryFileList.h"
#include "../../libthoro/common/StringUtils.h"

const char Configuration::cCommentCharacter = '#';

Configuration::Configuration()
: Directory("."+DirectorySeparator),
  DB_File("file_db.dat")
{
}

void Configuration::initialiseValues()
{
  Directory = "."+DirectorySeparator;
  DB_File = "file_db.dat";
}

bool Configuration::loadFromFile(const std::string& FileName)
{
  std::ifstream input;
  input.open(FileName.c_str(), std::ios::in | std::ios::binary);
  if (!input)
  {
    std::cout << "Configuration::loadFromFile: ERROR: Could not open file \""
              << FileName <<"\".\nLoading from file canceled.\n Using "
              << "predefined configuration instead.\n";
    initialiseValues();
    return false;
  }
  const unsigned int cMaxLine = 256;
  char buffer[cMaxLine];
  std::string line = "";
  size_t sep_pos = 0;
  while (input.getline(buffer, cMaxLine-1))
  {
    buffer[255] = '\0';
    line = std::string(buffer);
    //check for possible carriage return at end (happens on Windows systems)
    if (!line.empty())
    {
      if (line.at(line.length()-1)=='\r')
      {
        line.erase(line.length()-1);
      }//if
    }

    if (!line.empty()) //skip empty lines
    {
      //skip comments
      if (line.at(0)!=cCommentCharacter)
      {
        //got data for new setting
        sep_pos = line.find('=');
        if (sep_pos == std::string::npos || sep_pos == 0)
        {
          std::cout << "Configuration::loadFromFile: ERROR: Invalid line found: \""
                    << line <<"\".\nLoading from file canceled. Using "
                    << "predefined configuration instead.\n";
          input.close();
          return false;
        }
        //get name and value of setting
        std::string name = line.substr(0, sep_pos);
        std::string value = line.substr(sep_pos+1);
        trim(name);
        trim(value);
        if (!name.empty() and !value.empty())
        {
          if ((name=="database") or (name=="db"))
          {
            DB_File = value;
          }
          else if ((name=="directory") or (name=="dir"))
          {
            Directory = value;
          }
          else
          {
            std::cout << "Configuration::loadFromFile: ERROR: Invalid name found: \""
                    << name <<"\".\nCurrently the only allowed names are "
                    << "\"database\" and \"directory\".\nLoading from file "
                    << "canceled. Using predefined configuration instead.\n";
            initialiseValues();
            return false;
          }
        }//if non-empty name and value

      }//if not comment
    } //if not empty
  }//while
  input.close();
  return true;
}

bool Configuration::saveToFile(const std::string& FileName) const
{
  std::ofstream output;
  output.open(FileName.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
  if (!output)
  {
    std::cout << "Configuration::saveToFile: ERROR: Could not open file \""
              << FileName <<"\".\nSaving to file canceled.\n";
    return false;
  }
  const char cc = cCommentCharacter;
  output << cc <<" Picdbv configuration file\n"
         << cc <<"\n"
         << cc <<" This file holds some configuration settings which are read\n"
         << cc <<" at the start of the programme.\n"
         << cc <<" The format for settings is \"NameOfSetting=value\".\n"
         << cc <<" A new-line character starts a new configuration setting and\n"
         << cc <<" the maximum length of a line is limited to 255 characters.\n"
         << cc <<" Lines starting with "<<cc<<" are comments.\n\n"
         << cc <<" Currently there are the following possible settings:\n\n"
         << cc <<"   database=file_db.dat\n"
         << cc <<"   -> sets the file to read/write database data\n\n"
         << cc <<"   directory=path/to/directory\n"
         << cc <<"   -> path to the directory where the files listed within\n"
         << cc <<"      the database are located. This can be an absolute or\n"
         << cc <<"      relative path.\n\n";
  //write database path
  output << "database="<<DB_File<<"\n";
  //write directory value
  output << "directory="<<Directory<<"\n";
  output.close();
  return output.good();
}
