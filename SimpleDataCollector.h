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
  SimpleDataCollector.h
  
  This data collector records raw sensor data into an array at a sample rate
  and for a time period specified by the client.  For example, a user could run this for
  10 seconds sampling at 10 Hz while spinning a magnetometer around randomly to collect 
  100 samples in a variety of directions.  
  
  This could be problematic for accelerometer calibrations because it is difficult to move 
  the accelerometer without accelerating it and adding a false signal to the readings.
*/

#ifndef __SIMPLE_DATA_COLLECTOR_H
#define __SIMPLE_DATA_COLLECTOR_H

#include "DataCollector.h"

class SimpleDataCollector : public DataCollector {
  public:
    SimpleDataCollector(size_t numSamples, size_t samplingTimeMillis);
    virtual ~SimpleDataCollector() {}
    
    virtual void collect();
    
    
  private:
    size_t _numSamples;
    size_t _samplingTimeMillis;
};


#endif // __SIMPLE_DATA_COLLECTOR_H
