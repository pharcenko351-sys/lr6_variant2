#include "generator.h"
#include "random_source.h"
#include "wait_controller.h"
#include <iostream>
#include <format>

using namespace std;

namespace
{
const int MIN_VALUE = 1;
const int MAX_VALUE = 256;
const int MIN_DIFFERENCE = 16;
}

Task generate(int count)
{
    RandomSource source(MIN_VALUE, MAX_VALUE);
    WaitController controller(MIN_DIFFERENCE);
    for (int i = 1; i <= count; ++i)
    {
        int value = source.next();
        cout << format("Число {}: {}\n", i, value);
        co_await controller.wait(value);
    }
}
