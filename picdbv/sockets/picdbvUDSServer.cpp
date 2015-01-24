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
#include <map>
#include "../daemon/constants.hpp"
#include "../common/escaping.hpp"
//all implemented commands
// ---- built in
#include "../daemon/commands/CmdVersion.hpp"
#include "../daemon/commands/CmdListDatabases.hpp"
// ---- DB management
#include "../daemon/commands/CmdCreateDB.hpp"
#include "../daemon/commands/CmdDeleteDB.hpp"
#include "../daemon/commands/CmdExistsDB.hpp"
// ---- DB-related
#include "../daemon/commands/CmdClearDB.hpp"
#include "../daemon/commands/CmdLoadDB.hpp"
#include "../daemon/commands/CmdUntagged.hpp"
#include "../daemon/commands/CmdAutoTagDB.hpp"
#include "../daemon/commands/CmdAddFile.hpp"
#include "../daemon/commands/CmdFileExists.hpp"
#include "../daemon/commands/CmdListFiles.hpp"
#include "../daemon/commands/CmdDeleteFile.hpp"
#include "../daemon/commands/CmdFileData.hpp"

std::string padString(std::string str, const std::string::size_type n)
{
  const std::string::size_type len = str.size();
  if (n <= len || n == std::string::npos)
    return str;
  return str + std::string(n-len, ' ');
}

picdbvUDSServer::picdbvUDSServer()
: UnixDomainSocketServer(), m_Commands(std::vector<std::unique_ptr<Command> >())
{
  // add some "built-in" commands
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
  m_Commands.push_back(std::unique_ptr<CommandAutoTagDB>(new CommandAutoTagDB()));
  m_Commands.push_back(std::unique_ptr<CommandAddFile>(new CommandAddFile()));
  m_Commands.push_back(std::unique_ptr<CommandFileExists>(new CommandFileExists()));
  m_Commands.push_back(std::unique_ptr<CommandListFiles>(new CommandListFiles()));
  m_Commands.push_back(std::unique_ptr<CommandDeleteFile>(new CommandDeleteFile()));
  m_Commands.push_back(std::unique_ptr<CommandFileData>(new CommandFileData()));
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
    } //if stop
    else if (message == "help")
    {
      std::map<std::string, std::string> helpTexts;
      helpTexts["help"] = "shows this help message";
      helpTexts["stop"] = "stops the server";
      helpTexts["supported_commands"] = "prints a list of supported commands";
      std::string::size_type padding = 18; //length of "supported_commands"
      std::vector<std::unique_ptr<Command> >::const_iterator cmdIter = m_Commands.begin();
      while (cmdIter != m_Commands.end())
      {
        const std::string curName = cmdIter->get()->getName();
        helpTexts[curName] = cmdIter->get()->helpText();
        if (curName.size() > padding)
          padding = curName.size();
        ++cmdIter;
      } //while

      answer = std::string("List of commonly used commands:");
      std::map<std::string, std::string>::const_iterator mapIter = helpTexts.begin();
      while (mapIter != helpTexts.end())
      {
        answer += "\n    " + padString(mapIter->first, padding) + " - " + mapIter->second;
        ++mapIter;
      } //while
    } //if help
    else if (message == "supported_commands")
    {
      answer = codeOK +" stop supported_commands";
      std::vector<std::unique_ptr<Command> >::const_iterator iter = m_Commands.begin();
      while (iter != m_Commands.end())
      {
        answer += " " + iter->get()->getName();
        ++iter;
      }//while
    } //if supported_commands
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
