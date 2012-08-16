
/*
# copyright 2012 by Rolfe Schmidt
# This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
# To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send a
# letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
#
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
