/*
 -----------------------------------------------------------------------------
    This file is part of picdbd.
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
 -----------------------------------------------------------------------------
*/

#ifndef FILESYSTEM_FUNCTIONS_HPP
#define FILESYSTEM_FUNCTIONS_HPP

#include <string>

/** \brief checks, whether a file is readable
 *
 * \param fileName name of the file
 * \return Returns true, if the file is readable; returns false, if not.
 */
bool isReadable(const std::string& fileName);


/** \brief checks, whether a file is writable
 *
 * \param fileName name of the file
 * \return Returns true, if the file is writable; returns false, if not.
 */
bool isWritable(const std::string& fileName);


/** \brief checks, whether a file exists
 *
 * \param fileName name of the file
 * \return Returns true, if the file exists; returns false, if not.
 */
bool isExisting(const std::string& fileName);

#endif // FILESYSTEM_FUNCTIONS_HPP
