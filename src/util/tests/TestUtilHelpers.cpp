#define BOOST_TEST_MODULE OpengGL wrapper tests
#include <MathHelpers.hpp>
#include <boost/test/included/unit_test.hpp>
using namespace dream::helpers;

BOOST_AUTO_TEST_CASE(TestMathHelpers)
{
	for (int i = 1; i <= 20; i++)
	{
		float val = std::pow(i / 20, 3);
		BOOST_CHECK_CLOSE_FRACTION(
			val,
			dream::helpers::gammaToLinear(dream::helpers::linearToGamma(val)),
			1e-6);
		BOOST_CHECK_CLOSE_FRACTION(
			val,
			dream::helpers::linearToGamma(dream::helpers::gammaToLinear(val)),
			1e-6);
	}
}
