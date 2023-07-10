/*
 -------------------------------------------------------------------------------
    This file is part of picdbd.
    Copyright (C) 2015, 2023  Dirk Stolle

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

#include "CmdQuery.hpp"
#include "../constants.hpp"
#include "../../data/DatabaseManager.hpp"

CommandQuery::CommandQuery()
: Command("query")
{ }

bool CommandQuery::processMessage(const std::string& message, std::string& answer) const
{
  /* get a file's data from a database */
  if (message.size() > 6 && (message.substr(0, 6) == "query "))
  {
    std::string db_name = "";
    std::string::const_iterator iter = message.begin() + 6;
    while ((iter != message.end()) && (*iter != ' '))
    {
      db_name.push_back(*iter);
      ++iter;
    } //while
    if (iter == message.end())
    {
      // Valid string shall contain query string after db_name.
      answer = codeBadRequest + " query needs exactly two arguments: DB name without spaces and query string";
      return true;
    }
    // move to character after space
    ++iter;
    if (iter == message.end())
    {
      // Valid string shall contain query string after db_name and space.
      answer = codeBadRequest + " query needs exactly two arguments: DB name without spaces and query string";
      return true;
    }
    if (!DatabaseManager::get().hasDatabase(db_name))
    {
      answer = codeBadRequest + " unknown database " + db_name;
      return true;
    }
    const std::vector<std::string> qResult = DatabaseManager::get().getDatabase(db_name).executeQuery(std::string(iter, message.end()));
    if (qResult.empty())
    {
      answer = codeNoContent + " query returned empty set";
    }
    else
    {
      answer = codeOK + " query returned " + std::to_string(qResult.size()) + " match(es)";
      std::vector<std::string>::const_iterator vecIter = qResult.begin();
      while (vecIter != qResult.end())
      {
        answer += "\n" + *vecIter;
        ++vecIter;
      } //while
    } //else
    return true;
  } //if query
  else
    return false;
}

std::string CommandQuery::helpText() const
{
  return "executes a database query";
}
