#ifndef _WHEEL_H
#define _WHEEL_H

#include <FunctionalInterrupt.h>
#include <DataLogger.h>

class Wheel
{
  int direction = 0; // forward = 1, unknown = 0, backwards = -1
  double speed = .0;
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
  }

  void setup()
  {
    pinMode(pinA, INPUT);
    pinMode(pinB, INPUT);
    attachInterrupt(
        pinA, [this]() { onChange(); }, CHANGE);
    attachInterrupt(
        pinB, [this]() { onChange(); }, CHANGE);
  }

  void control()
  {
  }

private:
  void IRAM_ATTR onChange()
  {
    sensorA = digitalRead(pinA);
    sensorB = digitalRead(pinB);
  }
};

#endif
