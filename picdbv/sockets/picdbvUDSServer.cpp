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

#include "picdbvUDSServer.hpp"
#include <cstring>
#include <syslog.h>
#include <sys/socket.h>
#include "../daemon/constants.hpp"

void picdbvUDSServer::serveClient(const int client_socket_fd, bool& closeWhenDone)
{
  closeWhenDone = false;
  std::string message;
  if (receiveString(client_socket_fd, message))
  {
    syslog(LOG_INFO, "server received message \": %s", message.c_str());
    std::string answer;
    if (message == "version")
    {
      answer = codeOK + " 20141228.1";
    }
    else if (message == "stop")
    {
      answer = codeOK + " stopping server soon";
      m_shutdown = true;
      closeWhenDone = true;
    }
    else if (message == "list_dbs")
    {
      answer = codeNoContent + " none";
    }
    else if (message == "supported_commands")
    {
      answer = codeOK +" version stop list_dbs supported_commands";
    }
    else
    {
      answer = codeBadRequest + " Bad request";
    }
    //send answer to client
    if (!answer.empty())
      sendString(client_socket_fd, answer);
    else
      sendString(client_socket_fd, codeInternalServerError+" Server did not generate a response");
  }
}
