#ifndef _WHEEL_H
#define _WHEEL_H

#include <FunctionalInterrupt.h>
#include <Ticker.h>
#include <DataLogger.h>

class Wheel
{
  enum Direction
  {
    Forward = 1,
    Reverse = -1,
  };

  Ticker stateSpeedTicker = Ticker([this]() { resetSpeed(); }, WHEEL_SPEED_RESET_INTERVAL_MICROS, 0, MICROS_MICROS);

  int direction = Direction::Forward;
  int directionReversal = 0; // 1 = no reversal, -1 = direction reversed
  int sensorA = LOW;
  int sensorB = LOW;
  DataLogger *logger;
  int wheelId;
  int pinA;
  int pinB;

public:
  Wheel(DataLogger *logger, int wheelId, int pinA, int pinB) : logger(logger), wheelId(wheelId), pinA(pinA), pinB(pinB)
  {
    logger->addVariable(wheelId, VariableLevel::Public, speed);
    logger->addVariable(wheelId + 1, VariableLevel::Private, sensorA);
    logger->addVariable(wheelId + 2, VariableLevel::Private, sensorB);
    logger->addVariable(wheelId + 3, VariableLevel::Private, direction);
  }

  void setup()
  {
    pinMode(pinA, INPUT);
    pinMode(pinB, INPUT);
    attachInterrupt(
        pinA, [this]() { onChange(); }, CHANGE);
    attachInterrupt(
        pinB, [this]() { onChange(); }, CHANGE);
    resetState();
    // stateSpeedTicker.start();
  }

  void loop()
  {
    // stateSpeedTicker.update();
  }

  void resetSpeed()
  {
    // if (sensorA == LOW && sensorB == LOW)
    // {
    //   state = State::Base;
    // }
    // else
    // {
    //   state = State::Unknown;
    // }

    // direction = Direction::Forward;
  }

private:
  //void IRAM_ATTR onChange()
  void onChange()
  {
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
      direction = Direction::Forward;
    }
    else if (
        previousSensorA == LOW && previousSensorB == LOW && sensorA == LOW && sensorB == HIGH ||
        previousSensorA == LOW && previousSensorB == HIGH && sensorA == HIGH && sensorB == HIGH ||
        previousSensorA == HIGH && previousSensorB == HIGH && sensorA == HIGH && sensorB == LOW ||
        previousSensorA == HIGH && previousSensorB == LOW && sensorA == LOW && sensorB == LOW)
    {
      direction = Direction::Reverse;
    }
  }
};

#endif
