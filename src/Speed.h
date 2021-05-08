#ifndef _SPEED_H
#define _SPEED_H

#include <functional>
#include <Arduino.h>

class Speed
{
  bool hasPreviousEvent = false;
  bool hasPreviousInterval = false;
  unsigned long previousEventTime = 0;
  unsigned long previousInterval = 0;
  // std::function<void(void)> callback ;

public:
  void registerEvent()
  {
    unsigned long eventTime = micros();

    if (hasPreviousEvent)
    {
      previousInterval = eventTime - previousEventTime;
      hasPreviousInterval = true;
    }

    hasPreviousEvent = true;
    previousEventTime = eventTime;
  }

  // Events per second
  double getSpeed()
  {
    if (!hasPreviousInterval)
      return .0;

    return 1000000.0 / previousInterval;
  }

  void reset()
  {
    hasPreviousEvent = false;
    hasPreviousInterval = false;
  }
};

#endif
