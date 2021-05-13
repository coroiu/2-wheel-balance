#include <Arduino.h>
#include <FunctionalInterrupt.h>
#include <DataLogger.h>
#include <Ticker.h>
#include "Constants.h"
#include "Wheel.h"
#include "Motor.h"
#include "GlobalTicker.h"

void log();
void timedLoop();
void control();

Ticker logTicker([]() { log(); }, INTERVAL_LOG_MICROS, 0, MICROS_MICROS);
Ticker loopTicker([]() { timedLoop(); }, INTERVAL_CONTROL_MICROS, 0, MICROS_MICROS);
Ticker controlTicker([]() { control(); }, INTERVAL_CONTROL_MICROS, 0, MICROS_MICROS);

DataLogger dataLogger;
Wheel leftWheel(&dataLogger, WHEEL_LEFT, WHEEL_LEFT_PIN_A, WHEEL_LEFT_PIN_B);
Motor leftMotor(&dataLogger, MOTOR_LEFT, MOTOR_LEFT_1, MOTOR_LEFT_2, MOTOR_LEFT_PWM_PIN, MOTOR_LEFT_PWM_CHANNEL);

void setup()
{
  Serial.begin(115200);

  leftWheel.setup();

  logTicker.start();
  loopTicker.start();
  controlTicker.start();

  // leftMotor.test();
}

void loop()
{
  logTicker.update();
  loopTicker.update();
  controlTicker.update();
  GlobalTicker::updateAll();
}

void log()
{
  // dataLogger.log();
}

void timedLoop()
{
  leftWheel.loop();
}

void control()
{
  // leftWheel.control();
}