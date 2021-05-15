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
Wheel leftWheel(dataLogger.createLogger(1000), WHEEL_LEFT_PIN_A, WHEEL_LEFT_PIN_B, true);
Motor leftMotor(dataLogger.createLogger(1100), MOTOR_LEFT_IN_1, MOTOR_LEFT_IN_2, MOTOR_LEFT_PWM_PIN, MOTOR_LEFT_PWM_CHANNEL);
WheelSpeedController leftController(dataLogger.createLogger(1200), &leftWheel, &leftMotor);

Wheel rightWheel(dataLogger.createLogger(2000), WHEEL_RIGHT_PIN_A, WHEEL_RIGHT_PIN_B, false);
Motor rightMotor(dataLogger.createLogger(2100), MOTOR_RIGHT_IN_1, MOTOR_RIGHT_IN_2, MOTOR_RIGHT_PWM_PIN, MOTOR_RIGHT_PWM_CHANNEL);
WheelSpeedController rightController(dataLogger.createLogger(2200), &leftWheel, &rightMotor);

void setup()
{
  Serial.begin(115200);

  rightWheel.setup();
  rightMotor.setup();

  leftWheel.setup();
  leftMotor.setup();

  logTicker.start();
  loopTicker.start();
  controlTicker.start();

  commandHandler.command("test-motors", [](CommandHandler *handler) {
    leftMotor.test();
    rightMotor.test();
  });

  commandHandler.command("set-motors", [](CommandHandler *handler) {
    if (handler->argc < 2)
      return;
    int power = atoi(handler->argv[1]);
    leftMotor.setPower((double)power / 100.0);
    rightMotor.setPower((double)power / 100.0);
  });

  commandHandler.command("enable-wheel-speed", [](CommandHandler *handler) {
    leftController.enable();
    rightController.enable();
  });

  commandHandler.command("disable-wheel-speed", [](CommandHandler *handler) {
    leftController.disable();
    rightController.disable();
  });

  commandHandler.command("set-wheel-speed", [](CommandHandler *handler) {
    if (handler->argc < 2)
      return;
    int speed = atoi(handler->argv[1]);
    leftController.setSpeed(speed);
    rightController.setSpeed(speed);
    leftController.enable();
    rightController.enable();
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
  rightWheel.loop();
}

void control()
{
  leftController.control();
  rightController.control();
}