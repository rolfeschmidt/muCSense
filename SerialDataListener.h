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
  SerialDataListener.h
  
  Instances of SerialDataListener receive updates from a DataCollector
  and print those results to the Serial output stream.
*/

#ifndef __SERIAL_DATA_LISTENER_H
#define __SERIAL_DATA_LISTENER_H

#include "DataListener.h"
#include "DynamicArray.h"

class Sensor;



class SerialDataListener : public DataListener {
public:
  SerialDataListener() : _sensors(4) {}
  void addSensor(const Sensor* pS) {_sensors.push_back(pS);}
  virtual void update(const DataCollector* pSubject);
private:
  DynamicArray<const Sensor*> _sensors;
    
};


#endif // __SERIAL_DATA_LISTENER_H
