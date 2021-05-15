#ifndef _LOGGER_H
#define _LOGGER_H

#include <functional>

enum VariableLevel
{
  Debug = 0,
  Private = 1,
  Public = 2,
  Root = 3
};

class DataLogger
{
public:
  virtual void addVariable(int tag, VariableLevel level, int &variable) = 0;
  virtual void addVariable(int tag, VariableLevel level, double &variable) = 0;
  virtual void addCalculatedInteger(int tag, VariableLevel level, std::function<int(void)> integerFunction) = 0;
  virtual void addCalculatedDouble(int tag, VariableLevel level, std::function<double(void)> doubleFunction) = 0;
};

#endif
