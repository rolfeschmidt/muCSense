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
  BestSphereGaussNewtonCalibrator.h
  
  This class implements a constant-space, one-pass streaming algorithm
  to fit a set if three-dimensional points to an axis-oriented ellipsoid.
  
  It is designed to calibrate magnetometers and accelerometers, or other sensors
  that should take measurements of some constant vector at different orientations.
  
  These objects consume a fair bit of memory for a uC, so it is best not to leave them lying around
  when not in use (e.g. as global variables or memory leaks).
*/

#ifndef __BEST_SPHERE_GN_CALIBRATOR_H
#define __BEST_SPHERE_GN_CALIBRATOR_H

#include <stddef.h> //for size_t
#include <stdint.h>
#include "Calibrator.h"

class Sensor;


class BestSphereGaussNewtonCalibrator : public Calibrator {
  public:
    BestSphereGaussNewtonCalibrator(const Sensor* pS) : _pSensor(pS) {
      clearObservationMatrices();
    }
    virtual ~BestSphereGaussNewtonCalibrator() {}
    
    //implements the DataListener interface
    virtual void update(const DataCollector* pSubject);
    
    virtual void transform(const int16_t* rawInput, float* output) const;
    virtual void calibrate();
    
    //print the calibration parameters so we can see what is going on inside
    void printBeta() const;
  private:
  
   const Sensor* _pSensor;
   
   //observation summary structures
   float _mu[3]; //sum of all observations in each dimension
   float _mu2[3];  //sum of squares of all observations in each dimension
   float _ipXX[6]; //Symmetric matrix of inner products of observations with themselves
   float _ipX2X[3][3]; //matrix of inner products of squares of observations with the observations
   float _ipX2X2[6]; //Symmetric matrix of inner products of squares of observations with themselves
   size_t _N;  //The number of observations
   
   int16_t _obsMin[3]; // Keep track of min and max observation in each dimension to guess parameters
   int16_t _obsMax[3]; // Keep track of min and max observation in each dimension to guess parameters
    
    
   //Final calibration parameters
   float _beta[6];
   
   void clearObservationMatrices();
   void guessParameters();
   void clearGNMatrices(float JtJ[][6], float JtR[]);
   void computeGNMatrices(float JtJ[][6], float JtR[]);
   void findDelta(float JtJ[][6], float JtR[]);
   
    
};


#endif // __BEST_SPHERE_GN_CALIBRATOR_H
