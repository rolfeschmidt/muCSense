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

#include "SimpleDataCollector.h"
#include <Sensor.h>


SimpleDataCollector::SimpleDataCollector(size_t numSamples, size_t samplingTimeMillis) : _numSamples(numSamples), _samplingTimeMillis(samplingTimeMillis)
{
}

void SimpleDataCollector::collect() {
  
  size_t sampleInterval = _samplingTimeMillis / _numSamples;
  size_t i,j;

  for(i = 0; i < _numSamples; ++i)  {
    for(j=0;j<_sensorArray.size();++j) {
      _sensorArray[j]->read();
    }
    
    for(j=0;j<_sensorArray.size();++j) {
      const int16_t* pReading = _sensorArray[j]->rawReading();
      memcpy(_sensorData[j], pReading, dataSizeForSensor(_sensorArray[j])*sizeof(int16_t));
    }
    
    notifyListeners();
    delay(sampleInterval);
    
  }
}


