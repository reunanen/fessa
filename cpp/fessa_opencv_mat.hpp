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
}

#endif // FESSA_OPENCV_MAT