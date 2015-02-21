/*
 -----------------------------------------------------------------------------
    This file is part of picdb.
    Copyright (C) 2015  Thoronador

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

#ifndef SINGLETON_HPP
#define SINGLETON_HPP

template<class T>
class Singleton: public T
{
  public:
    static Singleton<T>& get();
    inline static Singleton<T>& getSingleton()
    {
      return get();
    }
  private:
    /* constructor */
    Singleton();

    /* copy constructor */
    Singleton(const T& op) {}
}; //class Singleton


template<class T>
Singleton<T>& Singleton<T>::get()
{
  static Singleton<T> Instance;
  return Instance;
}

template<class T>
Singleton<T>::Singleton()
: T()
{
}

#endif // SINGLETON_HPP
