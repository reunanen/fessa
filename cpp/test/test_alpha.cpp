// Fair Exponential Smoothing with Small Alpha by Juha Reunanen, 2015

#include "../fessa_detail.hpp"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(alpha)
{
    typedef double A;
    const A baseAlpha = 0.001;

    // define a narrot unit for time, so that we can easily reach the maximum value
    typedef unsigned char T;

    const fessa::detail::Alpha<A, T> alpha0(baseAlpha);
    
    BOOST_REQUIRE_EQUAL(alpha0.getAlpha(), 1);

    const fessa::detail::Alpha<A, T> alpha1(baseAlpha, 1);

    BOOST_REQUIRE_EQUAL(alpha0.getNext().getAlpha(), alpha1.getAlpha());

    BOOST_REQUIRE_GT(alpha1.getAlpha(), 0.50025);
    BOOST_REQUIRE_LT(alpha1.getAlpha(), 0.500251);

    auto alphaRecursive = alpha1;
    auto alphaPrevious = alphaRecursive;

    int tMax = std::numeric_limits<T>::max() * 2;
    for (int t = 2; t < tMax; ++t) {

        // update the recursive alpha
        alphaRecursive = alphaRecursive.getNext();

        // basically alpha_fessa(t + 1) > alpha_fessa(t) for all t
        if (t < std::numeric_limits<T>::max()) {
            BOOST_REQUIRE_LT(alphaRecursive.getAlpha(), alphaPrevious.getAlpha());
        }
        // ... but in practice this is not necessary for large values of t
        else {
            BOOST_REQUIRE_EQUAL(alphaRecursive.getAlpha(), alphaRecursive.getAlpha());
        }

        // at any rate, the difference should get small when t increases
        BOOST_REQUIRE_LT(alphaPrevious.getAlpha() - alphaRecursive.getAlpha(), 1 / static_cast<A>(t + 1));

        if (t < std::numeric_limits<T>::max()) {
            // we can also calculate the alpha directly based on t (without recursion, that is)
            const fessa::detail::Alpha<A, T> alphaDirect(baseAlpha, t);
            BOOST_REQUIRE_CLOSE(alphaRecursive.getAlpha(), alphaDirect.getAlpha(), 1e-10);
        }

        // check that the latest sample always has more weight than the others on average
        const auto oneMinusAlpha = 1 - alphaRecursive.getAlpha();
        const auto averageWeightOfEarlierSamples = oneMinusAlpha / t;
        BOOST_REQUIRE_GT(alphaRecursive.getAlpha(), averageWeightOfEarlierSamples);
        BOOST_REQUIRE_GT(alphaRecursive.getAlpha(), baseAlpha);

        alphaPrevious = alphaRecursive;
    }
}
