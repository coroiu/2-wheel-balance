#include <Arduino.h>
#include <FunctionalInterrupt.h>
#include <DataLogger.h>

DataLogger dataLogger;

#define PIN_A 21
#define PIN_B 17

int a(0);
int b(0);

void IRAM_ATTR interrupt(int arg)
{
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  a = digitalRead(PIN_A);
  b = digitalRead(PIN_B);
}

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  int test(3);
  // esp_intr_alloc(21, ESP_INTR_FLAG_IRAM | ETS_GPIO_INTR_SOURCE, interrupt, NULL, NULL);
  dataLogger.addVariable(0, a);
  dataLogger.addVariable(1, b);

  attachInterrupt(
      21, [&]() { interrupt(test); }, CHANGE);
  attachInterrupt(
      17, [&]() { interrupt(test); }, CHANGE);
}

void loop()
{
  // Serial.printf("%d,%d\n", a, b);
  dataLogger.log();
  delay(20);
}