// Fair Exponential Smoothing with Small Alpha by Juha Reunanen, 2015

#ifndef FESSA_OPENCV_MAT
#define FESSA_OPENCV_MAT

#include "fessa_detail.hpp"
#include <opencv2/core/core.hpp>

namespace fessa
{
    // A mutating filter for OpenCV matrix (image) objects.
    // The filter is mutating for performance reasons.
    template <typename A = double, typename T = unsigned short>
    class MatFilter {
    public:
        MatFilter(A baseAlpha, int type = CV_64FC1)
            : baseAlpha(baseAlpha), nextAlpha(baseAlpha), type(type)
        {
            assert(type != 0);
        }

        // Process a new image x, and return the filtered image s
        const cv::Mat& filter(cv::InputArray x) {
            if (s.type() != type) {
                // the first call
                s.create(x.getMat().size(), type);
                s.setTo(0);
            }
            else if (s.size() != x.size()) {
                // start over
                nextAlpha = detail::Alpha<A, T>(baseAlpha);
                s.create(x.getMat().size(), type);
                s.setTo(0);
            }
            const A a = nextAlpha.getAlpha();
            cv::addWeighted(x, a, s, 1 - a, 0, s, type);
            nextAlpha = nextAlpha.getNext();
            return s;
        }
    private:
        A baseAlpha;
        detail::Alpha<A, T> nextAlpha;
        int type;
        cv::Mat s;
    };

    // Like MatFilter, but accepts a mask when filtering
    template <typename A = double>
    class MaskedMatFilter {
    public:
        MaskedMatFilter(A baseAlpha, int type = CV_64FC1)
            : baseAlpha(baseAlpha), type(type)
        {
            assert(type != 0);
        }

        // Process a new image x, and return the filtered image s
        const cv::Mat& filter(cv::InputArray x, cv::InputArray mask) {
            if (x.size() != mask.size()) { // TODO: allow mask to be cv::noArray
                throw std::runtime_error("Invalid dimensions");
            }
            const bool startOver 
                = s.type() != type || s.size() != x.size()
                || a.type() != type || a.size() != x.size()
                || d.type() != type || d.size() != x.size();
            if (startOver) {
                a.create(x.getMat().size(), type);
                a.setTo(1);
                d.create(x.getMat().size(), type);
                d.setTo(0);
                s.create(x.getMat().size(), type);
                s.setTo(0);
            }

            // update d
            cv::add(d, a, d, mask);

            // update a
            cv::multiply(a, 1 - baseAlpha, temp);
            temp.copyTo(a, mask);

            // calculate alpha and 1 - alpha
            cv::divide(1.0, d, temp); // temp is now alpha
            cv::subtract(1.0, temp, temp2); // temp2 is now 1 - alpha

            // do the weighting
            cv::multiply(x, temp, temp, 1.0, type); // temp is now alpha * x
            cv::multiply(s, temp2, temp2); // temp2 is now (1 - alpha) * s

            // calculate the filtered result
            cv::add(temp, temp2, s, mask);
            return s;
        }
    private:
        A baseAlpha;
        int type;
        cv::Mat s;
        cv::Mat a;
        cv::Mat d;
        cv::Mat temp, temp2;
    };
}

#endif // FESSA_OPENCV_MAT