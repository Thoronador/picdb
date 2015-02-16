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

#include "CmdHashData.hpp"
#include "../constants.hpp"
#include "../../data/DatabaseManager.hpp"
#include "../../data/Serializer.hpp"

CommandHashData::CommandHashData()
: Command("hash_data")
{ }

bool CommandHashData::processMessage(const std::string& message, std::string& answer) const
{
  /* get a hash's data from a database */
  if (message.size() > 10 && (message.substr(0, 10) == "hash_data "))
  {
    const std::vector<std::string> args = Splitter::splitAtSpaceVector(message.substr(10));
    if (args.size()!=2)
    {
      answer = codeBadRequest + " hash_data needs exactly two arguments: DB name and SHA-256 hash";
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
        SHA256::MessageDigest paramHash;
        if (!paramHash.fromHexString(args[1]))
        {
          answer = codeBadRequest + " \"" + args[1] + "\" is not a valid hash";
        }
        else
        {
          if (db.hasDataForHash(paramHash))
          {
            answer = codeOK + " data for hash " + args[1] + " from database " + db_name + "\n";
            const PicData& data = db.getData(paramHash);
            answer += Serializer::serialize(data);
          }
          else
          {
            answer = codeNoContent + " database " + db_name + " has no data for hash " + args[1];
          }
        }//else
      } //else
    } //else
    return true;
  } //if file_data
  else
    return false;
}

std::string CommandHashData::helpText() const
{
  return "returns the data of a hash from a database";
}
