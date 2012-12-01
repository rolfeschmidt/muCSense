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
#include <string.h> //for memset
#include <math.h>
#include "Arduino.h"  //for Serial

#include "DataCollector.h"
#include "BestSphereGaussNewtonCalibrator.h"

size_t upperTriangularIndex(size_t i, size_t j);

void BestSphereGaussNewtonCalibrator::update(const DataCollector* pSubject) {
  size_t i,j, dataSize;
  const int16_t* obs = pSubject->sensorData(_pSensor);
  
  //increment sample count
  ++_N;
  
  int32_t squareObs[3];
  for(i=0; i < 3; ++i) {
    float curObs = obs[i];  //implicitly convert to bigger int
    squareObs[i] = curObs*curObs;  //square of 16-bit int will fit in 32-bit int
  }
    
 
  for(i=0; i < 3; ++i) {
    //Keep track of min and max in each dimension
    _obsMin[i] = (obs[i] < _obsMin[i]) ? obs[i] : _obsMin[i];
    _obsMax[i] = (obs[i] > _obsMax[i]) ? obs[i] : _obsMax[i];
    
    //accumulate sum and sum of squares in each dimension
    _mu[i] += obs[i];
    _mu2[i] += squareObs[i];
    
    //accumulate inner products of the vector of observations and the vector of squared observations.
    for(j=0;j<3;++j) {
      _ipX2X[i][j] += squareObs[i]*static_cast<int32_t>(obs[j]);
      if(i <= j) {
        size_t idx = upperTriangularIndex(i,j);
        _ipXX[idx] += static_cast<int32_t>(obs[i])*static_cast<int32_t>(obs[j]);
        _ipX2X2[idx] += squareObs[i]*static_cast<float>(squareObs[j]);
      }
    }
  }
  
  
}


void BestSphereGaussNewtonCalibrator::clearObservationMatrices(){
      _N = 0;
      memset(_mu, 0, 3*sizeof(float));
      memset(_mu2, 0, 3*sizeof(float));
      memset(_ipXX, 0, 6*sizeof(float));
      memset(_ipX2X2, 0, 6*sizeof(float));
      
      memset(_obsMin, 0, 3*sizeof(int16_t));
      memset(_obsMax, 0, 3*sizeof(int16_t));
      
      size_t i,j;
      for(i=0;i<3;++i) {
        _obsMin[i] = 0x7fff; //INT16_MAX;
        _obsMax[i] = (-0x7fff - 1); //INT16_MIN;
        for(j=0;j<3;++j) {
          _ipX2X[i][j] = 0.0;
        }
      }
}
  

  
void BestSphereGaussNewtonCalibrator::clearGNMatrices(float JtJ[][6], float JtR[]){
  size_t j,k;
    for(j=0;j<6;++j) {
       JtR[j] = 0.0;
       for(k=0;k<6;++k) {
         JtJ[j][k] = 0.0;
       }
    }
      
}

void BestSphereGaussNewtonCalibrator::transform(const int16_t* rawInput, float* output)  const    {
  for(int i=0;i<3;++i) {
    output[i] = (static_cast<float>(rawInput[i]) - _beta[i])/_beta[3 + i];
  }
}

