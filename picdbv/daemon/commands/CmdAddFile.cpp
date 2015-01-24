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

#include "CmdAddFile.hpp"
#include "../constants.hpp"
#include "../../data/DatabaseManager.hpp"
#include "../../data/Serializer.hpp"

CommandAddFile::CommandAddFile()
: Command("add_file")
{ }

bool CommandAddFile::processMessage(const std::string& message, std::string& answer) const
{
  /* adds data of a file to a database or updates the data, if the file is already there */
  if (message.size() > 9 && (message.substr(0, 9) == "add_file "))
  {
    const std::vector<std::string> lines = Splitter::splitAtSeparator(message.substr(9), '\n');
    if (lines.size()<2)
    {
      answer = codeBadRequest + " add_file needs several lines as arguments: "
             + "DB name and file name without spaces in the first line, serialized PicData in the following lines.";
    }
    else
    {
      const std::vector<std::string> args = Splitter::splitAtSpaceVector(lines[0]);
      const std::string db_name = args[0];
      if (!DatabaseManager::get().hasDatabase(db_name))
      {
        answer = codeBadRequest + " unknown database "+db_name;
      }
      else
      {
        PicData pd;
        std::string data = message.substr(lines[0].size()+1);

        if (!Serializer::unserialize(data, pd))
        {
          answer = codeBadRequest + " input is not deserializable.";
        }
        else
        {
          const std::string fileName = lines[0].substr(db_name.size()+1);
          if (!fileName.empty())
          {
            DatabaseManager::get().getDatabase(db_name).addFile(fileName, pd);
            answer = codeOK + " file \"" + fileName + "\" was added/updated";
          }
          else
          {
            answer = codeBadRequest + " empty file name is not allowed";
          }
        } //else
      } //else
    } //else

    return true;
  } //if add_file
  else
    return false;
}
