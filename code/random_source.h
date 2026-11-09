#pragma once

#include <random>

class RandomSource
{
public:
    RandomSource(int from, int to);
    int next();

private:
    std::mt19937 engine_;
    std::uniform_int_distribution<int> distribution_;
};
