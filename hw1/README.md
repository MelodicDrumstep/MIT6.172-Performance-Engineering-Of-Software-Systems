这个 homework 很容易， 主要是学习调试工具和写个矩阵乘法加速。

## 调试工具

学了
+ Makefile
+ gdb
+ valgrind
+ Gcov
这个 Gcov 很有意思， 它可以检查是否有哪行代码没有执行过， 从而帮助程序员找 Bug。
用法： 编译选项加`-fprofile-arcs and -ftest-coverage`， 然后
`llvm-cov gcov testbed.c`
不过在调试完成之后记得发行版不要带这个编译选项， 会很慢的。

## 矩阵乘法加速

最后呢就是矩阵乘法加速， 这个真是 hpc 逃不掉的 Open problem！
来看看加速前的：

开 -O0:

```shell
Running matrix_multiply_run()...
Elapsed execution time: 3.079134 sec
```

加速前开个 -O3 ：

```shell
Running matrix_multiply_run()...
Elapsed execution time: 0.728962 sec
```

下面为了一步步看优化效果， 先把 -O3 关了， 开 -O0。

循环互换：（换成 ikj）

```c
  for (int i = 0; i < A->rows; i++) 
  {
    for (int k = 0; k < A->cols; k++) 
    {
      for (int j = 0; j < B->cols; j++) 
      {
        C->values[i][j] += A->values[i][k] * B->values[k][j];
      }
    }
  }
```

```shell
Running matrix_multiply_run()...
Elapsed execution time: 1.949702 sec
```

存个临时变量：

```c
  for (int i = 0; i < A->rows; i++) 
  {
    for (int k = 0; k < A->cols; k++) 
    {
      int temp = A->values[i][k];
      for (int j = 0; j < B->cols; j++) 
      {
        C->values[i][j] += temp * B->values[k][j];
      }
    }
  }
```

```shell
Running matrix_multiply_run()...
Elapsed execution time: 1.679166 sec
```

写个分块：

```c
int block_size = 64;

for(int i = 0; i < A -> rows; i += block_size)
{
    for(int k = 0; k < A -> cols; k += block_size)
    {
    for(int j = 0; j < B -> cols; j += block_size)
    {
        for(int ii = 0; ii < block_size && i + ii < A -> rows; ii++)
        {
        for(int kk = 0; kk < block_size && k + kk < A -> cols; kk++)
        {
            for(int jj = 0; jj < block_size && j + jj < B -> cols; jj++)
            {
            C -> values[i + ii][j + jj] += A -> values[i + ii][k + kk] * B -> values[k + kk][j + jj];
            }
        }
        }
    }
    }
} 
```

这个 blocksize 太拉垮了。。。 

```shell
Running matrix_multiply_run()...
Elapsed execution time: 3.022591 sec
```

另外我也打算， 当矩阵足够小的时候退化成普通算法

试了下 blocksize 256:

```shell
Running matrix_multiply_run()...
Elapsed execution time: 2.869121 sec
```

感觉都效果不咋样啊。。。

用 valgrind cachegrind 看下吧。。

``` shell
root@LAPTOP-SV0KL6CH:/mnt/d/MIT6.172/hw1/matrix-multiply# valgrind --tool=cachegrind ./matrix_multipl
y
==90879== Cachegrind, a cache and branch-prediction profiler
==90879== Copyright (C) 2002-2017, and GNU GPL'd, by Nicholas Nethercote et al.
==90879== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==90879== Command: ./matrix_multiply
==90879== 
--90879-- warning: L3 cache found, using its data for the LL simulation.
Setup
==90879== brk segment overflow in thread #1: can't grow to 0x485b000
==90879== (see section Limitations in user manual)
==90879== NOTE: further instances of this message will not be shown
Running matrix_multiply_run()...
Elapsed execution time: 110.261226 sec
==90879== 
==90879== I   refs:      49,279,863,250
==90879== I1  misses:             1,585
==90879== LLi misses:             1,540
==90879== I1  miss rate:           0.00%
==90879== LLi miss rate:           0.00%
==90879== 
==90879== D   refs:      36,152,824,802  (32,111,552,319 rd   + 4,041,272,483 wr)
==90879== D1  misses:        66,868,040  (    66,550,054 rd   +       317,986 wr)
==90879== LLd misses:           190,633  (         1,346 rd   +       189,287 wr)
==90879== D1  miss rate:            0.2% (           0.2%     +           0.0%  )
==90879== LLd miss rate:            0.0% (           0.0%     +           0.0%  )
==90879== 
==90879== LL refs:           66,869,625  (    66,551,639 rd   +       317,986 wr)
==90879== LL misses:            192,173  (         2,886 rd   +       189,287 wr)
==90879== LL miss rate:             0.0% (           0.0%     +           0.0%  )
```

