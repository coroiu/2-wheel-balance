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
#include "Movement.h"
#include "RollController.h"
#include "WheelSpeedController.h"
#include "InertialMeasurementUnit.h"
#include "GlobalTicker.h"
#include "LED.h"

void log();
void logMeta();
void measureLoop();
void control();

CommandHandler commandHandler(&Serial);
SerialAdvancedDataLogger advancedDataLogger;

TaskHandle_t priorityTask;
void priorityLoop(void* parameter);

Ticker logTicker([]() { log(); }, INTERVAL_LOG_MICROS, 0, MICROS_MICROS);
Ticker logMetaTicker([]() { logMeta(); }, INTERVAL_LOG_META_MICROS, 0, MICROS_MICROS);
Ticker measureLoopTicker([]() { measureLoop(); }, INTERVAL_LOOP_MICROS, 0, MICROS_MICROS);
Ticker controlTicker([]() { control(); }, INTERVAL_CONTROL_MICROS, 0, MICROS_MICROS);

Timer mainLoopTimer;
Timer priorityLoopTimer;
Timer logTimer;
Timer logMetaTimer;
Timer measureLoopTimer;
Timer controlTimer;
double mainLoopTime = .0;
double priorityLoopTime = .0;
double logTime = .0;
double logMetaTime = .0;
double measureLoopTime = .0;
double controlTime = .0;
auto timerLogger = advancedDataLogger.createLogger("Timing");

InertialMeasurementUnit imu(&advancedDataLogger);

Wheel leftWheel(advancedDataLogger.createLogger("Left wheel"), WHEEL_LEFT_PIN_A, WHEEL_LEFT_PIN_B, true);
Motor leftMotor(advancedDataLogger.createLogger("Left motor"), MOTOR_LEFT_IN_1, MOTOR_LEFT_IN_2, MOTOR_LEFT_PWM_PIN, MOTOR_LEFT_PWM_CHANNEL, MOTOR_LEFT_DEADZONE);
// WheelSpeedController leftController(advancedDataLogger.createLogger("Left controller"), &leftWheel, &leftMotor);

Wheel rightWheel(advancedDataLogger.createLogger("Right wheel"), WHEEL_RIGHT_PIN_A, WHEEL_RIGHT_PIN_B, false);
Motor rightMotor(advancedDataLogger.createLogger("Right motor"), MOTOR_RIGHT_IN_1, MOTOR_RIGHT_IN_2, MOTOR_RIGHT_PWM_PIN, MOTOR_RIGHT_PWM_CHANNEL, MOTOR_RIGHT_DEADZONE);
// WheelSpeedController rightController(advancedDataLogger.createLogger("Right controller"), &leftWheel, &rightMotor);

Movement movement(advancedDataLogger.createLogger("Movement"), &leftMotor, &rightMotor);
RollController rollController(advancedDataLogger.createLogger("Roll controller"), &imu, &movement);

Sequence rollControllSequence;

void setup()
{
  Serial.begin(115200);
  Wire.begin(-1, -1, FREQ_IMU_I2C_HZ);
  Serial.printf("Using I2C freq: %d\n", Wire.getClock());

  rightWheel.setup();
  rightMotor.setup();

  leftWheel.setup();
  leftMotor.setup();

  imu.setup();

  led.setup();
  led.setColor(0x220000);

  logTicker.start();
  logMetaTicker.start();
  measureLoopTicker.start();
  controlTicker.start();

  rollControllSequence.addInstruction(0, []() {
    Serial.println("Roll controll sequence started.");
    led.setColor(0x222200);
  });
  rollControllSequence.addInstruction(1000, [&]() {
    Serial.println("Controller enabled");
    led.setColor(0x002200);
    rollController.setTargetRoll(-2.5);
    rollController.enable();
  });
  rollControllSequence.addInstruction(15000, [&]() {
    Serial.println("Controller disabled");
    led.setColor(0x220000);
    rollController.disable();
  });

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

  // commandHandler.command("enable-wheel-speed", [](CommandHandler *handler) {
  //   leftController.enable();
  //   rightController.enable();
  // });

  // commandHandler.command("disable-wheel-speed", [](CommandHandler *handler) {
  //   leftController.disable();
  //   rightController.disable();
  // });

  // commandHandler.command("set-wheel-speed", [](CommandHandler *handler) {
  //   if (handler->argc < 2)
  //     return;
  //   int speed = atoi(handler->argv[1]);
  //   leftController.setSpeed(speed);
  //   rightController.setSpeed(speed);
  //   leftController.enable();
  //   rightController.enable();
  // });

  // commandHandler.command("disable-wheel-speed", [](CommandHandler *handler) {
  //   leftController.disable();
  //   rightController.disable();
  // });

  commandHandler.command("enable-roll-controll", [](CommandHandler *handler) {
    if (handler->argc < 4)
      return;
    double p = atof(handler->argv[1]);
    double i = atof(handler->argv[2]);
    double d = atof(handler->argv[3]);
    rollController.setPID(p, i, d);
    rollControllSequence.run();
  });

  timerLogger->addDouble("MainLoop", "ms", [&]() { return mainLoopTime; });
  timerLogger->addDouble("PriorityLoop", "ms", [&]() { return priorityLoopTime; });
  timerLogger->addDouble("LogLoop", "ms", [&]() { return logTime; });
  timerLogger->addDouble("LogMetaLoop", "ms", [&]() { return logMetaTime; });
  timerLogger->addDouble("MeasureLoop", "ms", [&]() { return measureLoopTime; });
  timerLogger->addDouble("Control", "ms", [&]() { return controlTime; });

  Serial.println("Setup finished");

  xTaskCreatePinnedToCore(
    priorityLoop,
    "DualCoreTask",
    10000,
    NULL,
    10,
    &priorityTask,
    0
  );
}

void loop()
{
  mainLoopTime = mainLoopTimer.measure() / 1000.0;

  logTicker.update();
  logMetaTicker.update();
  GlobalTicker::updateAll();

  commandHandler.update();
}

void priorityLoop(void* parameter) {
  Serial.println("Starting priority loop...");
  for (;;) {
    priorityLoopTime = priorityLoopTimer.measure() / 1000.0;

    measureLoopTicker.update();
    controlTicker.update();
  }
}

void log()
{
  logTime = logTimer.measure() / 1000.0;
  // dataLogger.log();
  advancedDataLogger.log();
}

void logMeta()
{
  logMetaTime = logMetaTimer.measure() / 1000.0;
  advancedDataLogger.logMeta();
}

void measureLoop()
{
  measureLoopTime = measureLoopTimer.measure() / 1000.0;
  leftWheel.loop();
  rightWheel.loop();
  imu.loop();
}

void control()
{
  controlTime = controlTimer.measure() / 1000.0;
  // leftController.control();
  // rightController.control();
  rollController.control();
}