void BestSphereGaussNewtonCalibrator::computeGNMatrices(float JtJ[][6], float JtR[]) {
  size_t i,j;
  
  float beta2[6]; //precompute the squares of the model parameters
  for(i=0;i<6;++i) {
    beta2[i] = pow(_beta[i],2);
  }
  
  //compute the inner product of the vector of residuals with the constant 1 vector, the vector of 
  // observations, and the vector of squared observations.
  float r = _N; //sum of residuals
  float rx[3];  //Inner product of vector of residuals with each observation vector
  float rx2[3];  //Inner product of vector of residuals with each square observation vector
  
  
  //now correct the r statistics
  for(i=0;i<3;++i) {
    r -= (beta2[i]*_N + _mu2[i] - 2*_beta[i]*_mu[i])/beta2[3+i];
    rx[i] = _mu[i];
    rx2[i] = _mu2[i];
    for(j=0;j<3;++j) {
      rx[i] -= (beta2[j]*_mu[i] + _ipX2X[j][i] - 2*_ipXX[upperTriangularIndex(i,j)]*_beta[j])/beta2[3+j];
      rx2[i] -= (beta2[j]*_mu2[i] + _ipX2X2[upperTriangularIndex(i,j)] - 2*_ipX2X[i][j]*_beta[j])/beta2[3+j];
    }
  }
  
  for(i=0;i<3;++i) {
    //Compute product of Jacobian matrix with the residual vector
    JtR[i] = 2*(rx[i] - _beta[i]*r)/beta2[3+i];
    JtR[3+i] = 2*(rx2[i] - 2*_beta[i]*rx[i] + beta2[i]*r)/(beta2[3+i]*_beta[3+i]);
    
    //Now compute the product of the transpose of the jacobian with itself
    //Start with the diagonal blocks
    for(j=i;j<3;++j) {
      JtJ[i][j] = JtJ[j][i] = 4*(_ipXX[upperTriangularIndex(i,j)] - _beta[i]*_mu[j] - _beta[j]*_mu[i] + _beta[i]*_beta[j]*_N)/(beta2[3+i]*beta2[3+j]);
      JtJ[3+i][3+j] = JtJ[3+j][3+i]
                =  4*(_ipX2X2[upperTriangularIndex(i,j)] - 2*_beta[j]*_ipX2X[i][j] + beta2[j]*_mu2[i]
                       -2*_beta[i]*_ipX2X[j][i] + 4*_beta[i]*_beta[j]*_ipXX[upperTriangularIndex(i,j)] - 2*_beta[i]*beta2[j]*_mu[i]
                       +beta2[i]*_mu2[j] - 2*beta2[i]*_beta[j]*_mu[j] + beta2[i]*beta2[j]*_N)/pow(_beta[3+i]*_beta[3+j], 3);
    }
    //then get the off diagonal blocks
    for(j=0;j<3;++j) {
      JtJ[i][3+j] = JtJ[3+j][i]
          = 4*(_ipX2X[j][i] - 2*_beta[j]*_ipXX[upperTriangularIndex(i,j)] + beta2[j]*_mu[i]
                -_beta[i]*_mu2[j] + 2*_beta[i]*_beta[j]*_mu[j] - _beta[i]*beta2[j]*_N)/(beta2[3+i]*beta2[3+j]*_beta[3+j]);
    }
  }
}


void BestSphereGaussNewtonCalibrator::findDelta(float JtJ[][6], float JtR[]){
  //Solve 6-d matrix equation JtJS*x = JtR
  //first put in upper triangular form
  //Serial.println("find delta");
  int i,j,k;
  double lambda;
  
  //make upper triangular
  for(i=0;i<6;++i) {
    //eliminate all nonzero entries below JS[i][i]
    
    if( JtJ[i][i] == 0.0) {
      Serial.print("Diagonal entry ");
      Serial.print(i);
      Serial.print(" is zero!\n");
    }
    
    for(j=i+1;j<6;++j) {
      lambda = JtJ[j][i]/JtJ[i][i];
      if(lambda != 0.0) {
        JtR[j] -= lambda*JtR[i];
        for(k=i;k<6;++k) {
          JtJ[j][k] -= lambda*JtJ[i][k];
        } 
      }
    }
  }
 
 
  //back-substitute
  for(i=5;i>=0;--i) {
    JtR[i] /= JtJ[i][i];
    JtJ[i][i] = 1.0;
    for(j=0;j<i;++j) {
      lambda = JtJ[j][i];
      JtR[j] -= lambda*JtR[i];
      JtJ[j][i] = 0.0;
    }
  }
  
}


