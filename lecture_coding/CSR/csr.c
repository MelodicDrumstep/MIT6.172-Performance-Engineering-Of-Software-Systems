typedef struct 
{
    int n, nnz;
    int * rows;
    int * cols;
    double * vals;
}CSR;

void spmv(CSR * A, double * x, double * y)
{
    for(int i = 0; i < A -> n; i++)
    {
        y[i] = 0;
        for(int j = A -> rows[i]; j < A -> rows[i + 1]; j++)
        {
            y[i] += A -> vals[j] * x[A -> cols[j]];
        }
    }
}
