/*
 -----------------------------------------------------------------------------
    This file is part of picdb.
    Copyright (C) 2011, 2012, 2015  Thoronador

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

#ifndef PICDATABASE_HPP
#define PICDATABASE_HPP

#include "data/DirectDatabase.hpp"

/** \brief wraps the DirectDatabase class inside of a singleton pattern
 */
class PicDatabase: public DirectDatabase
{
  public:
    /** singleton access */
    static PicDatabase& getSingleton();

    /** destructor */
    virtual ~PicDatabase();
  private:
    ///constructor
    PicDatabase();

    ///empty copy constructor
    PicDatabase(const PicDatabase& op) {}
}; //class

#endif // PICDATABASE_HPP
