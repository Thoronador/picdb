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

#include "CmdHashExists.hpp"
#include "../constants.hpp"
#include "../../data/DatabaseManager.hpp"

CommandHashExists::CommandHashExists()
: Command("hash_exists")
{ }

bool CommandHashExists::processMessage(const std::string& message, std::string& answer) const
{
  /* check for existence of hash in a database */
  if (message.size() > 12 && (message.substr(0, 12) == "hash_exists "))
  {
    const std::vector<std::string> args = Splitter::splitAtSpaceVector(message.substr(12));
    if (args.size()!=2)
    {
      answer = codeBadRequest + " hash_exists needs exactly two arguments: DB name and hexadecimal SHA-256 hash without spaces";
    }
    else
    {
      SHA256::MessageDigest paramHash;
      if (!paramHash.fromHexString(args[1]))
      {
        answer = codeBadRequest + " \"" + args[1] + "\" is not a valid hash";
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
          if (db.hasDataForHash(paramHash))
            answer = codeOK + " database " + db_name + " contains hash " + args[1];
          else
          {
            answer = codeNoContent + " database " + db_name + " has no hash data for " + args[1];
          }
        } //else
      } //else
    } //else
    return true;
  } //if hash_exists
  else
    return false;
}

std::string CommandHashExists::helpText() const
{
  return "checks for existence of a hash in a database";
}