这看上去 cache hit 这么多啊？？ 看来程序瓶颈不在 cache 上面？

不过刚才试了下现在开 -O3, 确实快多了：

```shell
Running matrix_multiply_run()...
Elapsed execution time: 0.924603 sec
```

不说了， 上 openmp!

```c
 if(A -> rows * B -> cols > 512 * 512)
  {
    #pragma omp parallel for
    for(int i = 0; i < A -> rows; i += block_size)
    {
      for(int k = 0; k < A -> cols; k += block_size)
      {
        for(int j = 0; j < B -> cols; j += block_size)
        {
          for(int ii = 0; ii < block_size && i + ii < A -> rows; ii++)
          {
            for(int kk = 0; kk < block_size && k + kk < A -> cols; kk++)
            {
              for(int jj = 0; jj < block_size && j + jj < B -> cols; jj++)
              {
                C -> values[i + ii][j + jj] += A -> values[i + ii][k + kk] * B -> values[k + kk][j + jj];
              }
            }
          }
        }
      }
    }
  }

  else
  {
    #pragma omp parallel for
    for (int i = 0; i < A->rows; i++) 
    {
      for (int k = 0; k < A->cols; k++) 
      {
        int temp = A->values[i][k];
        for (int j = 0; j < B->cols; j++) 
        {
          C->values[i][j] += temp * B->values[k][j];
        }
      }
    }
  }
```

```shell
Running matrix_multiply_run()...
Elapsed execution time: 0.449746 sec
```

这是 -O0 哦！ 确实很快了！

再来一波向量化！

跑下不分块的版本：

```c
else
  {
    int num_element = B -> cols;
    int max_index = num_element / 8 * 8;

    #pragma omp parallel for
    for (int i = 0; i < A->rows; i++) 
    {
      for (int k = 0; k < A->cols; k++) 
      {
        int temp = A -> values[i][k];
        __m256i temp_vec = _mm256_set1_epi32(temp);
        for (int j = 0; j < max_index; j += 8) 
        {
          __m256i B_vec = _mm256_loadu_si256((__m256i*)(B->values[k] + j));
          __m256i C_vec = _mm256_loadu_si256((__m256i*)(C->values[i] + j));
          C_vec = _mm256_add_epi32(C_vec, _mm256_mul_epi32(B_vec, temp_vec));
          _mm256_storeu_si256((__m256i*)(C->values[i] + j), C_vec);
        }
        for(int j = max_index; j < num_element; j++)
        {
          C->values[i][j] += temp * B->values[k][j];
        }
      
      }
    }
  }
```

```shell
Running matrix_multiply_run()...
Elapsed execution time: 0.085565 sec
```

再来波循环展开

```c

for (int j = 0; j < max_index; j += 16) 
{
    B_vec = _mm256_loadu_si256((__m256i*)(B->values[k] + j));
    C_vec = _mm256_loadu_si256((__m256i*)(C->values[i] + j));
    C_vec = _mm256_add_epi32(C_vec, _mm256_mul_epi32(B_vec, temp_vec));
    _mm256_storeu_si256((__m256i*)(C->values[i] + j), C_vec);
    B_vec = _mm256_loadu_si256((__m256i*)(B->values[k] + j + 8));
    C_vec = _mm256_loadu_si256((__m256i*)(C->values[i] + j + 8));
    C_vec = _mm256_add_epi32(C_vec, _mm256_mul_epi32(B_vec, temp_vec));
    _mm256_storeu_si256((__m256i*)(C->values[i] + j + 8), C_vec);
}
```

这样效果不太好， 估计是 j + 8 算了多次的原因

```shell
Running matrix_multiply_run()...
Elapsed execution time: 0.100689 sec
```
改成这样：

```c
for (int j = 0; j < max_index;) 
{
    B_vec = _mm256_loadu_si256((__m256i*)(B->values[k] + j));
    C_vec = _mm256_loadu_si256((__m256i*)(C->values[i] + j));
    C_vec = _mm256_add_epi32(C_vec, _mm256_mul_epi32(B_vec, temp_vec));
    _mm256_storeu_si256((__m256i*)(C->values[i] + j), C_vec);
    j += 8;

    B_vec = _mm256_loadu_si256((__m256i*)(B->values[k] + j));
    C_vec = _mm256_loadu_si256((__m256i*)(C->values[i] + j));
    C_vec = _mm256_add_epi32(C_vec, _mm256_mul_epi32(B_vec, temp_vec));
    _mm256_storeu_si256((__m256i*)(C->values[i] + j), C_vec);
    j += 8;
}
```

