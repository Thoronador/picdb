/*
 -----------------------------------------------------------------------------
    This file is part of picdb.
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

#ifndef ESCAPING_HPP
#define ESCAPING_HPP

#include <string>

/** \brief escapes a string, i.e. replaces all NUL characters by backslash 0
 *
 * \param original the original string
 * \return Returns the escaped string.
 * \remarks To get the original string from an escaped string, use unescape().
 */
std::string escape(const std::string& original);


/** \brief escapes a string in place, i.e. replaces all NUL characters by backslash 0
 *
 * \param message   the original string
 * \remarks This function is slightly faster than escape(),
 *          but does not preserve the passed string.
 */
void escapeInPlace(std::string& message);


/** \brief unescapes a string, i.e. restores the original string
 *
 * \param escaped   an escaped string
 * \return Returns the unescaped (i.e.: original) string
 * \remarks Only useful, if @escaped ist a result of the escape() function.
 */
std::string unescape(const std::string& escaped);


/** \brief unescapes a string in place, i.e. restores the original string
 *
 * \param message   the escaped string
 * \remarks This function is slightly faster than unescape(),
 *          but does not preserve the passed string.
 */
void unescapeInPlace(std::string& message);

#endif // ESCAPING_HPP
