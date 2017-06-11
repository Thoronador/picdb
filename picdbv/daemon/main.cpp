/*
 -------------------------------------------------------------------------------
    This file is part of picdbv.
    Copyright (C) 2014, 2015  Dirk Stolle

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
#include "Daemonic.hpp"
#include "../sockets/picdbvUDSServer.hpp"
#include <syslog.h>
#include "constants.hpp"

int main()
{
  if (!Daemonic::get().daemonize(std::string("picdbd")))
  {
    syslog(LOG_INFO | LOG_DAEMON, "Could not create daemon!");
  }
  else
  {
      syslog(LOG_INFO | LOG_DAEMON, "Daemonize succeeded!");

      picdbvUDSServer server;
      if (!server.activate(serverSocketFile))
      {
        syslog(LOG_INFO | LOG_DAEMON, "Could not activate socket server!");
        return 1;
      }
      syslog(LOG_INFO | LOG_DAEMON, "Server is now active.");
      while (server.startListening(5000))
      {
        //syslog(LOG_INFO | LOG_DAEMON, "next loop iteration of server");
        if (server.shutdownRequested())
          break;
      }
      server.deactivate();
      syslog(LOG_INFO | LOG_DAEMON, "Server deactivated");
  }
  return 0;
}
