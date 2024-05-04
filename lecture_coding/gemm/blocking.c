
void gemm(float * A, float * B, float * C, int m, int n, int k)
{
    int s = 8;
    for(int i = 0 ; i < m; i += s)
    {
        for(int k = 0; k < n; k += s)
        {
            for(int j = 0; j < k; j += s)
            {
                for(int ii = 0; ii < s; ii++)
                {
                    for(int kk = 0; kk < s; kk++)
                    {
                        for(int jj = 0; jj < s; jj++)
                        {
                            C[(i + ii) * n + (k + kk)] += A[(i + ii) * n + (j + jj)] * B[(j + jj) * n + (k + kk)];
                        }
                    }
                }
            }
        }
    }
}