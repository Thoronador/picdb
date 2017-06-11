/*
 -------------------------------------------------------------------------------
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
 -------------------------------------------------------------------------------
*/

#include "CmdNonExistingFiles.hpp"
#include "../constants.hpp"
#include "../../data/DatabaseManager.hpp"

CommandNonExistingFiles::CommandNonExistingFiles()
: Command("nonexisting_files")
{ }

bool CommandNonExistingFiles::processMessage(const std::string& message, std::string& answer) const
{
  /* lists files that are in database but not on disk anymore */
  if (message.size() > 18 && (message.substr(0, 18) == "nonexisting_files "))
  {
    std::string db_name = "";
    std::string::const_iterator iter = message.begin()+18;
    while ((iter != message.end()) && (*iter != ' '))
    {
      db_name.push_back(*iter);
      ++iter;
    } //while
    if (iter==message.end())
    {
      //Valid string shall contain directory after db_name.
      answer = codeBadRequest + " nonexisting_files needs exactly two arguments: DB name without spaces and directory path";
      return true;
    }
    //move to character after space
    ++iter;
    if (iter==message.end())
    {
      //Valid string shall contain directory after db_name and space.
      answer = codeBadRequest + " nonexisting_files needs exactly two arguments: DB name without spaces and directory path";
      return true;
    }
    if (!DatabaseManager::get().hasDatabase(db_name))
    {
      answer = codeBadRequest + " unknown database "+db_name;
      return true;
    }

    const std::vector<std::string> files = DatabaseManager::get().getDatabase(db_name).getNonexistingFiles(std::string(iter, message.end()));
    if (!files.empty())
    {
      answer = codeOK + " found non-existent file(s) in " + db_name;
      std::vector<std::string>::const_iterator iter = files.begin();
      while (iter != files.end())
      {
        answer += "\n" + *iter;
        ++iter;
      } //while
    }
    else
    {
      answer = codeNoContent + " no non-existent files have been found";
    } //else
    return true;
  } //if nonexisting_files
  else
    return false;
}

std::string CommandNonExistingFiles::helpText() const
{
  return "lists files that are in database but not on disk";
}
