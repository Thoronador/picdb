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
#include <cstring>
#include <syslog.h>
#include <sys/socket.h>
#include "../daemon/constants.hpp"
#include "../data/DatabaseManager.hpp"
#include "../common/escaping.hpp"
#include "../common/filesystem/functions.hpp"

void picdbvUDSServer::serveClient(const int client_socket_fd, bool& closeWhenDone)
{
  closeWhenDone = false;
  std::string message;
  if (receiveString(client_socket_fd, message))
  {
    std::string answer;
    if (processBuiltInCommands(message, answer, closeWhenDone))
    {
      //stuff is done in function call above
    }
    else if (processManagerCommands(message, answer))
    {
      //stuff is done in function call above
    }
    else if (processDatabaseCommands(message, answer))
    {
      //stuff is done in function call above
    }
    else
    {
      answer = codeBadRequest + " Bad request";
    }
    //send answer to client
    if (!answer.empty())
      sendString(client_socket_fd, escape(answer));
    else
      sendString(client_socket_fd, codeInternalServerError+" Server did not generate a response");
  }
}

void picdbvUDSServer::help(std::string& answer)
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


bool picdbvUDSServer::processBuiltInCommands(const std::string& message, std::string& answer, bool& closeWhenDone)
{
  if (message == "version")
  {
    answer = codeOK + " " + serverVersion;
    return true;
  }
  else if (message == "stop")
  {
    answer = codeOK + " stopping server soon";
    m_shutdown = true;
    closeWhenDone = true;
    return true;
  }
  else if (message == "supported_commands")
  {
    answer = codeOK +" version stop list_dbs create_db delete_db exists_db load_db untagged supported_commands";
    return true;
  }
  else if (message == "help")
  {
    help(answer);
    return true;
  }
  //message was not handled
  return false;
}


bool picdbvUDSServer::processManagerCommands(const std::string& message, std::string& answer)
{
  /* list all database names */
  if (message == "list_dbs")
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

  /* create a new database */
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

  /* delete a database */
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

  /* existence check for a database */
  else if (message.size() > 10 && (message.substr(0, 10) == "exists_db "))
  {
    std::string db_name = message.substr(10);
    //check for spaces in name
    if (db_name.find(' ')==std::string::npos)
    {
      const bool exists = DatabaseManager::get().hasDatabase(db_name);
      if (exists)
        answer = codeOK + " database " + db_name + " exists";
      else
      {
        answer = codeNoContent + " database " + db_name + " does not exist";
      }
    }
    else
    {
      //name contains spaces
      answer = codeBadRequest + " database names shall not contain whitespace characters";
    }
  } //if exists_db

  /* message was not handled */
  else
  {
    return false;
  }
  //message was handled above
  return true;
} // end of processManagerCommands


bool picdbvUDSServer::processDatabaseCommands(const std::string& message, std::string& answer)
{
  /* clear a database */
  if (message.size() > 9 && (message.substr(0, 9) == "clear_db "))
  {
    const std::string db_name = message.substr(9);
    //check for spaces in name
    if (db_name.find(' ')==std::string::npos)
    {
      const bool exists = DatabaseManager::get().hasDatabase(db_name);
      if (!exists)
        answer = codeBadRequest + " database " + db_name + " does not exist";
      else
      {
        DatabaseManager::get().getDatabase(db_name).clearAllData();
        answer = codeOK + " database " + db_name + " was cleared";
      }
    }
    else
    {
      //name contains spaces
      answer = codeBadRequest + " database names shall not contain whitespace characters";
    }
  } //if clear_db

  /* load a database from a specified file */
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
        Database & db = DatabaseManager::get().getDatabase(db_name);
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

  /* list untagged files in a database */
  else if (message.size() > 9 && (message.substr(0, 9) == "untagged "))
  {
    std::string db_name = message.substr(9);
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
  } //if untagged

  /* No match. */
  else
  {
    // message not processed
    return false;
  }
  return true;
} //end of processDatabaseCommands
