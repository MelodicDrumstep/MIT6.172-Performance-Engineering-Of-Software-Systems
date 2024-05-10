#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int64_t fib(int64_t n)
{
    if(n < 2)
    {
        return n;
    }
    else
    {
        int64_t x, y;
        #pragma omp task shared(x, n)
        x = fib(n - 1);
        #pragma omp task shared(y, n)
        y = fib(n - 2);
        #pragma omp taskwait
        return x + y;
    }
}