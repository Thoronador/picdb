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

#include "CmdSaveDB.hpp"
#include "../constants.hpp"
#include "../../data/DatabaseManager.hpp"
#include "../../common/filesystem/functions.hpp"

CommandSaveDB::CommandSaveDB()
: Command("save_db")
{ }

bool CommandSaveDB::processMessage(const std::string& message, std::string& answer) const
{
  /* load a database from a specified file */
  if (message.size() > 8 && (message.substr(0, 8) == "save_db "))
  {
    std::string db_name = "";
    std::string::const_iterator iter = message.begin()+8;
    while ((iter != message.end()) && (*iter != ' '))
    {
      db_name.push_back(*iter);
      ++iter;
    } //while
    if (iter==message.end())
    {
      //Valid string shall contain filename after db_name.
      answer = codeBadRequest + " save_db needs exactly two arguments: DB name without spaces and filename";
      return true;
    }
    //move to character after space
    ++iter;
    if (iter==message.end())
    {
      //Valid string shall contain filename after db_name and space.
      answer = codeBadRequest + " save_db needs exactly two arguments: DB name without spaces and filename";
      return true;
    }
    if (!DatabaseManager::get().hasDatabase(db_name))
    {
      answer = codeBadRequest + " unknown database "+db_name;
      return true;
    }

    const bool success = DatabaseManager::get().getDatabase(db_name).saveToFile(std::string(iter, message.end()));
    if (success)
    {
      answer = codeOK + " database " + db_name + " was saved to file";
    }
    else
    {
      answer = codeBadRequest + " could not save database "+db_name + " to file";
    } //else
    return true;
  } //if save_db
  else
    return false;
}

std::string CommandSaveDB::helpText() const
{
  return "saves database content to a file";
}
