
/*
# copyright 2012 by Rolfe Schmidt
# This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
# To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send a
# letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
#
*/
/*
  HMC5843.h -- Encapsulate HMC5843 Magnetometer
  initialization and access in a standard way for use by other 
  code and libraries.
*/

#ifndef __HMC5843_STREAM_H
#define __HMC5843_STREAM_H

#include "Arduino.h"
#include "Sensor.h"



class HMC5843 : public Sensor {
public:
	
    virtual ~HMC5843() {}

    //methods that interact with the sensor
	virtual void init();
    virtual bool read();

	//methods that provide access tp ready data
    virtual size_t dim() const { return 3; }
	virtual const int16_t* rawReading() const;
	virtual bool isReady() const { return _isReady; }

	//Calibration functions
	virtual void setCalibrationParameters(const byte* serializedCalData);  
	virtual const float* calibratedReading() const;
    
	
	//Factory method to get the single instance of the HMC5843.
	//
	// Important:  these return UNINITIALIZED sensors.  The user is responsible for initialization.
	// This allows the user to set initialization parameters or wait until the device is needed before 
	// waking it up.
	static HMC5843*  instance();
private:
	
    HMC5843(); //private constructor -- only build this through a factory function

	virtual size_t getEEPROMAddress() const;
	virtual size_t getCalibrationDataSize() const;
	virtual const byte* serialzedCalibrationData() const;


	int16_t _data[3];
	float _calData[3];
    float _calibrationParameters[6];

	bool _isReady;
    
};

#endif //__HMC5843_STREAM_H
