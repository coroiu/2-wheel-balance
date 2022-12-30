#ifndef _ADVANCED_SERIAL_LOGGER_H
#define _ADVANCED_SERIAL_LOGGER_H

#include <map>
#include <functional>
#include <vector>
#include <string>
#include <Arduino.h>
#include <ArduinoJson.h>
#include "MainAdvancedDataLogger.h"

class SerialAdvancedDataLogger : public MainAdvancedDataLogger
{
  DynamicJsonDocument doc = DynamicJsonDocument(2048);

public:
  void log()
  {
    Serial.print("/*");

    for (auto itLoggers = loggers.begin(); itLoggers != loggers.end();)
    {
      auto& logger = *itLoggers;
      auto& variables = logger->variables;

      for (auto itVariables = variables.begin(); itVariables != variables.end();)
      {
        auto& variable = *itVariables;
        if (variable.type == VariableType::Integer)
        {
          Serial.printf("%d", variable.integerFunction());
        }
        else if (variable.type == VariableType::Double)
        {
          Serial.printf("%.3f", variable.doubleFunction());
        }

        if (++itVariables != variables.end())
        {
          Serial.printf(",");
        }
      }

      if (++itLoggers != loggers.end())
      {
        Serial.printf(",");
      }
    }

    Serial.println("*/");
  }

  void logMeta()
  {
    doc.clear();
    doc["t"] = "2-wheel-balance";
    auto groups = doc.createNestedArray("g");
    int index(1);

    for (auto itLoggers = loggers.begin(); itLoggers != loggers.end(); ++itLoggers)
    {
      auto& logger = *itLoggers;
      auto& variables = logger->variables;
      auto group = groups.createNestedObject();
      auto datasets = group.createNestedArray("d");
      
      group["t"] = logger->name;

      for (auto itVariables = variables.begin(); itVariables != variables.end(); ++itVariables)
      {
        auto& variable = *itVariables;
        if (variable.type == VariableType::Integer || variable.type == VariableType::Double)
        {
          // group['w'] = nothing
          auto dataset = datasets.createNestedObject();
          dataset["t"] = variable.name;
          dataset["u"] = variable.unit;
          dataset["v"] = "%" + std::to_string(index++);
          // dataset['w'] = nothing
        }
      }
    }

    Serial.print("/*");
    serializeJson(doc, Serial);
    Serial.println("*/");
  }
};

#endif
