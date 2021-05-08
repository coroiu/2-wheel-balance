#ifndef _WHEEL_H
#define _WHEEL_H

#include <FunctionalInterrupt.h>
#include <Ticker.h>
#include <DataLogger.h>
#include <Speed.h>

class Wheel
{
  enum Direction
  {
    Forward = 1,
    Reverse = -1,
  };

  Ticker resetSpeedTicker = Ticker([this]() { resetSpeed(); }, WHEEL_SPEED_RESET_INTERVAL_MICROS, 0, MICROS_MICROS);

  Speed speedAR;
  Speed speedAF;
  Speed speedBR;
  Speed speedBF;
  Speed *speed = &speedAR;
  int direction = Direction::Forward;
  int directionReversal = 1; // 1 = no reversal, -1 = direction reversed
  int sensorA = LOW;
  int sensorB = LOW;
  DataLogger *logger;
  int wheelId;
  int pinA;
  int pinB;

public:
  Wheel(DataLogger *logger, int wheelId, int pinA, int pinB) : logger(logger), wheelId(wheelId), pinA(pinA), pinB(pinB)
  {
    logger->addCalculatedDouble(wheelId, VariableLevel::Public, [this]() { return getSpeed(); });
    // logger->addVariable(wheelId + 1, VariableLevel::Private, sensorA);
    // logger->addVariable(wheelId + 2, VariableLevel::Private, sensorB);
  }

  void setup()
  {
    pinMode(pinA, INPUT);
    pinMode(pinB, INPUT);
    attachInterrupt(
        pinA, [this]() { onChange(); }, CHANGE);
    attachInterrupt(
        pinB, [this]() { onChange(); }, CHANGE);
    resetSpeedTicker.start();
  }

  double getSpeed()
  {
    return speed->getSpeed() * (double)(direction * directionReversal);
  }

  void loop()
  {
    resetSpeedTicker.update();
  }

  void resetSpeed()
  {
    speedAR.reset();
    speedAF.reset();
    speedBR.reset();
    speedBF.reset();
  }

private:
  void IRAM_ATTR onChange()
  // void onChange()
  {
    resetSpeedTicker.start();
    int previousSensorA = sensorA;
    int previousSensorB = sensorA;
    sensorA = digitalRead(pinA);
    sensorB = digitalRead(pinB);

    if (
        previousSensorA == LOW && previousSensorB == LOW && sensorA == HIGH && sensorB == LOW ||
        previousSensorA == HIGH && previousSensorB == LOW && sensorA == HIGH && sensorB == HIGH ||
        previousSensorA == HIGH && previousSensorB == HIGH && sensorA == LOW && sensorB == HIGH ||
        previousSensorA == LOW && previousSensorB == HIGH && sensorA == LOW && sensorB == LOW)
    {
      if (direction == Direction::Reverse)
        resetSpeed();
      direction = Direction::Forward;
    }
    else if (
        previousSensorA == LOW && previousSensorB == LOW && sensorA == LOW && sensorB == HIGH ||
        previousSensorA == LOW && previousSensorB == HIGH && sensorA == HIGH && sensorB == HIGH ||
        previousSensorA == HIGH && previousSensorB == HIGH && sensorA == HIGH && sensorB == LOW ||
        previousSensorA == HIGH && previousSensorB == LOW && sensorA == LOW && sensorB == LOW)
    {
      if (direction == Direction::Forward)
        resetSpeed();
      direction = Direction::Reverse;
    }

    if (previousSensorA == LOW && sensorA == HIGH)
    {
      speed = &speedAR;
      speed->registerEvent();
    }
    // else if (previousSensorA == HIGH && sensorA == LOW)
    // {
    //   speed = &speedAF;
    // }
    else if (previousSensorB == LOW && sensorB == HIGH)
    {
      speed = &speedBR;
      speed->registerEvent();
    }
    // else if (previousSensorB == HIGH && sensorB == LOW)
    // {
    //   speed = &speedBF;
    // }
  }
};

#endif
