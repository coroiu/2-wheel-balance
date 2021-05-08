#include <Arduino.h>
#include <FunctionalInterrupt.h>
#include <DataLogger.h>
#include <Ticker.h>
#include "Constants.h"
#include "Wheel.h"

void log();
void timedLoop();
void control();

Ticker logTicker([]() { log(); }, INTERVAL_LOG_MICROS, 0, MICROS_MICROS);
Ticker loopTicker([]() { timedLoop(); }, INTERVAL_CONTROL_MICROS, 0, MICROS_MICROS);
Ticker controlTicker([]() { control(); }, INTERVAL_CONTROL_MICROS, 0, MICROS_MICROS);

DataLogger dataLogger;
Wheel leftWheel(&dataLogger, WHEEL_LEFT, WHEEL_LEFT_PIN_A, WHEEL_LEFT_PIN_B);

void setup()
{
  Serial.begin(115200);

  leftWheel.setup();

  logTicker.start();
  loopTicker.start();
  controlTicker.start();
}

void loop()
{
  logTicker.update();
  loopTicker.update();
  controlTicker.update();
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