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
/*
  DataCollector.h
  
  This abstract class provides an interface to various data collection strategies
  for sensor data.  Intended to drive collection of samples that will be used for calibration.
*/

#ifndef __DATA_COLLECTOR_H
#define __DATA_COLLECTOR_H

#include <stddef.h> //for size_t
#include <stdint.h>

#include "DynamicArray.h"

class Sensor;
class DataListener;

class DataCollector {
public:
    DataCollector(size_t obsPerSensor = 1);
    virtual ~DataCollector() {}
    
    void addSensor(Sensor* pS);
    void addListener(DataListener* pL);
    
    const int16_t* sensorData(const Sensor* pS) const;
    size_t dataSizeForSensor(const Sensor* pS) const;
    
    //The key method of the class: collect data from the sensors and 
    //notify the listeners as it becomes ready.
    virtual void collect() = 0;

protected:
    void notifyListeners();
    int16_t* sensorData(const Sensor* pS);
    
    ptrdiff_t sensorIndex(const Sensor* pS) const;
    size_t _numObservationsPerSensor;
    DynamicArray<Sensor*> _sensorArray;
    DynamicArray<int16_t*> _sensorData;
    
private:
    
    DynamicArray<DataListener*> _listenerArray;
    
    
};


#endif // __DATA_COLLECTOR_H
