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

#include "CmdDeleteDB.hpp"
#include "../constants.hpp"
#include "../../data/DatabaseManager.hpp"

CommandDeleteDB::CommandDeleteDB()
: Command("delete_db")
{ }

bool CommandDeleteDB::processMessage(const std::string& message, std::string& answer) const
{
  /* delete a database */
  if (message.size() > 10 && (message.substr(0, 10) == "delete_db "))
  {
    const std::string db_name = message.substr(10);
    //check for spaces in name
    if (db_name.find(' ')==std::string::npos)
    {
      const bool created = DatabaseManager::get().deleteDatabase(db_name);
      if (created)
        answer = codeOK + " deleted database "+db_name;
      else
      {
        answer = codeBadRequest + " could not delete database";
      }
    }
    else
    {
      //name contains spaces
      answer = codeBadRequest + " database names shall not contain whitespace characters";
    }
    return true;
  } //if delete_db
  else
    return false;
}

std::string CommandDeleteDB::helpText() const
{
  return "delete a database";
}
