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

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>

/** \brief basic interface class for derived Command classes
 */
class Command
{
  public:
    /** \brief constructor
     *
     * \param name  the name of the command
     */
    Command(const std::string& name);


    /* virtual destructor */
    virtual ~Command() { }


    /** \brief returns the command's name
     */
    const std::string& getName() const;


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
    virtual bool processMessage(const std::string& message, std::string& answer) const = 0;


    /** \brief gets a short help text for the command
     *
     * The text should not exceed 55 characters.
     * \return Returns a string containing the help text.
     */
    virtual std::string helpText() const = 0;
  private:
    std::string m_Name;
}; //class

#endif // COMMAND_HPP
