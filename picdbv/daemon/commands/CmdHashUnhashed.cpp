/*
 -----------------------------------------------------------------------------
    This file is part of picdb.
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
 -----------------------------------------------------------------------------
*/

#include "CmdHashUnhashed.hpp"
#include "../constants.hpp"
#include "../../data/DatabaseManager.hpp"
#include "../../../libstriezel/common/StringUtils.hpp"

CommandHashUnhashed::CommandHashUnhashed()
: Command("hash_unhashed")
{ }

bool CommandHashUnhashed::processMessage(const std::string& message, std::string& answer) const
{
  /* adds hashes to unhashed files in a database */
  if (message.size() > 14 && (message.substr(0, 14) == "hash_unhashed "))
  {
    //extract database name
    std::string db_name = "";
    std::string::const_iterator iter = message.begin()+14;
    while ((iter != message.end()) && (*iter != ' '))
    {
      db_name.push_back(*iter);
      ++iter;
    } //while
    if (iter==message.end())
    {
      //Valid string shall contain limit (integer) after db_name.
      answer = codeBadRequest + " hash_unhashed needs three arguments: DB name without spaces, max. number of files to hash (unsigned integer) and base directory name";
      return true;
    }
    //move to character after space
    ++iter;
    if (iter==message.end())
    {
      //Valid string shall contain limit (integer) after db_name and space.
      answer = codeBadRequest + " hash_unhashed needs three arguments: DB name without spaces, max. number of files to hash (unsigned integer) and base directory name";
      return true;
    }

    std::string limitString = "";
    while ((iter != message.end()) && (*iter != ' '))
    {
      limitString.push_back(*iter);
      ++iter;
    } //while
    if (iter==message.end())
    {
      //Valid string shall contain directory path after limit (integer).
      answer = codeBadRequest + " hash_unhashed needs three arguments: DB name without spaces, max. number of files to hash (unsigned integer) and base directory name";
      return true;
    }
    //move to character after space
    ++iter;
    if (iter==message.end())
    {
      //Valid string shall contain directory path after limit (integer).
      answer = codeBadRequest + " hash_unhashed needs three arguments: DB name without spaces, max. number of files to hash (unsigned integer) and base directory name";
      return true;
    }
    unsigned int limit = 1;
    if (!stringToUnsignedInt(limitString, limit))
    {
      answer = codeBadRequest + " second argument of hash_unhashed has to be max. number of files to hash (unsigned integer)";
      return true;
    }
    //extract base directory
    const std::string baseDirectory = std::string(iter, message.end());
    if (!DatabaseManager::get().hasDatabase(db_name))
    {
      answer = codeBadRequest + " unknown database "+db_name;
      return true;
    }
    //add the hashes
    DatabaseManager::get().getDatabase(db_name).hashUnhashedFiles(baseDirectory, limit);
    answer = codeOK + " some files in database " + db_name + " might have been hashed";
    return true;
  } //if hash_unhashed
  else
    return false;
}

std::string CommandHashUnhashed::helpText() const
{
  return "adds hashes to unhashed files in a database";
}
