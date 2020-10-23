
#include "MathHelper.h"
#include <random>

float MathHelper::RandFloat()
{
	return static_cast<float>(std::rand()) / RAND_MAX;
}


float MathHelper::RandFloat(int l, float h)
{
	return (RandFloat() * (h - l) + l);
}

int MathHelper::RandInt(int l, int h)
{
	return (static_cast<int>(RandFloat(0, h - l + 1) + l));
}