#ifndef _ROLL_CONTROLLER_H
#define _ROLL_CONTROLLER_H

#include <MiniPID.h>
#include <AdvancedDataLogger.h>
#include "InertialMeasurementUnit.h"
#include "Wheel.h"
#include "Movement.h"

#define MAX_INPUT 90
#define ROLL_CONTROLLER_P 0.01
#define ROLL_CONTROLLER_I 0
#define ROLL_CONTROLLER_D 0
#define ROLL_CONTROLLER_F 0
#define ROLL_CONTROLLER_SHUTOFF 50

class RollController
{
  bool active = false;
  double setPoint = .0;
  double output = .0;
  double loopTime = .0;

  MiniPID controller;
  InertialMeasurementUnit *imu;
  Movement *movement;
  Timer timer;

public:
  RollController(AdvancedDataLogger *dataLogger, InertialMeasurementUnit *imu, Movement *movement)
      : controller(ROLL_CONTROLLER_P, ROLL_CONTROLLER_I, ROLL_CONTROLLER_D, ROLL_CONTROLLER_F),
        imu(imu),
        movement(movement)
  {
    controller.setOutputLimits(1, 1);
    // controller.setOutputFilter(.1);
    // controller.setOutputRampRate(.2);
    // controller.setSetpointRange(10);
    dataLogger->addDouble("Output", "%", [this]() { return this->output; });
    // dataLogger->addDouble("Timing", "ms", [this]() { return this->loopTime*1000.0; });
  }

  bool isActive() {
    return active;
  }

  void enable()
  {
    active = true;
    controller.reset();
  }

  void disable()
  {
    active = false;
    movement->setPower(0);
  }

  void setTargetRoll(double roll)
  {
    setPoint = roll;
  }

  void setPID(double P, double I, double D) {
    controller.setPID(P, I, D);
  }

  void control()
  {
    loopTime = timer.measureSeconds();

    if (active)
    {
      double roll = imu->roll;

      if (abs(roll) > ROLL_CONTROLLER_SHUTOFF) {
        disable();
        return;
      }

      if (roll > MAX_INPUT)
        roll = MAX_INPUT;
      else if (roll < -MAX_INPUT)
        roll = -MAX_INPUT;

      output = -controller.getOutput(roll, setPoint);
      movement->setPower(output);
    }
    else
    {
      output = .0;
    }
  }
};

#endif