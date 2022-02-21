#include "randomUtils.h"

float randomUtils::randIntervalF(float a, float b)
{
    float f = (float)rand() / (float)(RAND_MAX);
    return (b - a) * f + a;
}

int randomUtils::randIntervalI(int a, int b)
{
    int x = rand() % (b - a + 1);
    return a + x;
}
