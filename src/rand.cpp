#pragma once
#include "rand.h"
#include <time.h>
static std::mt19937 randEngine((unsigned)time(0));

int rand(int min, int max)
{
std::uniform_int_distribution<long> dist(min,max);
return dist(randEngine);
}

float rand(float min, float max)
{
std::uniform_real_distribution<float> dist(min,max);
return dist(randEngine);
}