
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
  ITG3200.h -- Encapsulate ITG3200 gyro
  initialization and access in a standard way for use by other 
  code and libraries.
*/

#ifndef __ITG3200_STREAM_H
#define __ITG3200_STREAM_H


#include "Arduino.h"
#include "Sensor.h"


//Sparkfun's Sensor Stick pulls pin 9 to LOW, giving this address by default
const byte ITG3200_ADDRESS = (0xD0 >> 1);

//But we may use a standalone sensor at the other address
const byte ITG3200_ALT_ADDRESS = (0xD2 >> 1);

class ITG3200 : public Sensor {
public:
    virtual ~ITG3200() {}

    //methods that interact with the sensor
	virtual void init();
    virtual bool read();

	//methods that provide access to ready data
    virtual size_t dim() const { return 3; }
	virtual const int16_t* rawReading() const;
	virtual bool isReady() const { return _isReady; }

	//Calibration functions
	virtual void setCalibrationParameters(const byte* serializedCalData);  
	virtual const float* calibratedReading() const;
    

	//Factory method to get the single instance of the ITG3200 at an I2C address.  The default address
	// is the "alternate" address which is wired into Sparkfun's "Sensor Stick".  If an incorrect address
	// is provided, the pointer will be NULL!
	//
	// Important:  these return UNINITIALIZED sensors.  The user is responsible for initialization.
	// This allows the user to set initialization parameters or wait until the device is needed before 
	// waking it up.
	static ITG3200* instance(byte i2cAddress = ITG3200_ADDRESS);

	
private:
    ITG3200(byte i2cAddress); //only allow construction by factory method
	
	virtual size_t getEEPROMAddress() const;
	virtual size_t getCalibrationDataSize() const;
	virtual const byte* serialzedCalibrationData() const;


	int16_t _data[3];
	float _calData[3];
    float _calibrationParameters[6];

	byte _i2cAddress;
	bool _isReady;
};

#endif //__ITG3200_STREAM_H
