/*
 -----------------------------------------------------------------------------
    This file is part of the Thoronador's random stuff.
    Copyright (C) 2011 thoronador

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

#ifndef READPNG_H
#define READPNG_H

#include <string>
#include "GLImageStructure.h"
#include <png.h>

/* returns a structure containing the image data of the given PNG file

   parameters:
       FileName - name of the PNG
*/
GLImageStructure readPNG(const std::string& FileName);

/* returns true, if the given file seems to be a PNG file according to its
   first bytes

   parameters:
       header - pointer to an array that contains at least the first eight bytes
                from the file
       length - number of elements in the array - has to be at least eight
*/
bool isPNG(const png_bytep header, const size_t length);

#endif //READPNG_H
