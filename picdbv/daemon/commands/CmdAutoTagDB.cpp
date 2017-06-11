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

#include "CmdAutoTagDB.hpp"
#include "../constants.hpp"
#include "../../data/DatabaseManager.hpp"

CommandAutoTagDB::CommandAutoTagDB()
: Command("autotag_db")
{ }

bool CommandAutoTagDB::processMessage(const std::string& message, std::string& answer) const
{
  /* tries to guess names and artists of pictures by analyzing files */
  if (message.size() > 11 && (message.substr(0, 11) == "autotag_db "))
  {
    const std::string db_name = message.substr(11);
    //check for spaces in name
    if (db_name.find(' ')==std::string::npos)
    {
      const bool exists = DatabaseManager::get().hasDatabase(db_name);
      if (!exists)
        answer = codeBadRequest + " database " + db_name + " does not exist";
      else
      {
        if (DatabaseManager::get().getDatabase(db_name).AutoTag_Splitter())
          answer = codeOK + " database " + db_name + " was automatically \"tagged\"";
        else
          answer = codeInternalServerError + " database " + db_name + " could not be \"tagged\"";
      }
    }
    else
    {
      //name contains spaces
      answer = codeBadRequest + " database names shall not contain whitespace characters";
    }
    return true;
  } //if autotag_db
  else
    return false;
}

std::string CommandAutoTagDB::helpText() const
{
  return "try to guess names and artists of pictures by analyzing files";
}
