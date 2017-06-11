/*
 -----------------------------------------------------------------------------
    This file is part of picdbv.
    Copyright (C) 2011, 2014  Dirk Stolle

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

#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <string>

/** \brief structure that holds the two basic configuration values
 */
struct Configuration
{
  /** \brief path to the directory that contains the files */
  std::string Directory;

  ///string containing the path to the database file
  std::string DB_File;

  /** default constructor */
  Configuration();

  /** \brief sets predefined values for all data members.
   * The application should call this once before accessing the data members
   * of this structure.
   */
  void initialiseValues();

  /** \brief Tries to load configuration from the file FileName.
   *  \return Returns true on success, returns false otherwise.
   *  \remarks If it fails, the configuration will be reset to some
   *           predefined values.
   *  \param FileName the file that will be used to load the configuration
   */
  bool loadFromFile(const std::string& FileName);

  /** \brief Tries to save the configuration to the file FileName.
   *
   * \param FileName name of the file that will be used to save the configuration
   *  \return Returns true on success, returns false otherwise.
   */
  bool saveToFile(const std::string& FileName) const;

  /** \brief character that indicates comment lines*/
  static const char cCommentCharacter;
};//struct

#endif //CONFIGURATION_HPP
