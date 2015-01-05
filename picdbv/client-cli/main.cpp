/*
 -------------------------------------------------------------------------------
    This file is part of picdb.
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

#include <iostream>
#include <cstring>
#include "../common/escaping.hpp"
#include "../daemon/constants.hpp"
#include "../sockets/UnixDomainSocketClient.hpp"

const int rcInvalidParameter = 1;
const int rcSocketError      = 2;


void showVersion()
{
  std::cout << "picdb-client-cli, version 1.0, 2015-01-05\n";
}

void showHelp()
{
  std::cout << "\npicdb-client-cli [OPTIONS]\n"
            << "\npicdb-client-cli <request string>\n"
            << "\n"
            << "  picdb-client-cli is a command line programme to send a request to a picdbd\n"
            << "  server and get the server's answer to the request.\n"
            << "\n"
            << "options:\n"
            << "  --help        - displays this help message and quits\n"
            << "  -?            - same as --help\n"
            << "  --version     - displays the version of the programme and quits\n"
            /*
            << "  -interactive  - starts the client in interactive mode which allows to\n"
            << "                  enter several requests commands\n"
            << "  -i            - same as --interactive\n"
            */
            ;
}

int main(int argc, char **argv)
{
  std::string request;
  //bool interactive = false;

  if (argc>1 and argv!=NULL)
  {
    int i=1;
    while (i<argc)
    {
      if (argv[i]!=NULL)
      {
        const std::string param = std::string(argv[i]);
        //help parameter
        if ((param=="--help") or (param=="-h") or (param=="-?") or (param=="/?"))
        {
          showHelp();
          return 0;
        }//if help wanted
        //version information requested?
        else if (param=="--version")
        {
          showVersion();
          return 0;
        }
        /*
        //interactive mode
        else if ((param=="--interactive") or (param=="-i"))
        {
          interactive = true;
          std::cout << "Requested interactive mode.\n";
        }//interactive mode
        */
        else if (param.substr(0, 2)=="--")
        {
          //unknown or wrong parameter
          std::cout << "Invalid parameter given: \""<<param<<"\".\n"
                    << "Use --help to get a list of valid parameters.\n";
          return rcInvalidParameter;
        }
        //argument is part of request?
        else
        {
          if (request.empty())
            request = param;
          else
            request += " " + param;
        }//part of request
      }//parameter exists
      else
      {
        std::cout << "Error: Parameter at index "<<i<<" is NULL.\n";
        return rcInvalidParameter;
      }
      ++i;//on to next parameter
    }//while
  }//if argc

  //is there any request so far?
  if (request.empty() /* and !interactive*/)
  {
    std::cout << "Error: No request string given as argument!\n";
    return rcInvalidParameter;
  }

  UnixDomainSocketClient socketCli;
  if (!socketCli.connectToServer(serverSocketFile))
  {
    std::cout << "Could not connect to server!\n"  ;
    return rcSocketError;
  }
  if (!socketCli.sendString(request))
  {
    std::cout << "Could not send request to server!\n";
    socketCli.closeConnection();
    return rcSocketError;
  }

  std::string answer = "";
  if (!socketCli.receiveString(answer))
  {
    std::cout << "Error: Could not receive answer from server.\n";
    socketCli.closeConnection();
    return rcSocketError;
  }
  socketCli.closeConnection();
  unescapeInPlace(answer);
  if (answer.empty())
  {
    std::cout << "Received empty answer from server, this should not happen!\n";
    return rcSocketError;
  }
  std::cout << answer<<"\n";
  return 0;
}
