#ifndef _LOGGER_H
#define _LOGGER_H

#include <map>
#include <Arduino.h>

class DataLogger
{
  enum VariableType
  {
    Integer
  };

  struct VariableInfo
  {
    VariableType type;
    void *pointer;
  };

  std::map<int, VariableInfo> variables;

public:
  void addVariable(int tag, int &variable)
  {
    variables[tag] = VariableInfo{
        .type = VariableType::Integer,
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

      if (++it != variables.end())
      {
        Serial.printf(",");
      }
    }
    Serial.println("");
  }
};

#endif
