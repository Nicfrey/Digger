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

float MathUtils::Atan2(float y, float x)
{
	return std::atan2f(y, x);
}

float MathUtils::Abs(float value)
{
	return std::abs(value);
}

float MathUtils::Cross2D(const glm::vec2& a, const glm::vec2& b)
{
	return a.x * b.y - a.y * b.x;
}

float MathUtils::Dot2D(const glm::vec2& a, const glm::vec2& b)
{
	return a.x * b.x + a.y * b.y;
}

float MathUtils::Pow(float value, int exponent)
{
	return std::powf(value, static_cast<float>(exponent));
}

float MathUtils::Sqrt(float x)
{
	return std::sqrtf(x);
}

