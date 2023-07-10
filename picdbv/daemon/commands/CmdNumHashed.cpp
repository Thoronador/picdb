/*
 -------------------------------------------------------------------------------
    This file is part of picdbd.
    Copyright (C) 2015, 2023  Dirk Stolle

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

#include "CmdNumHashed.hpp"
#include "../constants.hpp"
#include "../../data/DatabaseManager.hpp"

CommandNumHashed::CommandNumHashed()
: Command("num_hashed")
{ }

bool CommandNumHashed::processMessage(const std::string& message, std::string& answer) const
{
  /* get number of hashed files in a database */
  if (message.size() > 11 && (message.substr(0, 11) == "num_hashed "))
  {
    std::string db_name = message.substr(11);
    // check for spaces in name
    if (db_name.find(' ') == std::string::npos)
    {
      const bool exists = DatabaseManager::get().hasDatabase(db_name);
      if (exists)
        answer = codeOK + std::to_string(DatabaseManager::get().getDatabase(db_name).getNumHashed()) + " hashed files in database " + db_name;
      else
      {
        answer = codeBadRequest + " database " + db_name + " does not exist";
      }
    }
    else
    {
      // name contains spaces
      answer = codeBadRequest + " database names shall not contain whitespace characters";
    }
    return true;
  } //if num_hashed
  else
    return false;
}

std::string CommandNumHashed::helpText() const
{
  return "gets number of hashed files in database";
}
