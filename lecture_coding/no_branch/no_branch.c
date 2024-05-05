

int main()
{
    int x = 10;
    int y = 20;
//no branch swap:
    x = x ^ y;
    y = x ^ y;
    x = x ^ y;

//no branch min:
    int min = y ^ ((x ^ y) & -(x < y));

    //no branch modular when 0 \leq x, y \le n

    int n = 30;
    int z = x + y;
    int r = z - (n & -(z >= n));

    //round up to a power of 2:
    --n;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n |= n >> 32;
    ++n;

    return 0;
}

void merge(long * __restrict C, long * __restrict A, long * __restrict B, size_t na, size_t nb)
{
    while(na > 0 && nb > 0)
    {
        long cmp = (*A < *B);
        long min = *B ^ ((*A ^ *B) & -(cmp));
        *C = min;
        ++C;
        A += cmp;
        B += !cmp;
        na -= cmp;
        B -= !cmp
    }
    while(na > 0)
    {
        *C = *A;
        ++C;
        ++A;
        --na;
    }
    while(nb > 0)
    {
        *C = *B;
        ++C;
        ++B;
        --nb;
    }
}
