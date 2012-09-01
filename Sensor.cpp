
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
