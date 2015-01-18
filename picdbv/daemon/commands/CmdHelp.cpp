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

#include "CmdHelp.hpp"
#include "../constants.hpp"

CommandHelp::CommandHelp()
: Command("help")
{ }

bool CommandHelp::processMessage(const std::string& message, std::string& answer) const
{
  if (message == "help")
  {
    help(answer);
    return true;
  }
  return false;
}

void CommandHelp::help(std::string& answer)
{
  answer = std::string("List of commonly used commands:\n")
         + "   create_db          - create a new database\n"
         + "   delete_db          - delete a database\n"
         + "   exists_db          - checks existence of a database\n"
         + "   help               - show this help\n"
         + "   list_dbs           - lists all current databases\n"
         + "   load_db            - load database content from a file\n"
         + "   supported_commands - prints a list of supported commands\n"
         + "   stop               - stops the server\n"
         + "   untagged           - list untagged files in a database\n"
         + "   version            - return version of server";
}
