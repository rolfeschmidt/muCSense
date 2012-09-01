
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
  Sensor.h -- Abstract class to encapsulate sensor
  initialization and access
*/

#ifndef __SENSOR_STREAM_H
#define __SENSOR_STREAM_H

#include <stddef.h> //for size_t
#include <stdint.h>
#include "Arduino.h"


class Sensor {
public:
	Sensor()  {}
    virtual ~Sensor() {}

	//Methods that control interaction with the sensor
	virtual void init() = 0;  //prepares the sensor for measurement
    virtual bool read() = 0;  //fetches raw data from the sensor
	
	//Data access methods -- these do not trigger communication with the sensor
    virtual size_t dim() const = 0;
	virtual const int16_t* rawReading() const = 0;
	virtual bool isReady() const = 0;

	//Calibration functions
	virtual void setCalibrationParameters(const byte* serializedCalData) = 0;  
	virtual void writeCalibrationParametersToEEPROM() const;
	virtual void loadCalibrationDataFromEEPROM();
	virtual const float* calibratedReading() const = 0;

private:
	virtual size_t getEEPROMAddress() const = 0;
	virtual size_t getCalibrationDataSize() const = 0;
	virtual const byte* serialzedCalibrationData() const = 0;
	    
};

#endif //__SENSOR_STREAM_H
