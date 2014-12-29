/*
 -------------------------------------------------------------------------------
    This file is part of picdbv.
    Copyright (C) 2014  Thoronador

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

#ifndef PIDDBD_CONSTANTS_H
#define PIDDBD_CONSTANTS_H

#include <string>

//socket location
const std::string serverSocketFile = "/tmp/picdb_server.sock";

//server response codes
const std::string codeContinue              = "100";
const std::string codeOK                    = "200";
const std::string codeNoContent             = "204";
const std::string codeBadRequest            = "400";
const std::string codeRequestEntityTooLarge = "413";
const std::string codeInternalServerError   = "500";

#endif // PIDDBD_CONSTANTS_H
