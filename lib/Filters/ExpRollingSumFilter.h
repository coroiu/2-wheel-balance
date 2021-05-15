#ifndef _EXP_ROLLING_SUM_FILTER_H
#define _EXP_ROLLING_SUM_FILTER_H

class ExpRollingSumFilter
{
  double strength = .0;
  double lastOutput = .0;
  bool hasOutput = false;

public:
  ExpRollingSumFilter(double strength) : strength(strength)
  {
    if (strength < .0)
      strength = .0;
    else if (strength > 1.0)
      strength = 1.0;
  }

  double getOutput()
  {
    return lastOutput;
  }

  double getOutput(double input)
  {
    if (hasOutput)
      return lastOutput = lastOutput * strength + input * (1 - strength);

    hasOutput = true;
    return lastOutput = input;
  }
};

#endif