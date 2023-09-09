#ifndef _TIMER_H
#define _TIMER_H

#include <esp32-hal.h>

class Timer
{
  bool hasPreviousTime = false;
  unsigned long previousTime = 0;

public:
  long measure()
  {
    unsigned long time = micros();

    if (!hasPreviousTime)
    {
      hasPreviousTime = true;
      previousTime = time;
      return 0;
    }

    long interval = time - previousTime;
    previousTime = time;
    return interval;
  }

  double measureSeconds()
  {
    auto interval = measure();
    return interval / 1000000.0;
  }
};

#endif