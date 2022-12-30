#ifndef _ADVANCED_LOGGER_H
#define _ADVANCED_LOGGER_H

#include <functional>
#include <string>
#include <vector>

// enum LoggerType
// {
//   Basic,
//   Map, // Geographical position
//   ProgressBar, // Progress bar
//   Gyroscope, // Gyroscope with x, y, z
//   Accelerometer // Accelermoeter with x, y, z
// };


enum VariableType
{
  Integer,
  Double,
};

struct VariableInfo
{
  std::string name;
  std::string unit;
  VariableType type;
  void *pointer;
  std::function<int(void)> integerFunction;
  std::function<double(void)> doubleFunction;
};

class AdvancedDataLogger
{
public:
  std::string name;
  std::vector<VariableInfo> variables;

public:
  AdvancedDataLogger(std::string name) : name(name) {}
 
  void addInteger(std::string name, std::string unit, std::function<int(void)> integerFunction)
  {
    variables.push_back(VariableInfo{
        .name = name,
        .unit = unit,
        .type = VariableType::Integer,
        .integerFunction = integerFunction
    });
  }

  void addDouble(std::string name, std::string unit, std::function<double(void)> doubleFunction)
  {
    variables.push_back(VariableInfo{
        .name = name,
        .unit = unit,
        .type = VariableType::Double,
        .doubleFunction = doubleFunction
    });
  }
};

#endif