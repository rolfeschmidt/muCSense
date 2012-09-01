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
#include "Arduino.h"  //for Serial
#include "MinMaxSphereCalibrator.h"
#include "DataCollector.h"


void MinMaxSphereCalibrator::update(const DataCollector* pSubject) {
  size_t i;
  const int16_t* obs = pSubject->sensorData(_pSensor);
  
  for(i=0; i < 3; ++i) {
    //Keep track of min and max in each dimension
    _obsMin[i] = (obs[i] < _obsMin[i]) ? obs[i] : _obsMin[i];
    _obsMax[i] = (obs[i] > _obsMax[i]) ? obs[i] : _obsMax[i];
    
    
  }
  
  
}


void MinMaxSphereCalibrator::clearObservationMatrices(){
      size_t i,j;
      for(i=0;i<3;++i) {
        _obsMin[i] = 0x7fff; //INT16_MAX;
        _obsMax[i] = (-0x7fff - 1); //INT16_MIN;
      }
}
  
  
  

void MinMaxSphereCalibrator::calibrate() {
    for(int i=0;i<3;++i) {
      _beta[i] = ((double)(_obsMax[i] + _obsMin[i])) / 2.0;
      _beta[3+i] = ((double)(_obsMax[i] - _obsMin[i]))/2.0;
  }
}


void MinMaxSphereCalibrator::transform(const int16_t* rawInput, float* output)  const    {
  for(int i=0;i<3;++i) {
    output[i] = (static_cast<float>(rawInput[i]) - _beta[i])/_beta[3 + i];
  }
}

void MinMaxSphereCalibrator::printBeta() const {
  Serial.print("BETA: ");
  for(size_t i=0;i<6;++i) {
    Serial.print(_beta[i],7);
    Serial.print(" ");
  }
  
}

