#ifndef _ROLL_CONTROLLER_H
#define _ROLL_CONTROLLER_H

#include <MiniPID.h>
#include <AdvancedDataLogger.h>
#include "InertialMeasurementUnit.h"
#include "Wheel.h"
#include "Motor.h"

#define MAX_INPUT M_PI
#define ROLL_CONTROLLER_P 1
#define ROLL_CONTROLLER_I 0.001
#define ROLL_CONTROLLER_D 0
#define ROLL_CONTROLLER_F 0

class RollController
{
  bool active = false;
  double setPoint = .0;
  double output = .0;

  MiniPID controller;
  InertialMeasurementUnit *imu;
  Motor *motor;

public:
  RollController(AdvancedDataLogger *dataLogger, InertialMeasurementUnit *imu, Motor *motor)
      : controller(ROLL_CONTROLLER_P, ROLL_CONTROLLER_I, ROLL_CONTROLLER_D, ROLL_CONTROLLER_F),
        imu(imu),
        motor(motor)
  {
    controller.setOutputLimits(-1.0, 1.0);
    controller.setOutputFilter(.1);
    controller.setOutputRampRate(.2);
    controller.setSetpointRange(10);
    dataLogger->addDouble("Output", "%", [this]() { return this->output; });
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

  void setTargetRoll(double roll)
  {
    setPoint = roll;
  }

  void control()
  {
    if (active)
    {
      double roll = imu->roll;

      if (roll > MAX_INPUT)
        roll = MAX_INPUT;
      else if (roll < -MAX_INPUT)
        roll = -MAX_INPUT;

      output = controller.getOutput(roll, setPoint);
      motor->setPower(output);
    }
    else
    {
      output = .0;
    }
  }
};

#endif