#pragma once
#include <glm/glm.hpp>

namespace MathUtils
{
	/**
	 * \brief Do a linear interpolation between a and b
	 * \param a the first value
	 * \param b the second value
	 * \param t the percentage of the interpolation (between 0 and 1)
	 * \return the interpolated value
	 */
	float Lerp(float a, float b, float t);
	float Clamp(float value, float min, float max);
	float Deg2Rad(float angle);
	float Rad2Deg(float radian);
	float Sin(float angle);
	float Cos(float angle);
}
