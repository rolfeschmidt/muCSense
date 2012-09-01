
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
