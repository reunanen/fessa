// Fair Exponential Smoothing with Small Alpha by Juha Reunanen, 2015

#ifndef FESSA_SCALAR
#define FESSA_SCALAR

#include "fessa_detail.hpp"

namespace fessa
{
    // An immutable filter for scalar values.
    template <typename A = double, typename T = unsigned short>
    class ScalarFilter {
    public:
        ScalarFilter(A baseAlpha) : nextAlpha(baseAlpha), s(0) {}
        ScalarFilter filter(A x) const { 
            const A a = nextAlpha.getAlpha();
            const A sNew = a * x + (1 - a) * s;
            return ScalarFilter(nextAlpha.getNext(), sNew);
        }
        A getS() const {
            return s;
        }        
    private:
        ScalarFilter(detail::Alpha<A, T> nextAlpha, A s)
            : nextAlpha(nextAlpha), s(s) {}

        detail::Alpha<A, T> nextAlpha;
        A s;
    };
}

#endif // FESSA_SCALAR