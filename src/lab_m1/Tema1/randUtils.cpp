#include "randUtils.h"

float randUtils::randInterval(float a, float b)
{
    float f = (float)rand() / (float)(RAND_MAX);
    return (b - a) * f + a;
}
