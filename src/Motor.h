#ifndef _MOTOR_H
#define _MOTOR_H

#include <esp32-hal.h>
#include <soc/soc_caps.h>
#include <driver/ledc.h>

#include <AdvancedDataLogger.h>
#include "Sequence.h"
#include "utils.h"

class Motor
{
  double deadzone = .0;
  double power = .0;
  double realPower = .0;
  Sequence testSequence;

  AdvancedDataLogger *logger;
  int pinA;
  int pinB;
  int pwmPin;
  int pwmChannel;

public:
  Motor(AdvancedDataLogger *dataLogger, int pinA, int pinB, int pwmPin, int pwmChannel, double deadzone) : logger(dataLogger), pinA(pinA), pinB(pinB), pwmPin(pwmPin), pwmChannel(pwmChannel), deadzone(deadzone)
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
    logger->addDouble("Power", "%", [this]() { return this->power; });
    logger->addDouble("Real power", "%", [this]() { return this->realPower; });
    pinMode(pinA, OUTPUT);
    pinMode(pinB, OUTPUT);
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, LOW);
    ledcAttachPin(pwmPin, pwmChannel);
    // double resFreq = custom_ledcSetup(pwmChannel, MOTOR_PWM_FREQ, MOTOR_PWM_RESOLUTION_BITS);
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
    power = _power;
    if (power < -1.0)
      power = -1.0;
    else if (power > 1.0)
      power = 1.0;

    if (almost_equal(power, .0, MOTOR_POWER_RESOLUTION_DECIMALS))
    {
      digitalWrite(pinA, LOW);
      digitalWrite(pinB, LOW);
      realPower = 0;
      ledcWrite(pwmChannel, 0);
      return;
    }

    realPower = (1 - deadzone) * abs(power) + deadzone;
    double output = realPower * MOTOR_PWM_MAX_VALUE;

    if (power < .0)
    {
      digitalWrite(pinA, LOW);
      digitalWrite(pinB, HIGH);
      ledcWrite(pwmChannel, output);
    }
    else if (power > .0)
    {
      digitalWrite(pinA, HIGH);
      digitalWrite(pinB, LOW);
      ledcWrite(pwmChannel, output);
    }
  }
};

#endif
