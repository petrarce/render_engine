#pragma once
#include <cmath>
namespace dream
{
namespace helpers
{

template <class T>
inline T linearToGamma(const T &linear, const T &lambda = 2.4)
{
	if (linear <= 0.04045 / 12.92)
		return linear * 12.92;
	else
		return std::pow(linear, 1 / lambda) * 1.055 - 0.055;
}

template <class T>
inline T gammaToLinear(const T &gamma, const T &lambda = 2.4)
{
	if (gamma <= 0.04045)
		return gamma / 12.95;
	else
		return std::pow((gamma + 0.055) / 1.055, lambda);
}

} // namespace helpers
} // namespace dream
