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
#include "Arduino.h"

#include "DataCollector.h"
#include "SerialDataListener.h"


void SerialDataListener::update(const DataCollector* pSubject) {
  size_t i,j, dataSize;
  const int16_t* pData;
  for(i=0;i<_sensors.size();++i) {
    pData = pSubject->sensorData(_sensors[i]);
    dataSize = pSubject->dataSizeForSensor(_sensors[i]);
    
    for(j=0;j<dataSize;++j) {
      Serial.print(pData[j]);
      Serial.print("\t");
    }
  }
  Serial.println();
  
}
