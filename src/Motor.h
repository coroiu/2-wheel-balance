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
      digitalWrite(this->pinB, LOW);
    });
    testSequence.addInstruction(1000, [this]() {
      Serial.println("1% power.");
      setPower(.01);
    });
    testSequence.addInstruction(1000, [this]() {
      Serial.println("2% power.");
      setPower(.02);
    });
    testSequence.addInstruction(1000, [this]() {
      Serial.println("3% power.");
      setPower(.03);
    });
    testSequence.addInstruction(1000, [this]() {
      Serial.println("5% power.");
      setPower(.05);
    });
    testSequence.addInstruction(1000, [this]() {
      Serial.println("10% power.");
      setPower(.1);
    });
    testSequence.addInstruction(1000, [this]() {
      Serial.println("0% power.");
      setPower(.0);
    });
    testSequence.addInstruction(1000, [this]() {
      Serial.println("Pins - A: LOW - B: Low");
      digitalWrite(this->pinA, LOW);
      digitalWrite(this->pinB, LOW);
    });
    testSequence.addInstruction(0, []() {
      Serial.println("Sequence finished.");
    });
  }

  void setup()
  {
    Serial.printf("Setting up motor %d-%d-%d\n", pinA, pinB, pwmPin);
    // logger->addVariable(loggerOffset, VariableLevel::Public, power);
    pinMode(pinA, OUTPUT);
    pinMode(pinB, OUTPUT);
    digitalWrite(this->pinA, LOW);
    digitalWrite(this->pinB, LOW);
    ledcAttachPin(pwmPin, pwmChannel);
    double resFreq = ledcSetup(pwmChannel, 20000, 16);
    Serial.printf("Done. Using pwm freq: %.4f\n", resFreq);
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

    ledcWrite(pwmChannel, 65535 * _power);
    power = _power;
  }
};

#endif
