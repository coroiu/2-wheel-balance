#include <Arduino.h>
#include <FunctionalInterrupt.h>
#include <SerialDataLogger.h>
#include <Ticker.h>
#include <CommandHandler.h>
#include "Constants.h"
#include "Wheel.h"
#include "Motor.h"
#include "WheelSpeedController.h"
// #include "WheelAccelSpeedController.h"
#include "GlobalTicker.h"

void log();
void timedLoop();
void control();

Ticker logTicker([]() { log(); }, INTERVAL_LOG_MICROS, 0, MICROS_MICROS);
Ticker loopTicker([]() { timedLoop(); }, INTERVAL_LOOP_MICROS, 0, MICROS_MICROS);
Ticker controlTicker([]() { control(); }, INTERVAL_CONTROL_MICROS, 0, MICROS_MICROS);

CommandHandler commandHandler(&Serial);
SerialDataLogger dataLogger;
Wheel leftWheel(dataLogger.createLogger(100), WHEEL_LEFT_PIN_A, WHEEL_LEFT_PIN_B);
Motor leftMotor(dataLogger.createLogger(200), MOTOR_LEFT_IN_1, MOTOR_LEFT_IN_2, MOTOR_LEFT_PWM_PIN, MOTOR_LEFT_PWM_CHANNEL);
WheelSpeedController leftController(dataLogger.createLogger(300), &leftWheel, &leftMotor);
// WheelAccelSpeedController leftController(&dataLogger, &leftWheel, &leftMotor);

void setup()
{
  Serial.begin(115200);

  leftWheel.setup();
  leftMotor.setup();

  logTicker.start();
  loopTicker.start();
  controlTicker.start();

  commandHandler.command("test-motors", [](CommandHandler *handler) {
    leftMotor.test();
  });

  commandHandler.command("set-motors", [](CommandHandler *handler) {
    if (handler->argc < 2)
      return;
    int power = atoi(handler->argv[1]);
    leftMotor.setPower((double)power / 100.0);
  });

  commandHandler.command("enable-wheel-speed", [](CommandHandler *handler) {
    leftController.enable();
  });

  commandHandler.command("disable-wheel-speed", [](CommandHandler *handler) {
    leftController.disable();
  });

  commandHandler.command("set-wheel-speed", [](CommandHandler *handler) {
    if (handler->argc < 2)
      return;
    int speed = atoi(handler->argv[1]);
    leftController.setSpeed(speed);
    leftController.enable();
  });

  Serial.println("Setup finished");
}

void loop()
{
  logTicker.update();
  loopTicker.update();
  controlTicker.update();
  GlobalTicker::updateAll();

  commandHandler.update();
}

void log()
{
  dataLogger.log();
}

void timedLoop()
{
  leftWheel.loop();
}

void control()
{
  // leftWheel.control();
  leftController.control();
}