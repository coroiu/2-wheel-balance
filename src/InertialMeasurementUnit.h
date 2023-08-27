#ifndef _INERTIAL_MEASUREMENT_UNIT_H
#define _INERTIAL_MEASUREMENT_UNIT_H

#include <Wire.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>
#include <DataLogger.h>
#include <AttitudeEstimator.h>
#include "Timer.h"

#define M_PI 3.1415926535897932384626433832795
#define DEG_TO_RAD (M_PI / 180.0)

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
  double loopTime = .0;

public:
  double yaw, pitch, roll;
  
  InertialMeasurementUnit(MainAdvancedDataLogger *logger)
  {
    // Est.setPIGains(2.2, 2.65, 10, 1.25);
    est.setPIGains(0.5, 2.65, 5, 1.25);

    auto rawAccelerometerLogger = logger->createLogger("Raw Accelerometer", "accelerometer");
    rawAccelerometerLogger->addDouble("X", "m/s^2", [this]() { return this->accX; });
    rawAccelerometerLogger->addDouble("Y", "m/s^2", [this]() { return this->accY; });
    rawAccelerometerLogger->addDouble("Z", "m/s^2", [this]() { return this->accZ; });

    auto rawGyroscopeLogger = logger->createLogger("Raw Gyroscope", "gyro");
    rawGyroscopeLogger->addDouble("X", "rad/s", [this]() { return this->gyrX; });
    rawGyroscopeLogger->addDouble("Y", "rad/s", [this]() { return this->gyrY; });
    rawGyroscopeLogger->addDouble("Z", "rad/s", [this]() { return this->gyrZ; });

    auto attitudeLogger = logger->createLogger("Est Attitude", "gyro");
    attitudeLogger->addDouble("Roll", "deg", "roll", [this]() { return this->roll; });
    attitudeLogger->addDouble("Pitch", "deg", "pitch", [this]() { return this->pitch; });
    attitudeLogger->addDouble("Yaw", "deg", "yaw", [this]() { return this->yaw; });

    auto imuLogger = logger->createLogger("IMU");
    imuLogger->addDouble("Roll", "rad", [this]() { return this->roll; });
    imuLogger->addDouble("Timing", "ms", [this]() { return this->loopTime*1000.0; });
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
    loopTime = timer.measureSeconds();

    lsm.read();
    lsm.getEvent(&a, &m, &g, &temp);

    accX = a.acceleration.x;
    accY = a.acceleration.y;
    accZ = a.acceleration.z;

    magX = m.magnetic.x;
    magY = m.magnetic.y;
    magZ = m.magnetic.z;

    gyrX = g.gyro.x * DEG_TO_RAD;
    gyrY = g.gyro.y * DEG_TO_RAD;
    gyrZ = g.gyro.z * DEG_TO_RAD;

    // est.update(timer.measure(), gyrX, gyrY, gyrZ, accX, accY, accZ, magX, magY, magZ);

    // est.update(timer.measureSeconds(), .0, .0, .0, accX, accY, accZ, .0, .0, .0);
    // yaw = est.fusedYaw();
    // pitch = est.fusedPitch();
    // roll = est.fusedRoll();

    // est.update(timer.measureSeconds(), gyrX, gyrY, gyrZ, accX, accY, accZ, .0, .0, .0);
    // yaw = est.eulerYaw();
    // pitch = est.eulerPitch();
    // roll = est.eulerRoll();

    // One-axle acc-based roll
    // roll = (accX / 10.0) * M_PI;

    // Three-axle acc-based roll and pitch
    // roll = atan2(accX, sqrt( pow(accY, 2) + pow(accZ, 2))) * 180 / M_PI;
    // pitch = atan2(accY, accZ) * 180 / M_PI;

    // Three-axle acc-based roll and pitch - filtered
    double newRoll = atan2(accX, sqrt( pow(accY, 2) + pow(accZ, 2))) * 180 / M_PI;
    double newPitch = atan2(accY, accZ) * 180 / M_PI;
    roll = roll + (newRoll - roll) * 0.10;
  }
};

#endif
