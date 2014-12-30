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
#include "../daemon/DatabaseManager.hpp"
#include "../common/filesystem/functions.hpp"

void picdbvUDSServer::serveClient(const int client_socket_fd, bool& closeWhenDone)
{
  closeWhenDone = false;
  std::string message;
  if (receiveString(client_socket_fd, message))
  {
    //syslog(LOG_INFO, "server received message \": %s", message.c_str());
    std::string answer;
    if (message == "version")
    {
      answer = codeOK + " " + serverVersion;
    }
    else if (message == "stop")
    {
      answer = codeOK + " stopping server soon";
      m_shutdown = true;
      closeWhenDone = true;
    }
    else if (message == "list_dbs")
    {
      const std::set<std::string> dbs = DatabaseManager::get().listAllDatabaseNames();
      if (dbs.empty())
      {
        answer = codeNoContent + " none";
      }
      else
      {
        answer = codeOK;
        std::set<std::string>::const_iterator iter = dbs.begin();
        while (iter!=dbs.end())
        {
          answer += " "+*iter;
          ++iter;
        } //while
      } //else
    } //if list_dbs
    else if (message.size() > 10 && (message.substr(0, 10) == "create_db "))
    {
      std::string db_name = message.substr(10);
      //check for spaces in name
      if (db_name.find(' ')==std::string::npos)
      {
        const bool created = DatabaseManager::get().createDatabase(db_name);
        if (created)
          answer = codeOK + " created database "+db_name;
        else
        {
          answer = codeBadRequest + " could not create database";
        }
      }
      else
      {
        //name contains spaces
        answer = codeBadRequest + " database names shall not contain whitespace characters";
      }
    } //if create_db
    else if (message.size() > 10 && (message.substr(0, 10) == "delete_db "))
    {
      std::string db_name = message.substr(10);
      //check for spaces in name
      if (db_name.find(' ')==std::string::npos)
      {
        const bool created = DatabaseManager::get().deleteDatabase(db_name);
        if (created)
          answer = codeOK + " deleted database "+db_name;
        else
        {
          answer = codeBadRequest + " could not delete database";
        }
      }
      else
      {
        //name contains spaces
        answer = codeBadRequest + " database names shall not contain whitespace characters";
      }
    } //if delete_db
    else if (message.size() > 8 && (message.substr(0, 8) == "load_db "))
    {
      const std::vector<std::string> args = Splitter::splitAtSpaceVector(message.substr(8));
      if (args.size()!=2)
      {
        answer = codeBadRequest + " load_db needs exactly two arguments: DB name and file name without spaces";
      }
      else
      {
        const std::string db_name = args[0];
        if (!DatabaseManager::get().hasDatabase(db_name))
        {
          answer = codeBadRequest + " unknown database "+db_name;
        }
        else if (!isReadable(args[1]))
        {
          answer = codeBadRequest + " file is not readable";
        }
        else
        {
          DataBase & db = DatabaseManager::get().getDatabase(db_name);
          const bool loadSuccess = db.loadFromFile(args[1]);
          if (loadSuccess)
            answer = codeOK + " loaded database "+db_name + " from file";
          else
          {
            answer = codeBadRequest + " could not load database "+db_name + " from file";
          }
        } //else
      } //else
    } //if load_db
    else if (message == "supported_commands")
    {
      answer = codeOK +" version stop list_dbs create_db delete_db load_db supported_commands";
    }
    else if (message == "help")
    {
      help(answer);
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

void picdbvUDSServer::help(std::string& answer)
{
  answer = std::string("List of commonly used commands:\n")
         + "   create_db          - create a new database\n"
         + "   delete_db          - delete a database\n"
         + "   help               - show this help\n"
         + "   list_dbs           - lists all current databases\n"
         + "   load_db            - load database content from a file\n"
         + "   supported_commands - prints a list of supported commands\n"
         + "   stop               - stops the server\n"
         + "   version            - return version of server";
}
