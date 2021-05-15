#ifndef _SERIAL_LOGGER_H
#define _SERIAL_LOGGER_H

#include <map>
#include <functional>
#include <Arduino.h>
#include "DataLogger.h"
#include "OffsetDataLogger.h"

class SerialDataLogger : public DataLogger
{
  enum VariableType
  {
    Integer,
    Double,
    IntegerFunction,
    DoubleFunction
  };

  struct VariableInfo
  {
    VariableType type;
    VariableLevel level;
    void *pointer;
    std::function<int(void)> integerFunction;
    std::function<double(void)> doubleFunction;
  };

  std::map<int, VariableInfo> variables;
  std::vector<DataLogger *> subLoggers;

public:
  void addVariable(int tag, VariableLevel level, int &variable) override
  {
    variables[tag] = VariableInfo{
        .type = VariableType::Integer,
        .level = level,
        .pointer = &variable};
  }

  void addVariable(int tag, VariableLevel level, double &variable) override
  {
    variables[tag] = VariableInfo{
        .type = VariableType::Double,
        .level = level,
        .pointer = &variable};
  }

  void addCalculatedInteger(int tag, VariableLevel level, std::function<int(void)> integerFunction) override
  {
    variables[tag] = VariableInfo{
        .type = VariableType::IntegerFunction,
        .level = level};
    variables[tag].integerFunction = integerFunction;
  }

  void addCalculatedDouble(int tag, VariableLevel level, std::function<double(void)> doubleFunction) override
  {
    variables[tag] = VariableInfo{
        .type = VariableType::DoubleFunction,
        .level = level};
    variables[tag].doubleFunction = doubleFunction;
  }

  DataLogger *createLogger(int offset)
  {
    DataLogger *subLogger = new OffsetDataLogger(this, offset);
    subLoggers.push_back(subLogger);
    return subLogger;
  }

  void log()
  {
    for (auto it = variables.begin(); it != variables.end();)
    {
      if (it->second.type == VariableType::Integer)
      {
        Serial.printf("%d", *(int *)(it->second.pointer));
      }
      else if (it->second.type == VariableType::Double)
      {
        Serial.printf("%.3f", *(double *)(it->second.pointer));
      }
      else if (it->second.type == VariableType::IntegerFunction)
      {
        Serial.printf("%d", it->second.integerFunction());
      }
      else if (it->second.type == VariableType::DoubleFunction)
      {
        Serial.printf("%.3f", it->second.doubleFunction());
      }

      if (++it != variables.end())
      {
        Serial.printf(",");
      }
    }
    Serial.println("");
  }
};

#endif
