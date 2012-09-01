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
  MinMaxSphereCalibrator.h
  
  This class implements a simple algorithm to quickly calibrate a sensor that measures a 3-D
  constant magnitude vector by tracking the minimum and maximum observations in each component.
  
  It is designed to calibrate magnetometers and accelerometers, or other sensors
  that should take measurements of some constant vector at different orientations.
  
*/

#ifndef __MINMAX_SPHERE_CALIBRATOR_H
#define __MINMAX_SPHERE_CALIBRATOR_H

#include <stddef.h> //for size_t
#include <stdint.h>
#include "Calibrator.h"

class Sensor;


class MinMaxSphereCalibrator : public Calibrator {
  public:
    MinMaxSphereCalibrator(const Sensor* pS) : _pSensor(pS) {
      clearObservationMatrices();
    }
    virtual ~MinMaxSphereCalibrator() {}
    
    //implements the DataListener interface
    virtual void update(const DataCollector* pSubject);
    
    virtual void transform(const int16_t* rawInput, float* output) const;
    virtual void calibrate();
    
    //print the calibration parameters so we can see what is going on inside
    void printBeta() const;
    
  private:
  
   const Sensor* _pSensor;
   
   int16_t _obsMin[3]; // Keep track of min and max observation in each dimension to guess parameters
   int16_t _obsMax[3]; // Keep track of min and max observation in each dimension to guess parameters
    
    
   //Final calibration parameters
   float _beta[6];
   
   void clearObservationMatrices();
   
    
};


#endif // __MINMAX_SPHERE_CALIBRATOR_H
