// Fair Exponential Smoothing with Small Alpha by Juha Reunanen, 2015

#include "../fessa_opencv_mat.hpp"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(opencv_mat)
{
    typedef float T;
    const auto type = CV_32FC1;

    fessa::MatFilter<> filter(0.001, type);
    cv::Mat m(2, 2, type);

    m.setTo(1);
    const auto filtered1 = filter.filter(m);
    BOOST_REQUIRE_EQUAL(filtered1.type(), m.type());
    BOOST_REQUIRE_EQUAL(filtered1.rows, m.rows);
    BOOST_REQUIRE_EQUAL(filtered1.at<T>(0, 0), 1);

    m.setTo(2);
    const auto filtered2 = filter.filter(m);
    BOOST_REQUIRE_GT(filtered2.at<T>(0, 0), 1.5);
    BOOST_REQUIRE_LT(filtered2.at<T>(0, 0), 1.501);

    m.setTo(3);
    const auto filtered3 = filter.filter(m);
    BOOST_REQUIRE_GT(filtered3.at<T>(0, 0), 2.0);
    BOOST_REQUIRE_LT(filtered3.at<T>(0, 0), 2.001);
}
