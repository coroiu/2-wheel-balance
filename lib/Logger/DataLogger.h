#ifndef _LOGGER_H
#define _LOGGER_H

#include <map>
#include <functional>
#include <Arduino.h>

enum VariableLevel
{
  Debug = 0,
  Private = 1,
  Public = 2,
  Root = 3
};

class DataLogger
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

public:
  void addVariable(int tag, VariableLevel level, int &variable)
  {
    variables[tag] = VariableInfo{
        .type = VariableType::Integer,
        .level = level,
        .pointer = &variable};
  }

  void addVariable(int tag, VariableLevel level, double &variable)
  {
    Serial.printf("Trying to add variable: %d %d\n", tag, level);
    variables[tag] = VariableInfo{
        .type = VariableType::Double,
        .level = level,
        .pointer = &variable};
  }

  void addCalculatedInteger(int tag, VariableLevel level, std::function<int(void)> integerFunction)
  {
    variables[tag] = VariableInfo{
        .type = VariableType::IntegerFunction,
        .level = level};
    variables[tag].integerFunction = integerFunction;
  }

  void addCalculatedDouble(int tag, VariableLevel level, std::function<double(void)> doubleFunction)
  {
    variables[tag] = VariableInfo{
        .type = VariableType::DoubleFunction,
        .level = level};
    variables[tag].doubleFunction = doubleFunction;
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
