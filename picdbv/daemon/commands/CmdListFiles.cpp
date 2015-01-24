/*
 -------------------------------------------------------------------------------
    This file is part of picdbd.
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
 -------------------------------------------------------------------------------
*/

#include "CmdListFiles.hpp"
#include "../constants.hpp"
#include "../../data/DatabaseManager.hpp"
#include "../../../libthoro/common/StringUtils.h"

CommandListFiles::CommandListFiles()
: Command("list_files")
{ }

bool CommandListFiles::processMessage(const std::string& message, std::string& answer) const
{
  /* lists all files in a database */
  if (message.size() > 11 && (message.substr(0, 11) == "list_files "))
  {
    const std::string db_name = message.substr(11);
    //check for spaces in name
    if (db_name.find(' ') == std::string::npos)
    {
      if (DatabaseManager::get().hasDatabase(db_name))
      {
        const std::set<std::string> result = DatabaseManager::get().getDatabase(db_name).getListedFiles();
        answer = codeOK + " database " + db_name + " contains " + uintToString(result.size()) + " files";
        std::set<std::string>::const_iterator setIterator = result.begin();
        while (setIterator != result.end())
        {
          answer += '\n' + *setIterator;
          ++setIterator;
        } //while
      }
      else
      {
        answer = codeBadRequest + " database " + db_name + " does not exist";
      }
    } //if no space character in name
    else
    {
      //name contains spaces
      answer = codeBadRequest + " database names shall not contain whitespace characters";
    }
    return true;
  } //if list_files
  else
    return false;
}

std::string CommandListFiles::helpText() const
{
  return "list all files in a database";
}
