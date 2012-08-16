
/*
# copyright 2012 by Rolfe Schmidt
# This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
# To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send a
# letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
#
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
