
/*
# copyright 2012 by Rolfe Schmidt
# This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
# To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send a
# letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
#
*/
#include "I2C.h"
#include "HMC5843.h"


//First our constant location for calibration data storage in EEPROM
const size_t HMC5843_EEPROM_ADDRESS = (2 << 5);  //We'll give each sensor stick sensor 32 bytes starting at 32.
                                                 //ADXL345 is first, then HMC5843, then ITG3200.  Additional sensors follow

const byte HMC5843_ADDRESS = (0x3C >> 1);
 
 
 //The magnetometer will advance the address pointer through the
 //data registers, so just read this one then grab the next 5 w/o
 //sending a new address
const byte HMC5843_REGISTER_XMSB = (0x03);
const byte HMC5843_REGISTER_MEASMODE = (0x02);
const byte HMC5843_MEASMODE_CONT = (0x00) ;



//The factory method controls access to the single instance of this class.
HMC5843*  HMC5843::instance() {
	static HMC5843* pInstance = 0;

	if(pInstance == 0) {
		pInstance = new HMC5843();
	}

	return pInstance;
}

 
 HMC5843::HMC5843() : _isReady(false) {
 }

 
 void HMC5843::init() {
  byte data = 0;
  i2c_read(HMC5843_ADDRESS,  HMC5843_REGISTER_MEASMODE, 1, &data);
  
  //set up continuous measurement
  i2c_write(HMC5843_ADDRESS, HMC5843_REGISTER_MEASMODE, HMC5843_MEASMODE_CONT);
  
  data = 0;
  i2c_read(HMC5843_ADDRESS, HMC5843_REGISTER_MEASMODE, 1, &data);

  _isReady = true;
 }
 
 
bool HMC5843::read() {
  byte bytes[6];
  memset(bytes,0,6);
  
  //read 6 bytes from the ADXL345
  i2c_read(HMC5843_ADDRESS, HMC5843_REGISTER_XMSB, 6, bytes);
  
  //now unpack the bytes
  for (int i=0;i<3;++i) {
    this->_data[i] = (int16_t)bytes[2*i + 1] + (((int16_t)bytes[2*i]) << 8);
	_calData[i] = (static_cast<float>(_data[i]) - _calibrationParameters[i])/_calibrationParameters[3+i];
  }
  
	return true;
    
 }


 const int16_t* HMC5843::rawReading() const {
	 return this->_data;
 }

 
void HMC5843::setCalibrationParameters(const byte* serializedCalData) {
	memcpy(this->_calibrationParameters, serializedCalData, getCalibrationDataSize());
}


const float* HMC5843::calibratedReading() const {
	return this->_calData;
}


size_t HMC5843::getEEPROMAddress() const { return HMC5843_EEPROM_ADDRESS; }

size_t HMC5843::getCalibrationDataSize() const {
	return 6*sizeof(float);
}


const byte* HMC5843::serialzedCalibrationData() const {
	return reinterpret_cast<const byte*>(_calibrationParameters);
}


