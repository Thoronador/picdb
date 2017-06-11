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

#include "CmdPurge.hpp"
#include "../constants.hpp"
#include "../../data/DatabaseManager.hpp"

CommandPurge::CommandPurge()
: Command("purge")
{ }

bool CommandPurge::processMessage(const std::string& message, std::string& answer) const
{
  /* purge/remove files that are in database but not on disk anymore */
  if (message.size() > 6 && (message.substr(0, 6) == "purge "))
  {
    std::string db_name = "";
    std::string::const_iterator iter = message.begin()+6;
    while ((iter != message.end()) && (*iter != ' '))
    {
      db_name.push_back(*iter);
      ++iter;
    } //while
    if (iter==message.end())
    {
      //Valid string shall contain directory after db_name.
      answer = codeBadRequest + " purge needs exactly two arguments: DB name without spaces and directory path";
      return true;
    }
    //move to character after space
    ++iter;
    if (iter==message.end())
    {
      //Valid string shall contain directory after db_name and space.
      answer = codeBadRequest + " purge needs exactly two arguments: DB name without spaces and directory path";
      return true;
    }
    if (!DatabaseManager::get().hasDatabase(db_name))
    {
      answer = codeBadRequest + " unknown database "+db_name;
      return true;
    }

    DatabaseManager::get().getDatabase(db_name).purgeNonexistingFiles(std::string(iter, message.end()));
    answer = codeOK + " purged database " + db_name;
    return true;
  } //if purge
  else
    return false;
}

std::string CommandPurge::helpText() const
{
  return "purge files that are in database but not on disk";
}
