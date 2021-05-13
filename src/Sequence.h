#ifndef _SEQUENCE_H
#define _SEQUENCE_H

#include <vector>
#include <functional>
#include "GlobalTicker.h"

class Sequence
{
  struct Instruction
  {
    uint32_t delayMillis;
    std::function<void(void)> command;
  };

  GlobalTicker ticker = GlobalTicker(
      0, [this]() { currentInstruction.command(); next(); }, 1);
  std::vector<Instruction> instructions;
  size_t currentIndex;
  Instruction currentInstruction;

public:
  void run()
  {
    currentIndex = -1;
    next();
  }

  void stop()
  {
    ticker.stop();
  }

  void addInstruction(uint32_t delayMillis, std::function<void(void)> command)
  {
    instructions.push_back(Instruction{
        .delayMillis = delayMillis,
        .command = command});
  }

private:
  void next()
  {
    ++currentIndex;
    if (currentIndex == instructions.size())
    {
      stop();
      return;
    }

    currentInstruction = instructions[currentIndex];
    ticker.interval(currentInstruction.delayMillis);
    ticker.start();
  }
};

#endif