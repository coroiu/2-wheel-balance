#ifndef _MOTOR_H
#define _MOTOR_H

#include <DataLogger.h>
#include "Sequence.h"
#include "utils.h"

class Motor
{
  double power = .0;
  Sequence testSequence;

  DataLogger *logger;
  int pinA;
  int pinB;
  int pwmPin;
  int pwmChannel;

public:
  Motor(DataLogger *dataLogger, int pinA, int pinB, int pwmPin, int pwmChannel) : logger(dataLogger), pinA(pinA), pinB(pinB), pwmPin(pwmPin), pwmChannel(pwmChannel)
  {
    testSequence.addInstruction(0, []() {
      Serial.println("Test sequence started.");
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
      Serial.println("20% power.");
      setPower(.2);
    });
    testSequence.addInstruction(1000, [this]() {
      Serial.println("50% power.");
      setPower(.5);
    });
    testSequence.addInstruction(1000, [this]() {
      Serial.println("0% power.");
      setPower(.0);
    });
    testSequence.addInstruction(5000, [this]() {
      Serial.println("-10% power.");
      setPower(-.1);
    });
    testSequence.addInstruction(1000, [this]() {
      Serial.println("-20% power.");
      setPower(-.2);
    });
    testSequence.addInstruction(1000, [this]() {
      Serial.println("-50% power.");
      setPower(-.5);
    });
    testSequence.addInstruction(1000, [this]() {
      Serial.println("0% power.");
      setPower(.0);
    });
    testSequence.addInstruction(0, []() {
      Serial.println("Sequence finished.");
    });
  }

  void setup()
  {
    Serial.printf("Setting up motor %d-%d-%d\n", pinA, pinB, pwmPin);
    logger->addVariable(0, VariableLevel::Public, power);
    pinMode(pinA, OUTPUT);
    pinMode(pinB, OUTPUT);
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, LOW);
    ledcAttachPin(pwmPin, pwmChannel);
    double resFreq = ledcSetup(pwmChannel, MOTOR_PWM_FREQ, MOTOR_PWM_RESOLUTION_BITS);
    Serial.printf("Done. Using pwm freq: %.4f\n", resFreq);
  }

  void test()
  {
    testSequence.run();
  }

  void adjustPower(double adjustment)
  {
    setPower(power + adjustment);
  }

  void setPower(double _power)
  {
    if (_power < -1.0)
      _power = -1.0;
    else if (_power > 1.0)
      _power = 1.0;

    if (almost_equal(_power, .0, MOTOR_POWER_RESOLUTION_DECIMALS))
    {
      digitalWrite(pinA, LOW);
      digitalWrite(pinB, LOW);
      ledcWrite(pwmChannel, 0);
    }
    else if (_power < .0)
    {
      digitalWrite(pinA, LOW);
      digitalWrite(pinB, HIGH);
      ledcWrite(pwmChannel, -65535 * _power);
    }
    else if (_power > .0)
    {
      digitalWrite(pinA, HIGH);
      digitalWrite(pinB, LOW);
      ledcWrite(pwmChannel, 65535 * _power);
    }

    power = _power;
  }
};

#endif
