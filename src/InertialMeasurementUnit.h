#ifndef _INERTIAL_MEASUREMENT_UNIT_H
#define _INERTIAL_MEASUREMENT_UNIT_H

#include <Wire.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>
#include <DataLogger.h>
#include <AttitudeEstimator.h>
#include "Timer.h"

#define M_PI 3.1415926535897932384626433832795

using namespace stateestimation;

class InertialMeasurementUnit
{
  Adafruit_LSM9DS1 lsm;
  AttitudeEstimator est;
  Timer timer;

  sensors_event_t a, m, g, temp;
  double accX, accY, accZ;
  double magX, magY, magZ;
  double gyrX, gyrY, gyrZ;

public:
  double yaw, pitch, roll;
  
  InertialMeasurementUnit(AdvancedDataLogger *logger)
  {
    // Est.setPIGains(2.2, 2.65, 10, 1.25);
    est.setPIGains(0.5, 2.65, 5, 1.25);

    logger->addDouble("Yaw", "rad", [this]() { return this->yaw; });
    logger->addDouble("Pitch", "rad", [this]() { return this->pitch; });
    logger->addDouble("Roll", "rad", [this]() { return this->roll; });

    // logger->addVariable(3, VariableLevel::Private, accX);
    // logger->addVariable(4, VariableLevel::Private, accY);
    // logger->addVariable(5, VariableLevel::Private, accZ);

    // logger->addVariable(6, VariableLevel::Private, magX);
    // logger->addVariable(7, VariableLevel::Private, magX);
    // logger->addVariable(8, VariableLevel::Private, magX);

    // logger->addVariable(9, VariableLevel::Private, gyrX);
    // logger->addVariable(10, VariableLevel::Private, gyrX);
    // logger->addVariable(11, VariableLevel::Private, gyrX);
  }

  void setup()
  {
    if (!lsm.begin())
    {
      Serial.println("Oops ... unable to initialize the LSM9DS1. Check your wiring!");
    }
    Serial.println("Found LSM9DS1 9DOF.");

    // 1.) Set the accelerometer range
    lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_2G);
    //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_4G);
    //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_8G);
    //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_16G);

    // 2.) Set the magnetometer sensitivity
    lsm.setupMag(lsm.LSM9DS1_MAGGAIN_4GAUSS);
    //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_8GAUSS);
    //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_12GAUSS);
    //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_16GAUSS);

    // 3.) Setup the gyroscope
    lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_245DPS);
    //lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_500DPS);
    //lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_2000DPS);
  }

  void loop()
  {
    lsm.read();
    lsm.getEvent(&a, &m, &g, &temp);

    accX = a.acceleration.x;
    accY = a.acceleration.y;
    accZ = a.acceleration.z;

    magX = m.magnetic.x;
    magY = m.magnetic.y;
    magZ = m.magnetic.z;

    gyrX = g.gyro.x;
    gyrY = g.gyro.y;
    gyrZ = g.gyro.z;

    // est.update(timer.measure(), gyrX, gyrY, gyrZ, accX, accY, accZ, magX, magY, magZ);
    est.update(timer.measure(), gyrX, gyrY, gyrZ, accX, accY, accZ, .0, .0, .0);

    // yaw = est.fusedYaw();
    // pitch = est.eulerPitch();
    // roll = est.fusedRoll();

    roll = (accX / 10.0) * M_PI;
  }
};

#endif
