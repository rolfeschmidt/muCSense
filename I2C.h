
/*
# copyright 2012 by Rolfe Schmidt
# This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
# To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send a
# letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
#
*/
/*
  I2C.h -- Abstract class to encapsulate sensor
  initialization and access
*/

#ifndef __I2C_H
#define __I2C_H

#include "Arduino.h"

void i2c_read(int address, byte reg, int count, byte* data);
void i2c_write(int address, byte reg, byte data);

#endif //__I2C_H
