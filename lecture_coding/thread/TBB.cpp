#include <tbb/task.h>

class FibTask : public task
{
public:
    const int64_t n;
    int64_t * const sum;
    FibTask(int64_t n_, int64_t * sum) : n(n_), sum(sum) {}

    task * execute()
    {
        if(n < 2)
        {
            *sum = n;
        }
        else
        {
            int64_t x, y;
            FibTask & a = * new( allocate_child()) FibTask(n-1, &x);
        }
    }
}