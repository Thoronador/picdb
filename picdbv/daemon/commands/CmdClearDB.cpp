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

#include "CmdClearDB.hpp"
#include "../constants.hpp"
#include "../../data/DatabaseManager.hpp"

CommandClearDB::CommandClearDB()
: Command("clear_db")
{ }

bool CommandClearDB::processMessage(const std::string& message, std::string& answer) const
{
  /* clear a database */
  if (message.size() > 9 && (message.substr(0, 9) == "clear_db "))
  {
    const std::string db_name = message.substr(9);
    //check for spaces in name
    if (db_name.find(' ')==std::string::npos)
    {
      const bool exists = DatabaseManager::get().hasDatabase(db_name);
      if (!exists)
        answer = codeBadRequest + " database " + db_name + " does not exist";
      else
      {
        DatabaseManager::get().getDatabase(db_name).clearAllData();
        answer = codeOK + " database " + db_name + " was cleared";
      }
    }
    else
    {
      //name contains spaces
      answer = codeBadRequest + " database names shall not contain whitespace characters";
    }
    return true;
  } //if clear_db
  else
    return false;
}

std::string CommandClearDB::helpText() const
{
  return "deletes all entries from a database";
}
