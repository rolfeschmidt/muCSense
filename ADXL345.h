
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
  ADXL345.h -- Encapsulate ADXL345
  initialization and access in a standard way for use by other 
  code and libraries.
*/

#ifndef __ADXL345_STREAM_H
#define __ADXL345_STREAM_H

#include "Arduino.h"
#include "Sensor.h"


//Then the ADXL345 constants from the datasheet
//The sensor stick grounds the ADXL345 SDO pin, so we use the address
//that the datasheet refers to as the "alternate".  For this
//library, the Sensor stick address will be default.
const byte ADXL345_ADDRESS = (0xA6 >> 1);

//But we may use a standalone sensor at the other address
const byte ADXL345_ALT_ADDRESS = (0x3A >> 1);

class ADXL345 : public Sensor {
  public:
    virtual ~ADXL345() {}
	
	virtual void init();
    virtual bool read();

    virtual size_t dim() const { return 3; }
	virtual const int16_t* rawReading() const;
	virtual bool isReady() const { return _isReady; }

	
	//Calibration functions
	virtual void setCalibrationParameters(const byte* serializedCalData);  
	virtual const float* calibratedReading() const;

	//Factory method to get the single instance of the ADXL345 at an I2C address.  The default address
	// is the "alternate" address which is wired into Sparkfun's "Sensor Stick". If an incorrect address
	// is provided, the pointer will be NULL!
	//
	// Important:  these return UNINITIALIZED sensors.  The user is responsible for initialization.
	// This allows the user to set initialization parameters or wait until the device is needed before 
	// waking it up.
	static ADXL345*  instance(byte i2cAddress = ADXL345_ADDRESS);
    

private:
	ADXL345(byte i2cAddress);  //Private constructor -- only create this through factory function

	virtual size_t getEEPROMAddress() const;
	virtual size_t getCalibrationDataSize() const;
	virtual const byte* serialzedCalibrationData() const;

	int16_t _data[3];
	float _calData[3];
    float _calibrationParameters[6];

	byte _i2cAddress;
	bool _isReady;
};

#endif //__ADXL345_STREAM_H
