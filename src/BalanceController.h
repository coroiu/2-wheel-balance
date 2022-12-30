#ifndef _BALANCE_CONTROLLER_H
#define _BALANCE_CONTROLLER_H

#define MAX_INPUT 3.141592
// #define BALANCE_CONTROLLER_P 0.002
#define BALANCE_CONTROLLER_P 0.0001
#define BALANCE_CONTROLLER_I 0
#define BALANCE_CONTROLLER_D 0
#define BALANCE_CONTROLLER_F 0
// #define BALANCE_CONTROLLER_F 1.0 / (9.0 * 100.0)

#include <MiniPID.h>
#include <DataLogger.h>
#include "Wheel.h"
#include "Motor.h"

class WheelSpeedController
{
  bool active = false;
  double setPoint = .0;
  double output = .0;

  MiniPID controller;
  Wheel *wheel;
  Motor *motor;

public:
  WheelSpeedController(DataLogger *dataLogger, Wheel *wheel, Motor *motor)
      : controller(BALANCE_CONTROLLER_P, BALANCE_CONTROLLER_I, BALANCE_CONTROLLER_D, BALANCE_CONTROLLER_F),
        wheel(wheel),
        motor(motor)
  {
    controller.setOutputLimits(-1.0, 1.0);
    controller.setOutputFilter(.1);
    controller.setOutputRampRate(.2);
    controller.setSetpointRange(10);
    dataLogger->addVariable(0, VariableLevel::Public, output);
  }

  void enable()
  {
    active = true;
  }

  void disable()
  {
    active = false;
    motor->setPower(0);
  }

  void setSpeed(double speed)
  {
    setPoint = speed;
  }

  void control()
  {
    if (active)
    {
      double wheelSpeed = wheel->getSpeed();

      if (wheelSpeed > MAX_INPUT)
        wheelSpeed = MAX_INPUT;
      else if (wheelSpeed < -MAX_INPUT)
        wheelSpeed = -MAX_INPUT;

      output = controller.getOutput(wheelSpeed, setPoint);
      motor->setPower(output);
    }
    else
    {
      output = .0;
    }
  }
};

#endif