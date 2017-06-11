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

#include "CmdVersion.hpp"
#include "../constants.hpp"

CommandVersion::CommandVersion()
: Command("version")
{ }

bool CommandVersion::processMessage(const std::string& message, std::string& answer) const
{
  if (message == "version")
  {
    answer = codeOK + " " + serverVersion;
    return true;
  }
  return false;
}

std::string CommandVersion::helpText() const
{
  return "return version of server";
}
