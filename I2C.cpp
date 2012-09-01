
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

#include <Wire.h>
#include "I2C.h"


void i2c_read(int address, byte reg, int count, byte* data)  {
  int i = 0;
  
  //  Send input register address
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.endTransmission();

  //  Connect to device and request bytes
  Wire.beginTransmission(address);
  Wire.requestFrom(address,count);
  while(Wire.available())    // slave may send less than requested
  { 
    char c = Wire.read();    // receive a byte as character
    data[i] = c;
    i++;
  }

  Wire.endTransmission();
  
}

void i2c_write(int address, byte reg, byte data)  {
  //  Send output register address
  Wire.beginTransmission(address);
  Wire.write(reg);

  //  Connect to device and send byte
  Wire.write(data);  //  low byte

  Wire.endTransmission();
  
}
