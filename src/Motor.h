#ifndef _MOTOR_H
#define _MOTOR_H

#include <DataLogger.h>
#include "Sequence.h"

class Motor
{
  double power = .0;
  Sequence testSequence;

  DataLogger *logger;
  int loggerOffset;
  int pinA;
  int pinB;
  int pwmPin;
  int pwmChannel;

public:
  Motor(DataLogger *dataLogger, int loggerOffset, int pinA, int pinB, int pwmPin, int pwmChannel) : logger(), loggerOffset(loggerOffset), pinA(pinA), pinB(pinB), pwmPin(pwmPin), pwmChannel(pwmChannel)
  {
    testSequence.addInstruction(0, []() {
      Serial.println("Test sequence started.");
    });
    testSequence.addInstruction(1000, [this]() {
      Serial.println("Pins - A: High - B: Low");
      digitalWrite(this->pinA, HIGH);
      digitalWrite(this->pinB, HIGH);
    });
    testSequence.addInstruction(1000, []() {
      Serial.println("1% power.");
    });
    testSequence.addInstruction(1000, []() {
      Serial.println("2% power.");
    });
    testSequence.addInstruction(1000, []() {
      Serial.println("3% power.");
    });
    testSequence.addInstruction(1000, []() {
      Serial.println("5% power.");
    });
    testSequence.addInstruction(1000, []() {
      Serial.println("10% power.");
    });
    testSequence.addInstruction(0, []() {
      Serial.println("Sequence finished.");
    });
  }

  void setup()
  {
    logger->addVariable(loggerOffset, VariableLevel::Public, power);
    pinMode(pinA, OUTPUT);
    pinMode(pinB, OUTPUT);
    ledcAttachPin(pwmPin, pwmChannel);
    ledcSetup(pwmChannel, 10000, 16);
  }

  void test()
  {
    testSequence.run();
  }

  void setPower(double _power)
  {
    if (_power < .0)
      _power = .0;
    else if (_power > 100.0)
      _power = 100.0;

    ledcWrite(0, 65535 * _power);
    power = _power;
  }
};

#endif