现在很不错了， 继续这样展开几层试试

```shell
Running matrix_multiply_run()...
Elapsed execution time: 0.079956 sec
```

多展开一层反而效果不太好了：

```shell
Running matrix_multiply_run()...
Elapsed execution time: 0.082949 sec
```

那就用展开一层的。

现在再开一下 -O3 看看:

```shell
Running matrix_multiply_run()...
Elapsed execution time: 0.035096 sec
```

OKay, 现在可以很快很快了。

接下来看看分块这边的情况。

突然想到分块这边也可以存临时变量简单优化下：

```c
int temp = A -> values[i + ii][k + kk];
for(int jj = 0; jj < block_size && j + jj < B -> cols; jj++)
{
    C -> values[i + ii][j + jj] += temp * B -> values[k + kk][j + jj];
}
```


```shell
Running matrix_multiply_run()...
Elapsed execution time: 0.368875 sec
```

OK, 现在上 AVX：

写成了这样：

```c
#pragma omp parallel for
for(int i = 0; i < A -> rows; i += block_size)
{
    for(int k = 0; k < A -> cols; k += block_size)
    {
    for(int j = 0; j < B -> cols; j += block_size)
    {
        for(int ii = 0; ii < block_size && i + ii < A -> rows; ii++)
        {
        for(int kk = 0; kk < block_size && k + kk < A -> cols; kk++)
        {
            int num_element = block_size < B -> cols - j ? block_size : B -> cols - j;
            int max_index = num_element / 8 * 8;

            int temp = A -> values[i + ii][k + kk];
            __m256i temp_vec = _mm256_set1_epi32(temp);
            __m256i B_vec;
            __m256i C_vec;

            for(int jj = 0; jj < block_size && j + jj < B -> cols; jj += 8)
            {
            B_vec = _mm256_loadu_si256((__m256i*)(B->values[k + kk] + j + jj));
            C_vec = _mm256_loadu_si256((__m256i*)(C->values[i + ii] + j + jj));
            C_vec = _mm256_add_epi32(C_vec, _mm256_mullo_epi32(temp_vec, B_vec));
            _mm256_storeu_si256((__m256i*)(C->values[i + ii] + j + jj), C_vec);
            }
            for(int jj = max_index; jj < num_element; jj++)
            {
            C -> values[i + ii][j + jj] += temp * B -> values[k + kk][j + jj];
            }
        }
        }
    }
    }
}
```

看看效果：

```shell
Running matrix_multiply_run()...
Elapsed execution time: 0.324136 sec
```

开个 -O3：

```shell
Running matrix_multiply_run()...
Elapsed execution time: 0.080912 sec
```

所以针对 N = 1000 * 1000 的矩阵， 最快的是不分块 + 循环互换 + avx + 循环展开 + openmp。

也就是这个

```c
int num_element = B -> cols;
int max_index = num_element / 16 * 16;

#pragma omp parallel for
for (int i = 0; i < A->rows; i++) 
{
    for (int k = 0; k < A->cols; k++) 
    {
    int temp = A -> values[i][k];
    __m256i temp_vec = _mm256_set1_epi32(temp);
    __m256i B_vec;
    __m256i C_vec;
    for (int j = 0; j < max_index;) 
    {
        B_vec = _mm256_loadu_si256((__m256i*)(B->values[k] + j));
        C_vec = _mm256_loadu_si256((__m256i*)(C->values[i] + j));
        C_vec = _mm256_add_epi32(C_vec, _mm256_mul_epi32(B_vec, temp_vec));
        _mm256_storeu_si256((__m256i*)(C->values[i] + j), C_vec);
        j += 8;

        B_vec = _mm256_loadu_si256((__m256i*)(B->values[k] + j));
        C_vec = _mm256_loadu_si256((__m256i*)(C->values[i] + j));
        C_vec = _mm256_add_epi32(C_vec, _mm256_mul_epi32(B_vec, temp_vec));
        _mm256_storeu_si256((__m256i*)(C->values[i] + j), C_vec);
        j += 8;
    }
    for(int j = max_index; j < num_element; j++)
    {
        C->values[i][j] += temp * B->values[k][j];
    }
    
    }
}
```


来对比一下：


```shell
最初：
Elapsed execution time: 3.079134 sec
最终：
Elapsed execution time: 0.035096 sec
优化倍数：87.7 倍！！！
```

性能调优真好玩 XD