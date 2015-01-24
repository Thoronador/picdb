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

#include "CmdExistsDB.hpp"
#include "../constants.hpp"
#include "../../data/DatabaseManager.hpp"

CommandExistsDB::CommandExistsDB()
: Command("exists_db")
{ }

bool CommandExistsDB::processMessage(const std::string& message, std::string& answer) const
{
  /* existence check for a database */
  if (message.size() > 10 && (message.substr(0, 10) == "exists_db "))
  {
    std::string db_name = message.substr(10);
    //check for spaces in name
    if (db_name.find(' ')==std::string::npos)
    {
      const bool exists = DatabaseManager::get().hasDatabase(db_name);
      if (exists)
        answer = codeOK + " database " + db_name + " exists";
      else
      {
        answer = codeNoContent + " database " + db_name + " does not exist";
      }
    }
    else
    {
      //name contains spaces
      answer = codeBadRequest + " database names shall not contain whitespace characters";
    }
    return true;
  } //if exists_db
  else
    return false;
}

std::string CommandExistsDB::helpText() const
{
  return "checks existence of a database";
}
