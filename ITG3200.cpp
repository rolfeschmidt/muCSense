
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
#include "ITG3200.h"

 
//First our constant location for calibration data storage in EEPROM
const size_t ITG3200_EEPROM_ADDRESS = (3 << 5);  //We'll give each sensor stick sensor 32 bytes starting at 32.
                                                 //ADXL345 is first, then HMC5843, then ITG3200.  Additional sensors follow

const size_t ADXL345_EEPROM_ALT_ADDRESS = (5 << 5);  //In case there are 2 sensors attached, we'll have an extra storage slot

 
 //request burst of 6 bytes from this address
 const byte ITG3200_REGISTER_XMSB = (0x1D);
 const byte ITG3200_REGISTER_DLPF_FS = (0x16);
 const byte ITG3200_FULLSCALE = (0x03 << 3);
 const byte ITG3200_42HZ = (0x03);

 
//The factory method controls access to the single instances of this class for each I2C address
ITG3200*  ITG3200::instance(byte i2cAddress) {
	
	static ITG3200* pDefault = 0;
	static ITG3200* pAlt = 0;
	static ITG3200* pNull = 0;

	ITG3200* pResult;

	if(i2cAddress == ITG3200_ADDRESS) {
		if(!pDefault) {
			pDefault = new ITG3200(i2cAddress);
		}

		pResult = pDefault;

	} else if(i2cAddress == ITG3200_ALT_ADDRESS) {
		if(!pAlt) {
			pAlt = new ITG3200(i2cAddress);
		}

		pResult = pAlt;

	} else {
		pResult = pNull;
	}

	return pResult;
}
 
 
ITG3200::ITG3200(byte i2cAddress) : _i2cAddress(i2cAddress), _isReady(false) {}
 
 void ITG3200::init() {
  byte data = 0;
  i2c_read(_i2cAddress,  ITG3200_REGISTER_DLPF_FS, 1, &data);
  
  //set up continuous measurement
  i2c_write(_i2cAddress, ITG3200_REGISTER_DLPF_FS, ITG3200_FULLSCALE | ITG3200_42HZ);
  
  data = 0;
  i2c_read(_i2cAddress, ITG3200_REGISTER_DLPF_FS, 1, &data);

  _isReady = true;
 }
 
 
bool ITG3200::read() {
  byte bytes[6];
  memset(bytes,0,6);
  
  //read 6 bytes from the ADXL345
  i2c_read(_i2cAddress, ITG3200_REGISTER_XMSB, 6, bytes);
  
  //now unpack the bytes
  for (int i=0;i<3;++i) {
    this->_data[i] = (int16_t)bytes[2*i + 1] + (((int16_t)bytes[2*i]) << 8);
	//Don't know what gyro calibration looks like yet. Guessing it is Ax + B
	_calData[i] = (static_cast<float>(_data[i]) - _calibrationParameters[i])/_calibrationParameters[3+i];
  }
  
	return true;
 }


 const int16_t* ITG3200::rawReading() const {
	 return this->_data;
 }

 
 
void ITG3200::setCalibrationParameters(const byte* serializedCalData) {
	memcpy(this->_calibrationParameters, serializedCalData, getCalibrationDataSize());
}


const float* ITG3200::calibratedReading() const {
	return this->_calData;
}


size_t ITG3200::getEEPROMAddress() const { return ITG3200_EEPROM_ADDRESS; }

size_t ITG3200::getCalibrationDataSize() const {
	return 6*sizeof(float);
}



const byte* ITG3200::serialzedCalibrationData() const {
	return reinterpret_cast<const byte*>(_calibrationParameters);
}



