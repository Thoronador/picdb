/*
 -----------------------------------------------------------------------------
    This file is part of picdbd.
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
 -----------------------------------------------------------------------------
*/

#include "CmdFilesFromDirectory.hpp"
#include "../constants.hpp"
#include "../../data/DatabaseManager.hpp"

CommandFilesFromDirectory::CommandFilesFromDirectory()
: Command("files_from_directory")
{ }

bool CommandFilesFromDirectory::processMessage(const std::string& message, std::string& answer) const
{
  /* put all files from a directory into the database */
  if (message.size() > 21 && (message.substr(0, 21) == "files_from_directory "))
  {
    std::string db_name = "";
    std::string::const_iterator iter = message.begin()+21;
    while ((iter != message.end()) && (*iter != ' '))
    {
      db_name.push_back(*iter);
      ++iter;
    } //while
    if (iter==message.end())
    {
      //Valid string shall contain directory after db_name.
      answer = codeBadRequest + " files_from_directory needs exactly two arguments: DB name without spaces and directory path";
      return true;
    }
    //move to character after space
    ++iter;
    if (iter==message.end())
    {
      //Valid string shall contain directory after db_name and space.
      answer = codeBadRequest + " files_from_directory needs exactly two arguments: DB name without spaces and directory path";
      return true;
    }
    if (!DatabaseManager::get().hasDatabase(db_name))
    {
      answer = codeBadRequest + " unknown database "+db_name;
      return true;
    }

    const bool success = DatabaseManager::get().getDatabase(db_name).getFilesFromDirectory(std::string(iter, message.end()));
    if (success)
    {
      answer = codeOK + " database " + db_name + " has been updated";
    }
    else
    {
      answer = codeNoContent + " no files have been added to database "+db_name;
    } //else
    return true;
  } //if files_from_directory
  else
    return false;
}

std::string CommandFilesFromDirectory::helpText() const
{
  return "adds files from a directory to the database";
}
