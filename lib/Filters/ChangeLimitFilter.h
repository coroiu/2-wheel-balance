#ifndef _CHANGE_LIMIT_FILTER_H
#define _CHANGE_LIMIT_FILTER_H

#include "Filter.h"

class ChangeLimitFilter : public Filter
{
  bool hasPreviousOutput = false;

  double maxChange;
  double previousOutput;

public:
  ChangeLimitFilter(double maxChange) : maxChange(maxChange)
  {
  }

  double getOutput(double input)
  {
    if (!hasPreviousOutput)
    {
      hasPreviousOutput = true;
      return previousOutput = input;
    }

    if (input - previousOutput > maxChange)
    {
      return previousOutput = previousOutput + maxChange;
    }
    else if (previousOutput - input > maxChange)
    {
      return previousOutput = previousOutput - maxChange;
    }

    return previousOutput = input;
  }
};

#endif