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

#ifndef DATABASEMANAGER_HPP
#define DATABASEMANAGER_HPP

#include <map>
#include <string>
#include "DataBase.hpp"

class DatabaseManager
{
  public:
    /** destructor */
    ~DatabaseManager();

    /** \brief singleton access method
     * \return Returns a reference to the singleton instance.
     */
    static DatabaseManager& get();


    /** \brief creates a new database.
     *
     * \param db_name the name of the new database
     * \return Returns true, if the database was created. If a database with
     *         the given name already exists or the name is an empty string,
     *         the function will return false.
     */
    bool createDatabase(const std::string& db_name);

    /** \brief deletes an existing database
     *
     * \param db_name the name of the database
     * \return Returns true, if the database was deleted.
     */
    bool deleteDatabase(const std::string& db_name);


    /** \brief checks, whether a certain database exists
     *
     * \param name the name/alias of the database
     * \return Returns true, if the database exists. Returns false otherwise.
     */
    bool hasDatabase(const std::string& name) const;


    /** \brief returns a list of all currently managed databases
     *
     * \return Returns a set of std::strings that contains the database names.
     */
    std::set<std::string> listAllDatabaseNames() const;


    /** \brief gets a reference to the requested database
     *
     * \param db_name   name of the database
     * \return Returns a reference to the requested database, if it exists.
     *         If the database does not exist, the function will throw an exception.
     */
    DataBase& getDatabase(const std::string& db_name);
  private:
    /** constructor - private due to singleton pattern */
    DatabaseManager();

    /** copy constructor - private and empty due to singleton pattern */
    DatabaseManager(const DatabaseManager& op) {}

    std::map<std::string, DataBase> m_Databases; /**< holds the loaded databases */
}; //class

#endif // DATABASEMANAGER_HPP
