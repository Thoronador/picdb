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

#include "DatabaseManager.hpp"
#include <stdexcept>

DatabaseManager::DatabaseManager()
: m_Databases(std::map<std::string, DirectDatabase>())
{
}

DatabaseManager::~DatabaseManager()
{
  m_Databases.clear();
}

DatabaseManager& DatabaseManager::get()
{
  static DatabaseManager Instance;
  return Instance;
}

bool DatabaseManager::createDatabase(const std::string& db_name)
{
  if (db_name.empty())
    return false;
  const std::map<std::string, DirectDatabase>::const_iterator iter = m_Databases.find(db_name);
  if (iter != m_Databases.end())
  {
    //database already exists
    return false;
  }
  m_Databases[db_name] = DirectDatabase();
  return true;
}

bool DatabaseManager::deleteDatabase(const std::string& db_name)
{
  if (db_name.empty())
    return false;
  return (m_Databases.erase(db_name) > 0);
}

bool DatabaseManager::hasDatabase(const std::string& name) const
{
  return (m_Databases.find(name) != m_Databases.end());
}

std::set<std::string> DatabaseManager::listAllDatabaseNames() const
{
  std::set<std::string> result;
  std::map<std::string, DirectDatabase>::const_iterator iter = m_Databases.begin();
  while (iter!= m_Databases.end())
  {
    result.insert(iter->first);
    ++iter;
  } //while
  return result;
}

Database& DatabaseManager::getDatabase(const std::string& db_name)
{
  const std::map<std::string, DirectDatabase>::iterator iter = m_Databases.find(db_name);
  if (iter!=m_Databases.end())
  {
    return iter->second;
  }
  throw std::runtime_error("Could not find database \""+db_name+"\"!");
}
