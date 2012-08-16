
/*
# copyright 2012 by Rolfe Schmidt
# This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
# To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send a
# letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
#
*/

#include "Sensor.h"
#include <EEPROM.h>



void writeEEPROM(size_t address, const byte* data, size_t size) {
	for(size_t i = 0; i < size;++i) {
		//EEPROM.write(address, data[i]);
	}
}



void readEEPROM(size_t address, byte* data, size_t size) {
	for(size_t i = 0; i < size;++i) {
		//data[i] = EEPROM.read(address);
	}
}

//This method is a template to call virtual private functions
// that will be implemented by subclasses.
void Sensor::writeCalibrationParametersToEEPROM() const {  //Write calibration params in EEPROM
	
	//write params to EEPROM
	writeEEPROM(getEEPROMAddress(), serialzedCalibrationData(), getCalibrationDataSize());


}

void Sensor::loadCalibrationDataFromEEPROM()  {
	byte* buffer = reinterpret_cast<byte*>(malloc(getCalibrationDataSize()));
	readEEPROM(getEEPROMAddress(), buffer, getCalibrationDataSize());

	this->setCalibrationParameters(buffer);

	free(buffer);
}
