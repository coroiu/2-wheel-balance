#ifndef _MAIN_ADVANCED_LOGGER_H
#define _MAIN_ADVANCED_LOGGER_H

#include <functional>
#include <string>
#include <vector>
#include "AdvancedDataLogger.h"

class MainAdvancedDataLogger
{
protected:
  std::vector<AdvancedDataLogger *> loggers;

public:
  AdvancedDataLogger *createLogger(std::string name) {
    AdvancedDataLogger *logger = new AdvancedDataLogger(name);
    loggers.push_back(logger);
    return logger;
  }
};

#endif