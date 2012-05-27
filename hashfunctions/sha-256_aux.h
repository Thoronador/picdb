/*
 -----------------------------------------------------------------------------
    This file is part of the Thoronador's random stuff.
    Copyright (C) 2012 thoronador

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

#ifndef RANDOM_THORO_SHA_256_AUX_H
#define RANDOM_THORO_SHA_256_AUX_H

#include <stdint.h>

namespace SHA256
{

#if BYTE_ORDER == LITTLE_ENDIAN
// byte order adjustment functions (Shouldn't there be a std header for that?)

/* reverses little endian to big endian

   parameters:
       w - little endian value
       x - var. to store the converted value
*/
void reverse32(const uint32_t w, uint32_t& x);

/* reverses little endian to big endian

   parameters:
       w - little endian value
       x - var. to store the converted value
*/
void reverse64(const uint64_t w, uint64_t& x);
#endif

/* MessageBlock structure */
struct MessageBlock
{
  uint32_t words[16];
};//struct

/* basic message source class */
class MessageSource
{
  public:
    /* constructor */
    MessageSource();

    /* destructor */
    virtual ~MessageSource();

    /* puts the next message block from the source in mBlock and returns true,
       if there is at least one more message block. Returns false and leaves
       mBlock unchanged, if there are no more message blocks.

       parameters:
           mBlock - reference to the message blocked that should be filled
    */
    virtual bool getNextMessageBlock(MessageBlock& mBlock) = 0;
  protected:
    uint64_t m_BitsRead;
    uint8_t * m_PaddingBuffer;
    enum PaddingStatus {psUnpadded, psPadded512, psPadded1024, psPadded1024And512Read, psPaddedAndAllRead};
    PaddingStatus m_Status;
}; //class

/* message source class for buffers */
class BufferSource: public MessageSource
{
  public:
    /* constructor */
    BufferSource(uint8_t* data, const uint64_t data_length_in_bits);

    /* destructor */
    virtual ~BufferSource();

    /* puts the next message block from the source in mBlock and returns true,
       if there is at least one more message block. Returns false and leaves
       mBlock unchanged, if there are no more message blocks.

       parameters:
           mBlock - reference to the message blocked that should be filled
    */
    virtual bool getNextMessageBlock(MessageBlock& mBlock);
  private:
    uint8_t * m_BufferPointer;
    uint64_t m_BufferSize;
}; //class

} //namespace

#endif // RANDOM_THORO_SHA_256_AUX_H
