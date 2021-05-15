#ifndef _INERTIAL_MEASUREMENT_UNIT_H
#define _INERTIAL_MEASUREMENT_UNIT_H

#include <Wire.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>
#include <DataLogger.h>

class InertialMeasurementUnit
{
  Adafruit_LSM9DS1 lsm;

  sensors_event_t a, m, g, temp;
  double accX, accY, accZ;
  double magX, magY, magZ;
  double gyrX, gyrY, gyrZ;

public:
  InertialMeasurementUnit(DataLogger *logger)
  {
    logger->addVariable(0, VariableLevel::Private, accX);
    logger->addVariable(1, VariableLevel::Private, accY);
    logger->addVariable(2, VariableLevel::Private, accZ);

    logger->addVariable(3, VariableLevel::Private, magX);
    logger->addVariable(4, VariableLevel::Private, magX);
    logger->addVariable(5, VariableLevel::Private, magX);

    logger->addVariable(6, VariableLevel::Private, gyrX);
    logger->addVariable(7, VariableLevel::Private, gyrX);
    logger->addVariable(8, VariableLevel::Private, gyrX);
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
  }
};

#endif
