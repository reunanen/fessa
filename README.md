# Fair Exponential Smoothing with Small Alpha

Have you ever hesitated to set a small *α* in [exponential smoothing](https://en.wikipedia.org/wiki/Exponential_smoothing), because the first raw data sample *x*(0) may then have such a huge impact on the filtered signal *s*(*t*) for small values of *t*?

Yeah, thought so – but still wrote this code, and the [related paper](https://github.com/reunanen/fessa/blob/master/fessa.pdf).