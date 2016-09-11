// Fair Exponential Smoothing with Small Alpha by Juha Reunanen, 2015

#ifndef FESSA_ALPHA
#define FESSA_ALPHA

#include <limits>
#include <assert.h>

namespace fessa
{
    namespace detail
    {
        // A is for the calculations; it must be floating-point
        // T is for time; essentially integer, need not be very wide
        template <typename A = double, typename T = unsigned short>
        class Alpha {
        public:
            Alpha(A baseAlpha, T t = 0)
                : baseAlpha(baseAlpha), t(t), a(1), d(0)
            {
                assert(baseAlpha > 0 && baseAlpha <= 1);
                assert(t >= 0 && t < std::numeric_limits<T>::max());
                for (T tau = 0; tau <= t; ++tau) {
                    d += a;
                    a = a * (1 - baseAlpha);
                }
            }

            Alpha getNext() const {
                if (t < std::numeric_limits<A>::max()) {
                    return Alpha(baseAlpha, t + 1, a * (1 - baseAlpha), d + a);
                }
                else {
                    return *this;
                }
            }

            A getAlpha() const { return 1 / d; }

            A getBaseAlpha() const { return baseAlpha; }

        private:
            Alpha(A baseAlpha, T t, A a, A d)
                : baseAlpha(baseAlpha), t(t), a(a), d(d) {}
            A baseAlpha;
            T t;
            A a;
            A d;
        };
    }
}

#endif // FESSA_ALPHA