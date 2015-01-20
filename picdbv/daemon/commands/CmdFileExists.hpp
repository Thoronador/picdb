/*
 -------------------------------------------------------------------------------
    This file is part of picdbd.
    Copyright (C) 2015  Thoronador

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

#ifndef CMDFILEEXISTS_HPP
#define CMDFILEEXISTS_HPP

#include "Command.hpp"

/** \brief class for checking existence of a certain file in a database
 */
class CommandFileExists: public Command
{
  public:
    /** \brief constructor */
    CommandFileExists();


    /** \brief tries to process a client's message/request
     *
     * This function checks whether a given message calls for the command that
     * is implemented in this particular command class, and if so, processes
     * that message.
     *
     * \param message  the message that was received from the client
     * \param answer   string that will be used to store the answer, if any
     * \return Returns true, if the message was processed. Returns false otherwise.
     */
    virtual bool processMessage(const std::string& message, std::string& answer) const;
}; //class

#endif // CMDFILEEXISTS_HPP
