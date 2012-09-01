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
  Calibrator.h
  
  This abstract class provides an interface to various calibration strategies for sensor data.
  A calibration strategy will use an array of pre-collected sensor data to compute optimal model
  parameters. Once this is done, the Calibrator can ransform raw observations from the stream into
  calibrated observations.
*/

#ifndef __CALIBRATOR_H
#define __CALIBRATOR_H

#include <stddef.h> //for size_t
#include <stdint.h>
#include "DataListener.h"



class Calibrator : public DataListener {
  public:
    Calibrator() {}
    virtual ~Calibrator() {}
    
    //implements the DataListener interface
    virtual void update(const DataCollector* pSubject) = 0;
    
    virtual void transform(const int16_t* rawInput, float* output) const = 0;
    virtual void calibrate() = 0;
  private:
};


#endif // __CALIBRATOR_H
