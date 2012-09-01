/*
# copyright 2012 by Rolfe Schmidt

	This file is part of muCSense.

    muCSense is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    muCSense is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with muCSense.  If not, see <http://www.gnu.org/licenses/>.

*/
#ifndef __DYNAMIC_ARRAY_H
#define __DYNAMIC_ARRAY_H

#include <stdlib.h>

template<typename T>
class DynamicArray {
public:
  DynamicArray(size_t capacity=4) : _size(0), _capacity(capacity), _data(0) {
      
    _data =  reinterpret_cast<T*>(malloc(_capacity*sizeof(T)));
  }
  
    
  ~DynamicArray() {
    //destroy elements
    for(size_t i = 0; i < _size; ++i) {
      _data[i].~T();
    }
    //free memory
    free(_data);
  }
  size_t size() const { return _size; }
  size_t capacity() const { return _capacity; }
  
  void reserve(size_t n) {
    T* newArray = reinterpret_cast<T*>(malloc(n*sizeof(T)));
    
    //move data into new structures
    memcpy(newArray, _data, _capacity*sizeof(T));
    
    //free the old pointers
    free(_data);
    
    //set the member variables
    _data = newArray;
    _capacity = n;
      
  }
  
  const T& operator[](size_t idx) const { return _data[idx%_size]; }
  T& operator[](size_t idx) { return _data[idx%_size]; }
  
  void push_back(const T& x) {
    if(_size >= _capacity) { grow(); }
    _data[_size++] = x;
  }
  
  


private:

  DynamicArray(const DynamicArray& rhs) {}  //forbid copying for now
  DynamicArray& operator=(const DynamicArray& rhs) { //Do something with _data before making public
    this->_size = rhs._size;
    this->_capacity = rhs._capacity;
    this->_data = rhs._data;
    return this;
  } 
  
  void grow() {reserve(_capacity*2);}
  size_t _size;
  size_t _capacity;
  T* _data;
};

#endif // __DYNAMIC_ARRAY_H
