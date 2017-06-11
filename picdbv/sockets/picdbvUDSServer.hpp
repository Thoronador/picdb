/*
 -------------------------------------------------------------------------------
    This file is part of picdbd.
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

#ifndef PICDBVUDSSERVER_HPP
#define PICDBVUDSSERVER_HPP

#include "UnixDomainSocketServer.hpp"
#include <map>
#include <memory> // contains std::unique_ptr, requires C++11 support
#include <vector>
#include "../daemon/commands/Command.hpp"

class picdbvUDSServer: public UnixDomainSocketServer
{
  public:
    /** constructor */
    picdbvUDSServer();

  protected:
    /** \brief virtual function that handles the client/server transactions. Has to be
     * implemented in a derived class.
     *
     * \param client_socket_fd contains the file descriptor of the client socket
                               that is connected with the server
     */
    virtual void serveClient(const int client_socket_fd, bool& closeWhenDone);
  private:
    /** \brief auxiliary function to increase command usage statistics for a certain command
     *
     * \param cmdName name of the command
     */
    void increaseCount(const std::string& cmdName);


    /** \brief reorders commands internally to allow faster processing
     */
    void optimizeCommandOrder();


    /** \brief auxiliary function for showing how often commands have been used
     *
     * \param answer string that shall hold the stats/the servers answer
     */
    void showCommandStats(std::string& answer) const;

    std::vector<std::unique_ptr<Command> > m_Commands; /**< vector of recognized commands */
    std::map<std::string, unsigned int> m_CommandCount; /**< counts uses of recognized commands */
}; //class

#endif // PICDBVUDSSERVER_HPP
