#include "random_source.h"

using namespace std;

RandomSource::RandomSource(int from, int to) : distribution_(from, to)
{
    random_device r;
    seed_seq seeds{r(), r(), r(), r(), r(), r()};
    engine_.seed(seeds);
}

int RandomSource::next()
{
    return distribution_(engine_);
}
