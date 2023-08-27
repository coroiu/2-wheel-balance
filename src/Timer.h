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

  long measureSeconds()
  {
    auto interval = measure();
    return interval * 10e6;
  }
};

#endif