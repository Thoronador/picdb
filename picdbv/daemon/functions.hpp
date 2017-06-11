/*
 -------------------------------------------------------------------------------
    This file is part of picdbv.
    Copyright (C) 2015  Dirk Stolle

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

#ifndef PICDBD_FUNCTIONS_HPP
#define PICDBD_FUNCTIONS_HPP

#include <string>

/** \brief extracts the status code from a (picdbd) server response
 *
 * \param response  a string containing the server's response
 * \return returns the status code as an integer value;
 *         returns zero, if @response is not a valid response string.
 *         (That does not interfere with other status codes, because the picdbd
 *          will never return the status code zero.)
 */
int extractStatusCodeFromResponse(const std::string& response);

#endif // PICDBD_FUNCTIONS_HPP
