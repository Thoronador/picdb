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

#include "CmdUntagged.hpp"
#include "../constants.hpp"
#include "../../data/DatabaseManager.hpp"

CommandUntagged::CommandUntagged()
: Command("untagged")
{ }

bool CommandUntagged::processMessage(const std::string& message, std::string& answer) const
{
  /* list untagged files in a database */
  if (message.size() > 9 && (message.substr(0, 9) == "untagged "))
  {
    const std::string db_name = message.substr(9);
    //check for spaces in name
    if (db_name.find(' ')==std::string::npos)
    {
      if (!DatabaseManager::get().hasDatabase(db_name))
        answer = codeBadRequest + " database does not exist";
      else
      {
        const std::vector<std::string> result = DatabaseManager::get().getDatabase(db_name).getUntaggedFiles();
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
  } //if untagged
  else
    return false;
}

std::string CommandUntagged::helpText() const
{
  return "list untagged files in a database";
}
