/*
# copyright 2012 by Rolfe Schmidt

	This file is part of SensorLib.

    SensorLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SensorLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SensorLib.  If not, see <http://www.gnu.org/licenses/>.

*/
#include <Wire.h>
#include <ADXL345.h>
#include <HMC5843.h>
#include <ITG3200.h>

//Pointers to implemented sensors that we'll create on the heap.
ADXL345* pAccel;
HMC5843* pMag;
ITG3200* pGyro;
Sensor* sensors[3];
char* names[] = {"ACCEL: ", "MAG: ", "GYRO: "};

void setup() {
  Serial.begin(115200); //Default for BlueSMiRF
  Wire.begin();
  
  //Call factory methods to get single instances of each sensor.
  // To get a sensor at a specific I2C bus address, pass the 
  // address in to the factory method.  pAccel = ADXL345::instance();
  pMag = HMC5843::instance();
  pGyro = ITG3200::instance();
  
  //Put the sensors into the array.  We could have
  //constructed them here from the start, but sometimes
  //it is nice to have access to the derived class pointers.
  sensors[0] = pAccel;
  sensors[1] = pMag;
  sensors[2] = pGyro;
  
  //All of our objects implement the Sensor interface,
  //so we can work with them uniformly. Here we initialize.
  Serial.println("Initializing sensors");
  for(size_t i=0; i < 3 ; ++i) {
    sensors[i]->init();
  }
  
  Serial.println("Sensors ready");
  
}


void loop() {
  
  //Now we read the sensors.  This call actually triggers 
  // I2C communication (or whatever other communication is needed)
  // with the device and stores the readings internally.
  for(size_t i=0; i < 3 ; ++i) {
    sensors[i]->read();
  }
  
  //Now that the sensors have been read, we can ask the sensors what 
  // the readings were.  This DOES NOT trigger communication with the
  // sensor and the rawReading method is const. This allows multiple
  // clients to request the rawReading and they will all get the same
  // result -- a feature that can be very important.
  const int16_t* buf = 0;
  
  for(size_t i=0; i < 3 ; ++i) {
    buf = sensors[i]->rawReading();
    Serial.print(names[i]);
    printInt16Array(buf,sensors[i]->dim());
  }
  
  Serial.println();
  
  
  delay(100);
}

void printInt16Array(const int16_t* buf, size_t len) {
  size_t i;
  for(i=0;i<len;++i) {
    Serial.print(buf[i]);
    Serial.print("\t");
  }
  Serial.println();
}
