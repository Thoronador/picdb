/*
 -----------------------------------------------------------------------------
    This file is part of picdbv.
    Copyright (C) 2011, 2012  Dirk Stolle

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

#ifndef WHOSTATRECORD_HPP
#define WHOSTATRECORD_HPP

#include <string>

struct WhoStatRec
{
  std::string who;
  unsigned int count;

  /** constructor */
  WhoStatRec();
};//struct

bool wsr_compare(const WhoStatRec& a, const WhoStatRec& b);

#endif // WHOSTATRECORD_HPP
