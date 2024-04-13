#include "MathUtils.h"

float MathUtils::Lerp(float a, float b, float t)
{
	const float clampedT{ Clamp(t,0,1) };
	return a + (b - a) * clampedT;
}

float MathUtils::Clamp(float value, float min, float max)
{
	if (value < min)
	{
		return min;
	}
	if (value > max)
	{
		return max;
	}
	return value;
}

float MathUtils::Deg2Rad(float angle)
{
	return glm::radians(angle);
}

float MathUtils::Rad2Deg(float radian)
{
	return glm::degrees(radian);
}

float MathUtils::Sin(float angle)
{
	return std::sinf(angle);
}

float MathUtils::Cos(float angle)
{
	return std::cosf(angle);
}
