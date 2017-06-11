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

#include "CmdUnknownWho.hpp"
#include "../constants.hpp"
#include "../../data/DatabaseManager.hpp"

CommandUnknownWho::CommandUnknownWho()
: Command("no_who")
{ }

bool CommandUnknownWho::processMessage(const std::string& message, std::string& answer) const
{
  /* list untagged files in a database */
  if (message.size() > 7 && (message.substr(0, 7) == "no_who "))
  {
    const std::string db_name = message.substr(7);
    //check for spaces in name
    if (db_name.find(' ')==std::string::npos)
    {
      if (!DatabaseManager::get().hasDatabase(db_name))
        answer = codeBadRequest + " database does not exist";
      else
      {
        const std::vector<std::string> result = DatabaseManager::get().getDatabase(db_name).getUnknownWhoFiles();
        if (result.empty())
        {
          answer = codeNoContent + " none";
        }
        else
        {
          answer = codeOK;
          unsigned int i;
          for (i=0; i<result.size(); ++i)
          {
            answer += std::string(1, '\0') + result[i];
          } //for
        } //else
      } //else
    } //if
    else
    {
      //name contains spaces
      answer = codeBadRequest + " database names shall not contain whitespace characters";
    }
    return true;
  } //if no_artist
  else
    return false;
}

std::string CommandUnknownWho::helpText() const
{
  return "list files without \"who\" info in a database";
}
