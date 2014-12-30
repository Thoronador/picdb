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

#ifndef PICDBVUDSSERVER_HPP
#define PICDBVUDSSERVER_HPP

#include "UnixDomainSocketServer.hpp"

class picdbvUDSServer: public UnixDomainSocketServer
{
  protected:
    /** \brief virtual function that handles the client/server transactions. Has to be
     * implemented in a derived class.
     *
     * \param client_socket_fd contains the file descriptor of the client socket
                               that is connected with the server
     */
    virtual void serveClient(const int client_socket_fd, bool& closeWhenDone);
  private:
    /** \brief fills a given string with the server's help message
     *
     * \param answer string that will be used to store the help message
     */
    void help(std::string& answer);
}; //class

#endif // PICDBVUDSSERVER_HPP
