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

#include "CmdLoadDB.hpp"
#include "../constants.hpp"
#include "../../data/DatabaseManager.hpp"
#include "../../common/filesystem/functions.hpp"

CommandLoadDB::CommandLoadDB()
: Command("load_db")
{ }

bool CommandLoadDB::processMessage(const std::string& message, std::string& answer) const
{
  /* load a database from a specified file */
  if (message.size() > 8 && (message.substr(0, 8) == "load_db "))
  {
    const std::vector<std::string> args = Splitter::splitAtSpaceVector(message.substr(8));
    if (args.size()!=2)
    {
      answer = codeBadRequest + " load_db needs exactly two arguments: DB name and file name without spaces";
    }
    else
    {
      const std::string db_name = args[0];
      if (!DatabaseManager::get().hasDatabase(db_name))
      {
        answer = codeBadRequest + " unknown database "+db_name;
      }
      else if (!isReadable(args[1]))
      {
        answer = codeBadRequest + " file is not readable";
      }
      else
      {
        Database & db = DatabaseManager::get().getDatabase(db_name);
        const bool loadSuccess = db.loadFromFile(args[1]);
        if (loadSuccess)
          answer = codeOK + " loaded database "+db_name + " from file";
        else
        {
          answer = codeBadRequest + " could not load database "+db_name + " from file";
        }
      } //else
    } //else
    return true;
  } //if load_db
  else
    return false;
}
