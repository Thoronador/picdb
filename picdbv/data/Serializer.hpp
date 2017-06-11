/*
 -----------------------------------------------------------------------------
    This file is part of picdb.
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
 -----------------------------------------------------------------------------
*/

#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#include "PicData.hpp"

/** \brief class for serializing and deserializing PicData objects
 */
class Serializer
{
  public:
    /** \brief serializes a PicData object
     *
     * \param data  the PicData that should be serialized
     * \return Returns a string that contains the serialized data.
     */
    static std::string serialize(const PicData& data);


    /** \brief deserializes a PicData object
     *
     * \param serializedData  string containing the serialized data
     * \param data  PicData that will receive the deserialized data
     * \return Returns true, if deserialization was successful.
     *         Returns false otherwise.
     * \remarks If deserialization fails, the contents of data are
     *          undefined.
     */
    static bool unserialize(const std::string& serializedData, PicData& data);
}; //class

#endif // SERIALIZER_HPP
