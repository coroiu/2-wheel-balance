#ifndef _CHANGE_DISCARD_FILTER_H
#define _CHANGE_DISCARD_FILTER_H

#include <cmath>
#include "Filter.h"

class ChangeDiscardFilter : public Filter
{
  bool hasPreviousOutput = false;

  double maxChange;
  double previousOutput;

public:
  ChangeDiscardFilter(double maxChange) : maxChange(maxChange)
  {
  }

  double getOutput(double input)
  {
    if (!hasPreviousOutput)
    {
      hasPreviousOutput = true;
      return previousOutput = input;
    }

    if (std::abs(input - previousOutput) > maxChange)
    {
      return previousOutput;
    }

    return previousOutput = input;
  }
};

#endif