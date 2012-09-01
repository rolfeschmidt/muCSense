

muCSense is a sensor access library for microcontrollers.  It allows simple access to the ADXL345, HMC5843, and ITG3200 sensors in Arduiono projects.

To use this library in an Arduino sketch, do the following.



(1) Place the source code in the directory 

	<YourArduinoDir>/libraries/muCSense/

	You may have to create the directory "<YourArduinoDir>/libraries/".  

(2) Open your sketch in Arduino 1.0.1 or higher.

(3) On the Menu select Sketch->Import Library...->muCSense.  muCSense will appear under "Contributed Libraries"

Now you can write your code.

This has been designed to make access to Sparkfun's 9dof Sensor Stick as easy as possible.  Here is a simple sketch that uses SensorLib to get raw data from the Sensor Stick:

#include <Wire.h>
#include <ADXL345.h>
#include <HMC5843.h>
#include <I2C.h>
#include <ITG3200.h>
#include <Sensor.h>


ADXL345* pAccel;
HMC5843* pMag;
ITG3200* pGyro;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  pAccel = ADXL345::instance();
  pMag = HMC5843::instance();
  pGyro = ITG3200::instance();
  
  Serial.println("Initializing sensors");
  pAccel->init();
  pMag->init();
  pGyro->init();
  
  Serial.println("Sensors ready");
  
}


void loop() {
  
  pAccel->read();
  pMag->read();
  pGyro->read();
  
  const int16_t* buf = 0;
  
  buf = pAccel->rawReading();
  printInt16Array(buf,3);
  buf = pMag->rawReading();
  printInt16Array(buf,3);
  buf = pGyro->rawReading();
  printInt16Array(buf,3);
  
  Serial.println("_________________________");
  
  delay(1000);
}

void printInt16Array(const int16_t* buf, size_t len) {
  size_t i;
  for(i=0;i<len;++i) {
    Serial.print(buf[i]);
    Serial.print("\t");
  }
  Serial.println();
}

This work copyright 2012 by Rolfe Schmidt, who can be contacted at rolfeschmidt@gmail.com  
Available at https://github.com/rolfeschmidt/muCSense

