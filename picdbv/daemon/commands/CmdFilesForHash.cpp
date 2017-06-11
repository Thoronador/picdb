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

#include "CmdFilesForHash.hpp"
#include "../constants.hpp"
#include "../../data/DatabaseManager.hpp"
#include "../../../libstriezel/common/StringUtils.hpp"

CommandFilesForHash::CommandFilesForHash()
: Command("files_for_hash")
{ }

bool CommandFilesForHash::processMessage(const std::string& message, std::string& answer) const
{
  /* get a hash's data from a database */
  if (message.size() > 15 && (message.substr(0, 15) == "files_for_hash "))
  {
    const std::vector<std::string> args = Splitter::splitAtSpaceVector(message.substr(15));
    if (args.size()!=2)
    {
      answer = codeBadRequest + " files_for_hash needs exactly two arguments: DB name and SHA-256 hash";
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
        SHA256::MessageDigest paramHash;
        if (!paramHash.fromHexString(args[1]))
        {
          answer = codeBadRequest + " \"" + args[1] + "\" is not a valid hash";
        }
        else
        {
          const Database & db = DatabaseManager::get().getDatabase(db_name);
          const std::set<std::string> result = db.getAllFilesForHash(paramHash);
          if (!result.empty())
          {
            answer = codeOK + " found " + intToString(result.size()) + " file(s) for hash " + args[1] + " in database " + db_name;
            std::set<std::string>::const_iterator iter = result.begin();
            while (iter != result.end())
            {
              answer += "\n" + *iter;
              ++iter;
            } //while
          }
          else
          {
            answer = codeNoContent + " database " + db_name + " has no files for hash " + args[1];
          }
        }//else
      } //else
    } //else
    return true;
  } //if files_for_hash
  else
    return false;
}

std::string CommandFilesForHash::helpText() const
{
  return "returns a list of files with a given hash";
}
