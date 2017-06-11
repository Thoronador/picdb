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

#include "CmdDeleteFile.hpp"
#include "../constants.hpp"
#include "../../data/DatabaseManager.hpp"

CommandDeleteFile::CommandDeleteFile()
: Command("delete_file")
{ }

bool CommandDeleteFile::processMessage(const std::string& message, std::string& answer) const
{
  /* delete a file from a database */
  if (message.size() > 12 && (message.substr(0, 12) == "delete_file "))
  {
    const std::vector<std::string> args = Splitter::splitAtSpaceVector(message.substr(12));
    if (args.size()!=2)
    {
      answer = codeBadRequest + " delete_file needs exactly two arguments: DB name and file name without spaces";
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
        Database & db = DatabaseManager::get().getDatabase(db_name);
        if (db.deleteFile(args[1]))
          answer = codeOK + " file " + args[1] + " was deleted from database " + db_name;
        else
        {
          answer = codeNoContent + " database " + db_name + " has no file " + args[1];
        }
      } //else
    } //else
    return true;
  } //if delete_file
  else
    return false;
}

std::string CommandDeleteFile::helpText() const
{
  return "deletes a file from a database";
}
