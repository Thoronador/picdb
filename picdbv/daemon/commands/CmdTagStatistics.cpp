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

#include "CmdTagStatistics.hpp"
#include <sstream>
#include "../constants.hpp"
#include "../../data/DatabaseManager.hpp"

CommandTagStatistics::CommandTagStatistics()
: Command("tag_stats")
{ }

bool CommandTagStatistics::processMessage(const std::string& message, std::string& answer) const
{
  /* show tag statistics */
  if (message.size() > 10 && (message.substr(0, 10) == "tag_stats "))
  {
    const std::string db_name = message.substr(10);
    //check for spaces in name
    if (db_name.find(' ')==std::string::npos)
    {
      const bool exists = DatabaseManager::get().hasDatabase(db_name);
      if (!exists)
        answer = codeBadRequest + " database " + db_name + " does not exist";
      else
      {
        std::ostringstream outStringStream;
        DatabaseManager::get().getDatabase(db_name).showTagStatistics(outStringStream);
        answer = codeOK + " "+outStringStream.str();
      }
    }
    else
    {
      //name contains spaces
      answer = codeBadRequest + " database names shall not contain whitespace characters";
    }
    return true;
  } //if tag_stats
  else
    return false;
}

std::string CommandTagStatistics::helpText() const
{
  return "shows tag statistics of a database";
}
