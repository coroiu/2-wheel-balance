#ifndef _MOVEMENET_H
#define _MOVEMENET_H

#include <AdvancedDataLogger.h>
#include "Sequence.h"
#include "Motor.h"
#include "utils.h"

class Movement
{
  double power = .0;
  double differential = .0;

  AdvancedDataLogger *logger;
  Motor *leftMotor;
  Motor *rightMotor;

public:
  Movement(AdvancedDataLogger *dataLogger, Motor *leftMotor, Motor *rightMotor) : logger(dataLogger), leftMotor(leftMotor), rightMotor(rightMotor)
  {
  }

  void setup()
  {
    // logger->addDouble("Power", "%", [this]() { return this->power; });
  }

  void setPower(double _power)
  {
    power = _power;
    setMotors();
  }

  void setDifferential(double _differential)
  {
    differential = _differential;
  }

private:
  void setMotors()
  {
    leftMotor->setPower(-power - differential);
    rightMotor->setPower(power + differential);
  }
};

#endif
