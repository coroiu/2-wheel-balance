#ifndef _WHEEL_H
#define _WHEEL_H

#define TOLERATED_DEVIANCE 0.1 // 10%

#include <FilterChain.h>
#include <ChangeDiscardFilter.h>
#include <ChangeLimitFilter.h>
#include <ExpRollingSumFilter.h>
#include <FunctionalInterrupt.h>
#include <Ticker.h>
#include <DataLogger.h>
#include <Speed.h>
#include "Constants.h"

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
  FilterChain speedFilter = FilterChain({
      new ChangeDiscardFilter(1000.0),
      new ChangeLimitFilter(50.0),
      new ExpRollingSumFilter(0.5),
  });
  int direction = Direction::Forward;
  int directionReversal = 1; // 1 = no reversal, -1 = direction reversed
  int sensorA = LOW;
  int sensorB = LOW;

  DataLogger *logger;
  int pinA;
  int pinB;

public:
  Wheel(DataLogger *logger, int pinA, int pinB, bool reversed) : logger(logger), pinA(pinA), pinB(pinB), directionReversal(reversed ? -1 : 1)
  {
    logger->addCalculatedDouble(0, VariableLevel::Public, [this]() { return getSpeed(); });
    // logger->addCalculatedDouble(wheelId + 1, VariableLevel::Private, [this]() { return speedAR.getSpeed(); });
    // logger->addCalculatedDouble(wheelId + 2, VariableLevel::Private, [this]() { return speedBR.getSpeed(); });
    // logger->addVariable(wheelId + 1, VariableLevel::Private, sensorA);
    // logger->addVariable(wheelId + 2, VariableLevel::Private, sensorB);
  }

  void setup()
  {
    pinMode(pinA, INPUT);
    pinMode(pinB, INPUT);

    // attachInterrupt(
    //     pinA, [this]() { onChange(); }, CHANGE);
    // attachInterrupt(
    //     pinB, [this]() { onChange(); }, CHANGE);

    attachInterrupt(
        pinA, [this]() { speedAR.registerEvent(); onChange(); }, RISING);
    // attachInterrupt(
    //     pinA, [this]() { speedAF.registerEvent(); onChange(); }, FALLING);
    attachInterrupt(
        pinB, [this]() { speedBR.registerEvent(); onChange(); }, RISING);
    // attachInterrupt(
    //     pinB, [this]() { speedBF.registerEvent(); onChange(); }, FALLING);

    resetSpeedTicker.start();
  }

  double getSpeed()
  {
    double ar = speedAR.getSpeed();
    double br = speedBR.getSpeed();
    double difference = std::abs(ar - br);
    double mean = (ar + br) / 2.0;
    double deviance = difference / mean;
    double toReturn;

    if (deviance > TOLERATED_DEVIANCE)
    {
      if (std::abs(ar) < std::abs(br))
        toReturn = ar;
      else
        toReturn = br;
    }
    else
    {
      toReturn = mean;
    }

    toReturn = speedFilter.getOutput(toReturn);

    return toReturn * (double)(direction * directionReversal);
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
  void IRAM_ATTR onChange();
  // void onChange()
};

#endif
