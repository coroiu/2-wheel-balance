#ifndef _LOGGER_H
#define _LOGGER_H

#include <map>
#include <Arduino.h>

enum VariableLevel
{
  Debug,
  Private,
  Public,
  Root
};

class DataLogger
{
  enum VariableType
  {
    Integer,
    Double
  };

  struct VariableInfo
  {
    VariableType type;
    VariableLevel level;
    void *pointer;
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
    variables[tag] = VariableInfo{
        .type = VariableType::Double,
        .level = level,
        .pointer = &variable};
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

      if (++it != variables.end())
      {
        Serial.printf(",");
      }
    }
    Serial.println("");
  }
};

#endif
