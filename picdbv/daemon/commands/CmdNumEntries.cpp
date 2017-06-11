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

#include "CmdNumEntries.hpp"
#include "../constants.hpp"
#include "../../data/DatabaseManager.hpp"
#include "../../../libstriezel/common/StringUtils.hpp"

CommandNumEntries::CommandNumEntries()
: Command("num_entries")
{ }

bool CommandNumEntries::processMessage(const std::string& message, std::string& answer) const
{
  /* get number of entries in a database */
  if (message.size() > 12 && (message.substr(0, 12) == "num_entries "))
  {
    std::string db_name = message.substr(12);
    //check for spaces in name
    if (db_name.find(' ')==std::string::npos)
    {
      const bool exists = DatabaseManager::get().hasDatabase(db_name);
      if (exists)
        answer = codeOK + intToString(DatabaseManager::get().getDatabase(db_name).getNumEntries()) + " entries in database " + db_name;
      else
      {
        answer = codeBadRequest + " database " + db_name + " does not exist";
      }
    }
    else
    {
      //name contains spaces
      answer = codeBadRequest + " database names shall not contain whitespace characters";
    }
    return true;
  } //if num_entries
  else
    return false;
}

std::string CommandNumEntries::helpText() const
{
  return "gets number of entries in database";
}
