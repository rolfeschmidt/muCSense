
/*
# copyright 2012 by Rolfe Schmidt
# This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
# To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send a
# letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
#
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
