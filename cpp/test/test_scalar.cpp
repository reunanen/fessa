// Fair Exponential Smoothing with Small Alpha by Juha Reunanen, 2015

#include "../fessa_scalar.hpp"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(scalar)
{
    const fessa::ScalarFilter<> filter(0.001);

    const auto filter1 = filter.filter(1);
    BOOST_REQUIRE_EQUAL(filter1.getS(), 1.0);

    const auto filter2 = filter1.filter(2);
    BOOST_REQUIRE_GT(filter2.getS(), 1.5);
    BOOST_REQUIRE_LT(filter2.getS(), 1.501);

    const auto filter3 = filter1.filter(3);
    BOOST_REQUIRE_GT(filter3.getS(), 2.0);
    BOOST_REQUIRE_LT(filter3.getS(), 2.001);
}
