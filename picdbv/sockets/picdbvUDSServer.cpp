/*
 -------------------------------------------------------------------------------
    This file is part of picdbd.
    Copyright (C) 2014, 2015  Thoronador

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
#include "../daemon/constants.hpp"
#include "../common/escaping.hpp"
//all implemented commands
#include "../daemon/commands/CmdVersion.hpp"
#include "../daemon/commands/CmdHelp.hpp"
#include "../daemon/commands/CmdListDatabases.hpp"
#include "../daemon/commands/CmdCreateDB.hpp"
#include "../daemon/commands/CmdDeleteDB.hpp"
#include "../daemon/commands/CmdExistsDB.hpp"
#include "../daemon/commands/CmdClearDB.hpp"
#include "../daemon/commands/CmdLoadDB.hpp"
#include "../daemon/commands/CmdUntagged.hpp"

picdbvUDSServer::picdbvUDSServer()
: UnixDomainSocketServer(), m_Commands(std::vector<std::unique_ptr<Command> >())
{
  // add some "built-in" commands
  m_Commands.push_back(std::unique_ptr<CommandHelp>(new CommandHelp()));
  m_Commands.push_back(std::unique_ptr<CommandVersion>(new CommandVersion()));
  // add DatabaseManager-related commands
  m_Commands.push_back(std::unique_ptr<CommandListDatabases>(new CommandListDatabases()));
  m_Commands.push_back(std::unique_ptr<CommandCreateDatabase>(new CommandCreateDatabase()));
  m_Commands.push_back(std::unique_ptr<CommandDeleteDB>(new CommandDeleteDB()));
  m_Commands.push_back(std::unique_ptr<CommandExistsDB>(new CommandExistsDB()));
  // add Database-related commands
  m_Commands.push_back(std::unique_ptr<CommandClearDB>(new CommandClearDB()));
  m_Commands.push_back(std::unique_ptr<CommandLoadDB>(new CommandLoadDB()));
  m_Commands.push_back(std::unique_ptr<CommandUntagged>(new CommandUntagged()));
}

void picdbvUDSServer::serveClient(const int client_socket_fd, bool& closeWhenDone)
{
  closeWhenDone = false;
  std::string message;
  if (receiveString(client_socket_fd, message))
  {
    std::string answer;

    if (message == "stop")
    {
      answer = codeOK + " stopping server soon";
      m_shutdown = true;
      closeWhenDone = true;
    }
    else if (message == "supported_commands")
    {
      answer = codeOK +" stop supported_commands";
      std::vector<std::unique_ptr<Command> >::const_iterator iter = m_Commands.begin();
      while (iter != m_Commands.end())
      {
        answer += " " + iter->get()->getName();
        ++iter;
      }//while
    }
    else
    {
      bool processed = false;
      std::vector<std::unique_ptr<Command> >::const_iterator iter = m_Commands.begin();
      while (iter != m_Commands.end())
      {
        processed = iter->get()->processMessage(message, answer);
        if (processed)
          break;
        ++iter;
      }//while

      if (!processed)
      {
        answer = codeBadRequest + " Bad request";
      }
    }
    //send answer to client
    if (!answer.empty())
      sendString(client_socket_fd, escape(answer));
    else
      sendString(client_socket_fd, codeInternalServerError+" Server did not generate a response");
  }
}
