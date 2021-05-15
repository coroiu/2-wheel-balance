#ifndef _FILTER_CHAIN_H
#define _FILTER_CHAIN_H

#include <vector>
#include <memory>
#include "Filter.h"

class FilterChain : public Filter
{
  std::vector<std::unique_ptr<Filter>> filters;

public:
  FilterChain(std::initializer_list<Filter *> args)
  {
    for (auto filter : args)
    {
      filters.emplace_back(filter);
    }
  }

  double getOutput(double input) override
  {
    if (filters.size() == 0)
      return input;

    double value = input;
    for (auto &&filter : filters)
    {
      value = filter->getOutput(value);
    }
    return value;
  }
};

#endif