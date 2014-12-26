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

#ifndef DAEMONIC_HPP
#define DAEMONIC_HPP

#include <string>

class Daemonic
{
  public:
    /** \brief destructor */
    ~Daemonic();

    /** \brief singleton access method
     *
     * \return Returns the Daemonic instance
     */
    static Daemonic& get();

    /** \brief tries to make the calling process a daemon process
     *
     * \param logName ident string that is prepended to every log message (in syslog)
     * \return Returns true, if the process could be turned into a daemon.
     *         Returns false otherwise.
     * \remarks This function might not return at all, if an error occurred, and exit the program instead.
     */
    bool daemonize(const std::string& logName);
  private:
    bool hasOpenSyslog; /**<  tracks whether openlog was called yet */

    /** \brief constructor */
    Daemonic();

    /** \brief empty copy constructor  (empty due to singleton pattern . there can only be one!) */
    Daemonic(const Daemonic& op) {}
}; //class

#endif // DAEMONIC_HPP
