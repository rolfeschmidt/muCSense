#include <Wire.h>
#include <ADXL345.h>
#include <HMC5843.h>
#include <I2C.h>
#include <ITG3200.h>
#include <Sensor.h>

#include <SerialDataListener.h>
#include <SimpleDataCollector.h>
#include <BestSphereGaussNewtonCalibrator.h>
#include <MinMaxSphereCalibrator.h>

//Global objects.  Pointers are to objects that are created and initialized in setup().

//Pointers to sensors on the SparkFun Sensor Stick

Sensor* sensors[3];

//A DataCollector object that will simply callect 100 raw readings over 15 seconds
SimpleDataCollector sdc(100, 15000);

//The accelerometer and magnetometer will each get a Calibrator
Calibrator* pAccelCal;
Calibrator* pMagCal;


void setup() {
  
  Serial.begin(115200);  //115200 is Default for BlueSMiRF
  Wire.begin();          //start I2C
  
  //Create instances of the sensors we'll be using
  sensors[0] = ADXL345::instance();
  sensors[1] = HMC5843::instance();
  sensors[2] = ITG3200::instance();
  
  //Initialize the sensors, making sure they are ready for continuous measurement
  // Once initialized, add each sensor to the DataCollector.
  Serial.println("Initializing sensors");
  size_t i;
  for(i=0;i<3;++i) {
    sensors[i]->init();
    sdc.addSensor(sensors[i]);
  }
  
  Serial.println("Sensors ready");
  
  
  //Now we create some objects that will listen to the data collector.
  
  //The simplest DataListener is a SerialDataListener.  When notified that a DataCollector
  // has data ready, it simply fetches the data and writes it to Serial
  SerialDataListener* pSDL = new SerialDataListener;
  
  //Have the SerialDataListener listen to all of the sensors
  for(i=0;i<3;++i) {
    pSDL->addSensor(sensors[i]);
  }
  
  // Now we create some more sophisticated DataListeners.
  // Here we have two kinds of calibrators: a heavyweight Gauss-Newton calibrator
  // that finds the model parameters that are optimal (in a certain sense, discussed
  // at http://chionophilous.wordpress.com/2011/08/26/accelerometer-calibration-iii-improving-accuracy-with-least-squares-and-the-gauss-newton-method/
  //
  // For the magnetometer, we use a much more lightweight (and less accurate, but still decent) calibrator
  // that uses the minimum and maximum observations in each dimension to estimate offset and sensitivity.
  //
  // Either one of these calibrators could be used on either a magnetometer or accelerometer. The point here
  // is that you are free to mix and match, or write your own Calibrators, as you see fit.
  pAccelCal = new BestSphereGaussNewtonCalibrator(sensors[0]); //the ADXL345
  pMagCal = new MinMaxSphereCalibrator(sensors[1]); //the HMC5843
  
  //Now we need to register each of our DataListeners with the DataCollector. This
  // lets the DataCollector know which listeners to notify when new data is ready.
  sdc.addListener(pSDL);
  sdc.addListener(pMagCal);
  sdc.addListener(pAccelCal);
  
  
  //Now perform the calibration.  You might move this out of setup and likely want
  // some way for the user to trigger it.
  
  //Collect the data.  While this is running you should smoothly rotate your sensor around
  // so that every axis has the chance to be "vertical" both upward and downward.  Try not to shake
  // it too much, it throws off the accelerometer readings.  
  //
  // To calibrate an accelerometer well, a more careful data collection strategy helps, but
  // this one is pretty good for magnetometers.
  sdc.collect();
  
  //Perform calibration on the collected data.  We don't have a gyro calibrator here.
  pMagCal->calibrate();
  pAccelCal->calibrate();
  
  
}


void loop() {
  
  Sensor* pAccel = sensors[0];
  Sensor* pMag = sensors[1];
  
  //read the sensors directly
  for(size_t i=0;i<3;++i) {
    sensors[i]->read();
  }
  
  //Pointer to a buffer that will cointain the raw readings
  const int16_t* buf = 0;
  
  //Read and print the accelerometer data
  buf = pAccel->rawReading();
  printInt16Array(buf,3);
  
  //Now transform the reading using the calibratior.  Accumulate the sum
  // of squares too, so we can check that it is close to 1.
  float calibrated[4];
  calibrated[3] = 0.0;
  pAccelCal->transform(buf, calibrated);
  for(int i = 0; i < 3; ++i) {
    calibrated[3] += calibrated[i]*calibrated[i];
    Serial.print(calibrated[i]);
    Serial.print("\t");
  }
  Serial.println(sqrt(calibrated[3]));
  
  
  //Read and print the magnetometer data
  buf = pMag->rawReading();
  printInt16Array(buf,3);
 
 
  //Now transform the reading using the calibrator.  Accumulate the sum
  // of squares too, so we can check that it is close to 1.
  calibrated[3] = 0.0;
  pMagCal->transform(buf, calibrated);
  for(int i = 0; i < 3; ++i) {
    calibrated[3] += calibrated[i]*calibrated[i];
    Serial.print(calibrated[i]);
    Serial.print("\t");
  }
  Serial.println(sqrt(calibrated[3]));
  
  //buf = pGyro->rawReading();
  //printInt16Array(buf,3);
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