void BestSphereGaussNewtonCalibrator::calibrate() {
  Serial.println("calibrate");
  guessParameters();
  float JtJ[6][6];
  float JtR[6];
  clearGNMatrices(JtJ,JtR);
  //print beta

  int i;
  for(i=0;i<6;++i) {
    Serial.print(_beta[i],7);
    Serial.print(" ");
  }
  
  float eps = 0.000000001;
  int num_iterations = 20;
  float change = 100.0;
  while (--num_iterations >=0 && change > eps) {
    
    computeGNMatrices(JtJ, JtR);
    findDelta(JtJ, JtR);
    
    change = JtR[0]*JtR[0] + JtR[1]*JtR[1] + JtR[2]*JtR[2] + JtR[3]*JtR[3]*(_beta[3]*_beta[3]) + JtR[4]*JtR[4]*(_beta[4]*_beta[4]) + JtR[5]*JtR[5]*(_beta[5]*_beta[5]); 
    
    if (!isnan(change)) {
      for(i=0;i<6;++i) {
        _beta[i] -= JtR[i];
      }
      
      if(i >=3) { _beta[i] = fabs(_beta[i]); }
    }
    
    clearGNMatrices(JtJ,JtR);
    
  }
  
}


void BestSphereGaussNewtonCalibrator::printBeta() const {
  Serial.print("BETA: ");
  for(size_t i=0;i<6;++i) {
    Serial.print(_beta[i],7);
    Serial.print(" ");
  }
  
}

size_t upperTriangularIndex(size_t i, size_t j) {
  if (i > j) {
    size_t temp = i;
    i = j;
    j = temp;
  }
  
  return (j*(j+1))/2 + i;
}

void  BestSphereGaussNewtonCalibrator::guessParameters() {
    for(int i=0;i<3;++i) {
      _beta[i] = ((double)(_obsMax[i] + _obsMin[i])) / 2.0;
      _beta[3+i] = ((double)(_obsMax[i] - _obsMin[i]))/2.0;
  }
}
/*

void BestSphereGaussNewtonCalibrator::printMatrices() const {
  
    int j,k;
    Serial.print("[");
    for(j=0;j<6;++j) {
       for(k=0;k<6;++k) {
         Serial.print(_JtJ[j][k],7);
         Serial.print (" ");
       }
       Serial.println(";");
    }
    Serial.println("]");
    
    
    Serial.print("[");
    for(j=0;j<6;++j) {
       Serial.print(_JtR[j],7);
       Serial.println(";");
    }
    Serial.println("]");
}


void BestSphereGaussNewtonCalibrator::printStatMatrices() const {
  
    int j,k;
    
    Serial.print("_obsMin: ");
    for(j=0;j<3;++j) {
       Serial.print(_obsMin[j]);
       Serial.print("\t");
    }
    Serial.println();
    
    
    Serial.print("_obsMax: ");
    for(j=0;j<3;++j) {
       Serial.print(_obsMax[j]);
       Serial.print("\t");
    }
    Serial.println();
    
    Serial.print("_mu: ");
    for(j=0;j<3;++j) {
       Serial.print(_mu[j]);
       Serial.print("\t");
    }
    Serial.println();
    
    Serial.print("_mu2: ");
    for(j=0;j<3;++j) {
       Serial.print(_mu2[j]);
       Serial.print("\t");
    }
    Serial.println();
    
    Serial.print("_ipXX: ");
    for(j=0;j<6;++j) {
       Serial.print(_ipXX[j]);
       Serial.print("\t");
    }
    Serial.println();
    
    Serial.print("_ipX2X2: ");
    for(j=0;j<6;++j) {
       Serial.print(_ipX2X2[j]);
       Serial.print("\t");
    }
    Serial.println();
    
    
    
    Serial.println("_ipX2X: ");
    for(j=0;j<3;++j) {
      for(k=0;k<3;++k) {
         Serial.print(_ipX2X[j][k]);
         Serial.print("\t");
      }Serial.println();
    }
    Serial.println();
    
    
}
*/
