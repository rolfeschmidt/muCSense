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
#include "DataCollector.h"
#include "Sensor.h"
#include "Calibrator.h"


DataCollector::DataCollector(size_t obsPerSensor) : _numObservationsPerSensor(obsPerSensor),
                                _sensorArray(4), _sensorData(4), _listenerArray(4)
{
}

ptrdiff_t DataCollector::sensorIndex(const Sensor* pS) const {
  ptrdiff_t result = 0;
  for(result = 0; result < _sensorArray.size(); ++result) {
    if(_sensorArray[result] == pS) {
      break;
    }
  }
  
  return result < _sensorArray.size() ? result : -1;
}

void DataCollector::addSensor(Sensor* pS) {
  
  _sensorArray.push_back(pS);
  
  //allocate data storage.  These must be freed in the dtor.
  _sensorData.push_back(reinterpret_cast<int16_t*>(malloc(dataSizeForSensor(pS)*sizeof(int16_t))));
}


void DataCollector::addListener(DataListener* pL) {
  
  _listenerArray.push_back(pL);
  
}


const int16_t* DataCollector::sensorData(const Sensor* pS) const {
  return _sensorData[sensorIndex(pS)];
}

int16_t* DataCollector::sensorData(const Sensor* pS)  {
  return _sensorData[sensorIndex(pS)];
}

size_t DataCollector::dataSizeForSensor(const Sensor* pS) const {
  return _numObservationsPerSensor*pS->dim();
}


void DataCollector::notifyListeners() {
  ptrdiff_t i;
  for(i=0; i<_listenerArray.size(); ++i) {
    _listenerArray[i]->update(this);
  }
}
  

