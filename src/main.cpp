#include <Arduino.h>
#include <FunctionalInterrupt.h>
#include <SerialAdvancedDataLogger.h>
#include <SerialDataLogger.h>
#include <Ticker.h>
#include <CommandHandler.h>

//
// Needed on macos
#include <FilterChain.h>
//

#include "Constants.h"
#include "Wheel.h"
#include "Motor.h"
#include "WheelSpeedController.h"
#include "InertialMeasurementUnit.h"
#include "GlobalTicker.h"

void log();
void logMeta();
void timedLoop();
void control();

Ticker logTicker([]() { log(); }, INTERVAL_LOG_MICROS, 0, MICROS_MICROS);
Ticker logMetaTicker([]() { logMeta(); }, INTERVAL_LOG_META_MICROS, 0, MICROS_MICROS);
Ticker loopTicker([]() { timedLoop(); }, INTERVAL_LOOP_MICROS, 0, MICROS_MICROS);
Ticker controlTicker([]() { control(); }, INTERVAL_CONTROL_MICROS, 0, MICROS_MICROS);

CommandHandler commandHandler(&Serial);
SerialAdvancedDataLogger advancedDataLogger;
// SerialDataLogger dataLogger;

Wheel leftWheel(advancedDataLogger.createLogger("Left wheel"), WHEEL_LEFT_PIN_A, WHEEL_LEFT_PIN_B, true);
Motor leftMotor(advancedDataLogger.createLogger("Left motor"), MOTOR_LEFT_IN_1, MOTOR_LEFT_IN_2, MOTOR_LEFT_PWM_PIN, MOTOR_LEFT_PWM_CHANNEL);
WheelSpeedController leftController(advancedDataLogger.createLogger("Left controller"), &leftWheel, &leftMotor);

Wheel rightWheel(advancedDataLogger.createLogger("Right wheel"), WHEEL_RIGHT_PIN_A, WHEEL_RIGHT_PIN_B, false);
Motor rightMotor(advancedDataLogger.createLogger("Right motor"), MOTOR_RIGHT_IN_1, MOTOR_RIGHT_IN_2, MOTOR_RIGHT_PWM_PIN, MOTOR_RIGHT_PWM_CHANNEL);
WheelSpeedController rightController(advancedDataLogger.createLogger("Right controller"), &leftWheel, &rightMotor);

InertialMeasurementUnit imu(advancedDataLogger.createLogger("IMU"));

void setup()
{
  Serial.begin(115200);

  rightWheel.setup();
  rightMotor.setup();

  leftWheel.setup();
  leftMotor.setup();

  imu.setup();

  logTicker.start();
  logMetaTicker.start();
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
  logMetaTicker.update();
  loopTicker.update();
  controlTicker.update();
  GlobalTicker::updateAll();

  commandHandler.update();
}

void log()
{
  // dataLogger.log();
  advancedDataLogger.log();
}

void logMeta()
{
  advancedDataLogger.logMeta();
}

void timedLoop()
{
  leftWheel.loop();
  rightWheel.loop();
  imu.loop();
}

void control()
{
  leftController.control();
  rightController.control();
}