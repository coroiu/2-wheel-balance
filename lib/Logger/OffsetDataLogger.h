#ifndef _SUB_DATA_LOGGER_H
#define _SUB_DATA_LOGGER_H

#include "DataLogger.h"

class OffsetDataLogger : public DataLogger
{
  DataLogger *logger;
  int offset;

public:
  OffsetDataLogger(DataLogger *logger, int offset) : logger(logger), offset(offset)
  {
  }

  void addVariable(int tag, VariableLevel level, int &variable) override
  {
    logger->addVariable(offset + tag, level, variable);
  }

  void addVariable(int tag, VariableLevel level, double &variable) override
  {
    logger->addVariable(offset + tag, level, variable);
  }

  void addCalculatedInteger(int tag, VariableLevel level, std::function<int(void)> integerFunction) override
  {
    logger->addCalculatedInteger(offset + tag, level, integerFunction);
  }

  void addCalculatedDouble(int tag, VariableLevel level, std::function<double(void)> doubleFunction) override
  {
    logger->addCalculatedDouble(offset + tag, level, doubleFunction);
  }
};

#endif