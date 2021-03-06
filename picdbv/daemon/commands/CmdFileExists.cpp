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

#include "CmdFileExists.hpp"
#include "../constants.hpp"
#include "../../data/DatabaseManager.hpp"

CommandFileExists::CommandFileExists()
: Command("file_exists")
{ }

bool CommandFileExists::processMessage(const std::string& message, std::string& answer) const
{
  /* check for existence of file in a database */
  if (message.size() > 12 && (message.substr(0, 12) == "file_exists "))
  {
    const std::vector<std::string> args = Splitter::splitAtSpaceVector(message.substr(12));
    if (args.size()!=2)
    {
      answer = codeBadRequest + " file_exists needs exactly two arguments: DB name and file name without spaces";
    }
    else
    {
      const std::string db_name = args[0];
      if (!DatabaseManager::get().hasDatabase(db_name))
      {
        answer = codeBadRequest + " unknown database "+db_name;
      }
      else
      {
        const Database & db = DatabaseManager::get().getDatabase(db_name);
        if (db.hasFile(args[1]))
          answer = codeOK + " database " + db_name + " contains file " + args[1];
        else
        {
          answer = codeNoContent + " database " + db_name + " has no file " + args[1];
        }
      } //else
    } //else
    return true;
  } //if file_exists
  else
    return false;
}

std::string CommandFileExists::helpText() const
{
  return "checks for existence of a file in a database";
}
