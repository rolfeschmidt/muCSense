
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
#include "I2C.h"
#include "ADXL345.h"

//Put the ADXL specific data in the implementation file so we don't
//clutter the global namespace.

//First our constant location for calibration data storage in EEPROM
const size_t ADXL345_EEPROM_ADDRESS = (1 << 5);  //We'll give each sensor stick sensor 32 bytes starting at 32.
                                                 //ADXL345 is first, then HMC5843, then ITG3200.  Additional sensors follow

const size_t ADXL345_EEPROM_ALT_ADDRESS = (4 << 5);  //In case there are 2 sensors attached, we'll have an extra storage slot

 
//We will only  access the first data register on the sensor,
// but we will grab all 6 bytes of data from it.
const byte  ADXL345_REGISTER_XLSB = (0x32);
 
//Need to set power control bit to wake up the adxl345
const byte  ADXL_REGISTER_PWRCTL = (0x2D);
const byte  ADXL_PWRCTL_MEASURE = (1 << 3);

//The factory method controls access to the single instances of this class for each I2C address
ADXL345*  ADXL345::instance(byte i2cAddress) {
	static ADXL345* pDefault = 0;
	static ADXL345* pAlt = 0;
	static ADXL345* pNull = 0;

	ADXL345* pResult;

	if(i2cAddress == ADXL345_ADDRESS) {
		if(!pDefault) {
			pDefault = new ADXL345(i2cAddress);
		}

		pResult = pDefault;

	} else if(i2cAddress == ADXL345_ALT_ADDRESS) {
		if(!pAlt) {
			pAlt = new ADXL345(i2cAddress);
		}

		pResult = pAlt;

	} else {
		pResult = pNull;
	}

	return pResult;
}
 

//Construct a new ADXL345 for a given I2C bus address
 ADXL345::ADXL345(byte i2cAddress) : _i2cAddress(i2cAddress), _isReady(false) {}

 void ADXL345::init() {
  byte data = 0;
  i2c_read(_i2cAddress, 0, 1, &data);

  data = 0;
  i2c_read(_i2cAddress, ADXL_REGISTER_PWRCTL, 1, &data);
  
  i2c_write(_i2cAddress, ADXL_REGISTER_PWRCTL, ADXL_PWRCTL_MEASURE);
  
  data = 0;
  i2c_read(_i2cAddress, ADXL_REGISTER_PWRCTL, 1, &data);

  _isReady = true;
 }
 
 
bool ADXL345::read() {
    byte bytes[6];
    memset(bytes,0,6);
    
    //read 6 bytes from the ADXL345
    i2c_read(_i2cAddress, ADXL345_REGISTER_XLSB, 6, bytes);
    
    //now unpack the bytes
    for (int i=0;i<3;++i) {
      _data[i] = (int16_t)bytes[2*i] + (((int16_t)bytes[2*i + 1]) << 8);
	  _calData[i] = (static_cast<float>(_data[i]) - _calibrationParameters[i])/_calibrationParameters[3+i];
    }
	
	return true;
    
 }

 const int16_t* ADXL345::rawReading() const {
	 return this->_data;
 }

 
void ADXL345::setCalibrationParameters(const byte* serializedCalData) {
	//TO DO: Consider setting parameters directly on the sensor
	memcpy(this->_calibrationParameters, serializedCalData, getCalibrationDataSize());
}

const float* ADXL345::calibratedReading() const {
	return this->_calData;
}

size_t ADXL345::getEEPROMAddress() const { return ADXL345_EEPROM_ADDRESS; }

size_t ADXL345::getCalibrationDataSize() const {
	return 6*sizeof(float);
}

const byte* ADXL345::serialzedCalibrationData() const {
	return reinterpret_cast<const byte*>(_calibrationParameters);
}

