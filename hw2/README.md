这个 homework 教了我如何使用性能分析工具， 比如 linux Perf, cachegrind.

## Perf

用法：

`perf record <program_name> <program_arguments> ` + `perf report `。
（记得编译要开 -g）

## Cachegrind

valgrind 的 cachegrind 工具可以看 cache hit 情况和 branch hit 情况。

用法：

` valgrind --tool=cachegrind --branch-sim=yes <program_name> <program_arguments>`

这个是在运行时机器上跑模拟。输出大概这样：

```
Allocated array of size 10000000
Summing 100000000 random values...
Done. Value = 938895920
==98559== 
==98559== I   refs:      3,800,146,907
==98559== I1  misses:            1,364
==98559== LLi misses:            1,356
==98559== I1  miss rate:          0.00%
==98559== LLi miss rate:          0.00%
==98559== 
==98559== D   refs:      1,470,048,676  (950,034,776 rd   + 520,013,900 wr)
==98559== D1  misses:      100,506,110  ( 99,880,422 rd   +     625,688 wr)
==98559== LLd misses:       37,858,824  ( 37,233,172 rd   +     625,652 wr)
==98559== D1  miss rate:           6.8% (       10.5%     +         0.1%  )
==98559== LLd miss rate:           2.6% (        3.9%     +         0.1%  )
==98559== 
==98559== LL refs:         100,507,474  ( 99,881,786 rd   +     625,688 wr)
==98559== LL misses:        37,860,180  ( 37,234,528 rd   +     625,652 wr)
==98559== LL miss rate:            0.7% (        0.8%     +         0.1%  )
==98559== 
==98559== Branches:        210,025,841  (110,025,461 cond + 100,000,380 ind)
==98559== Mispredicts:           3,223  (      3,050 cond +         173 ind)
==98559== Mispred rate:            0.0% (        0.0%     +         0.0%   )
```

D1 指的是最低的 cache（比如 L1）， LL 指的是最高的 cache（比如 L3）

怎么查看自己的 CPU 情况？ 可以用 lscpu：

```
root@LAPTOP-SV0KL6CH:/mnt/d/MIT6.172/hw2/recitation# lscpu
Architecture:            x86_64
  CPU op-mode(s):        32-bit, 64-bit
  Address sizes:         46 bits physical, 48 bits virtual
  Byte Order:            Little Endian
CPU(s):                  20
  On-line CPU(s) list:   0-19
Vendor ID:               GenuineIntel
  Model name:            13th Gen Intel(R) Core(TM) i9-13900H
    CPU family:          6
    Model:               186
    Thread(s) per core:  2
    Core(s) per socket:  10
    Socket(s):           1
    Stepping:            2
    BogoMIPS:            5990.39
    Flags:               fpu vme de pse tsc msr pae mce cx8 apic sep mtrr p
                         ge mca cmov pat pse36 clflush mmx fxsr sse sse2 ss
                          ht syscall nx pdpe1gb rdtscp lm constant_tsc rep_
                         good nopl xtopology tsc_reliable nonstop_tsc cpuid
                          pni pclmulqdq vmx ssse3 fma cx16 sse4_1 sse4_2 x2
                         apic movbe popcnt tsc_deadline_timer aes xsave avx
                          f16c rdrand hypervisor lahf_lm abm 3dnowprefetch 
                         ssbd ibrs ibpb stibp ibrs_enhanced tpr_shadow vnmi
                          ept vpid ept_ad fsgsbase tsc_adjust bmi1 avx2 sme
                         p bmi2 erms invpcid rdseed adx smap clflushopt clw
                         b sha_ni xsaveopt xsavec xgetbv1 xsaves avx_vnni u
                         mip waitpkg gfni vaes vpclmulqdq rdpid movdiri mov
                         dir64b fsrm md_clear serialize flush_l1d arch_capa
                         bilities
Virtualization features: 
  Virtualization:        VT-x
  Hypervisor vendor:     Microsoft
  Virtualization type:   full
Caches (sum of all):     
  L1d:                   480 KiB (10 instances)
  L1i:                   320 KiB (10 instances)
  L2:                    12.5 MiB (10 instances)
  L3:                    24 MiB (1 instance)
Vulnerabilities:         
  Gather data sampling:  Not affected
  Itlb multihit:         Not affected
  L1tf:                  Not affected
  Mds:                   Not affected
  Meltdown:              Not affected
  Mmio stale data:       Not affected
  Retbleed:              Mitigation; Enhanced IBRS
  Spec rstack overflow:  Not affected
  Spec store bypass:     Mitigation; Speculative Store Bypass disabled via 
                         prctl and seccomp
  Spectre v1:            Mitigation; usercopy/swapgs barriers and __user po
                         inter sanitization
  Spectre v2:            Mitigation; Enhanced IBRS, IBPB conditional, RSB f
                         illing, PBRSB-eIBRS SW sequence
  Srbds:                 Not affected
  Tsx async abort:       Not affected
```

下面就是学习如何性能调优啦！ 这次是优化一个 mergesort, 下面一步步来优化看

### inline

下面的测试参数都是 n = 100000, 重复 100 次。

把 copy, malloc, free 都 inline掉：

```
 --> test_correctness at line 217: PASS
sort_a          : Elapsed execution time: 0.011312 sec
sort_a repeated : Elapsed execution time: 0.011239 sec
sort_i          : Elapsed execution time: 0.011322 sec
```

效果不是很明显， 试试 inline 一下 merge:

```
 --> test_correctness at line 217: PASS
sort_a          : Elapsed execution time: 0.011103 sec
sort_a repeated : Elapsed execution time: 0.011124 sec
sort_i          : Elapsed execution time: 0.011053 sec
```

最后试一下把 sort 这个递归函数也 inline 看看：

```
 --> test_correctness at line 217: PASS
sort_a          : Elapsed execution time: 0.011220 sec
sort_a repeated : Elapsed execution time: 0.011049 sec
sort_i          : Elapsed execution time: 0.011104 sec
``` 

当我们试图 inline 一个递归函数的时候， 编译器只会把它展开基层， 之后还是递归。

函数内联虽然可以消除调用函数的开销（维护栈， 备份寄存器）， 但是也会增大代码体积， 破坏指令局部性。 比如， 一个函数在多处被调用， 如果内联的话就有多份函数内容的备份， 这样代码体积就大了， 更多的代码需要被加载到有限的指令缓存中， 减小了指令缓存命中率。

### 用指针代替数组操作

大概就是把 `&(A[p])` 换成 `A + p`, 这样就不用展开成 &(*(A + p))了。
但是实践中好像优化效果不是特别好：

```
 --> test_correctness at line 217: PASS
sort_a          : Elapsed execution time: 0.011326 sec
sort_a repeated : Elapsed execution time: 0.011270 sec
sort_i          : Elapsed execution time: 0.011324 sec
sort_p          : Elapsed execution time: 0.011731 sec
```

为什么会负优化， 查了一下， 可能有这些原因：

```
不同的优化策略：虽然 A[i] 和 *(A + i) 在低级别是等价的，但编译器可能对这两种形式的代码应用了不同的优化策略。例如，编译器可能针对数组索引访问进行特定的循环向量化或其他优化，而对指针操作则没有这么做。
优化启用条件：某些优化，比如循环展开、预取等，可能在编译器看到 A[i] 形式时更容易触发。更改为 *(A + i) 可能在某种程度上改变了编译器对代码结构的解读，从而影响了优化效果。
```

### Coarsening

对于一个递归函数， 我们可以把 base condition 设宽一点， 然后对于小规模问题， 就不要继续递归了， 直接用其他算法解决。 因为规模很小的时候， 递归的开销就显得很大了。

比如这里， 我可以设置一个 BASE, 当递归到数组长度小于 BASE 的时候， 我就不要继续归并了， 直接上选择排序：

```c
#define NUM_BASE 8

void sort_c(data_t* A, int p, int r) 
{
  assert(A);
  //When the array is small enough, I do not want the
  //overhead of recursive function
  //Then I choose to implement selection sort on
  //these small arrays
  if(p + NUM_BASE > r)
  {
    //A[p, ... ,i - 1] are sorted 
    for(int i = p + 1; i <= r; i++)
    {
      data_t key = A[i];
      int j = i - 1;
      while(j >= p && A[j] > key)
      {
        A[j + 1] = A[j];
        j--;
      }
      A[j + 1] = key;
    }
  }
  else
  {
    int q = (p + r) / 2;
    sort_c(A, p, q);
    sort_c(A, q + 1, r);
    merge_c(A, p, q, r);
  }
}
```

这个优化的效果是真的很好， NUM_BASE = 8的时候：

```
 --> test_correctness at line 217: PASS
sort_a          : Elapsed execution time: 0.010950 sec
sort_a repeated : Elapsed execution time: 0.010942 sec
sort_i          : Elapsed execution time: 0.011098 sec
sort_p          : Elapsed execution time: 0.011128 sec
sort_c          : Elapsed execution time: 0.008022 sec
```

NUM_BASE = 16 的时候：

```
sort_a          : Elapsed execution time: 0.010947 sec
sort_a repeated : Elapsed execution time: 0.011069 sec
sort_i          : Elapsed execution time: 0.011107 sec
sort_p          : Elapsed execution time: 0.011096 sec
sort_c          : Elapsed execution time: 0.007837 sec
```

NUM_BASE = 32 的时候：

```
sort_a          : Elapsed execution time: 0.011135 sec
sort_a repeated : Elapsed execution time: 0.011154 sec
sort_i          : Elapsed execution time: 0.011251 sec
sort_p          : Elapsed execution time: 0.011403 sec
sort_c          : Elapsed execution time: 0.007939 sec
```

NUM_BASE = 128 的时候： 

```
sort_a          : Elapsed execution time: 0.011310 sec
sort_a repeated : Elapsed execution time: 0.011253 sec
sort_i          : Elapsed execution time: 0.011365 sec
sort_p          : Elapsed execution time: 0.011462 sec
sort_c          : Elapsed execution time: 0.009111 sec
```

所以这就是一个很重要的经验： 对于递归函数， 递归到规模很小的时候就不要继续递归了， 直接上一个简单算法解决掉。

### 减少空间开销

这里还有一个很重要的优化点：我每次 merge 的时候， 一定要把左边的数组和右边的数组都分配内存并复制吗？ 其实没必要的！我只复制左边的就够了， 右边的拿原来的数组就行。就像这样：

```c
static inline void merge_m(data_t* A, int p, int q, int r) 
{
  assert(A);
  assert(p <= q);
  assert((q + 1) <= r);
  int n1 = q - p + 1;
  int n2 = r - q;

  data_t* left = 0, * right = A + q + 1;
  mem_alloc(&left, n1);
  if (left == NULL || right == NULL) 
  {
    return;
  }

  copy_m(&(A[p]), left, n1);

  int i = 0;
  int j = 0;
  int k = p;

  while(i < n1 && j < n2)
  {
    if(left[i] <= *(right + j))
    {
      A[k] = left[i];
      ++i;
    }
    else
    {
      A[k] = *(right + j);
      ++j;
    }
    ++k;
  }
  while(i < n1)
  {
    A[k] = left[i];
    ++k;
    ++i;
  }
  mem_free(&left);
}
```

这样的优化也很大：

```
sort_a          : Elapsed execution time: 0.010880 sec
sort_a repeated : Elapsed execution time: 0.010974 sec
sort_i          : Elapsed execution time: 0.011064 sec
sort_p          : Elapsed execution time: 0.011121 sec
sort_c          : Elapsed execution time: 0.007818 sec
sort_m          : Elapsed execution time: 0.006437 sec
```

### 提前开好空间

最后一种优化思路是， 我现在不要每次临时开空间了， 我最初就开好， 每次复制直接用:

```c
void sort_f(data_t* A, int p, int r) 
{
  data_t * copy = 0;
  mem_alloc(&copy, r - p + 1);
  copy_f(&(A[p]), copy, r - p + 1);
  sort_f_helper(A, p, r, copy);
  mem_free(&copy);
}

void sort_f_helper(data_t* A, int p, int r, data_t* copy)
{
  assert(A);
  //When the array is small enough, I do not want the
  //overhead of recursive function
  //Then I choose to implement selection sort on
  //these small arrays
  if(p + NUM_BASE > r)
  {
    //A[p, ... ,i - 1] are sorted 
    for(int i = p + 1; i <= r; i++)
    {
      data_t key = A[i];
      int j = i - 1;
      while(j >= p && A[j] > key)
      {
        A[j + 1] = A[j];
        j--;
      }
      A[j + 1] = key;
    }
  }
  else
  {
    int q = (p + r) / 2;
    sort_f_helper(A, p, q, copy);
    sort_f_helper(A, q + 1, r, copy);
    merge_f(A, p, q, r, copy);
  }
}

// A merge routine. Merges the sub-arrays A [p..q] and A [q + 1..r].
// Uses two arrays 'left' and 'right' in the merge operation.
static inline void merge_f(data_t * A, int p, int q, int r, data_t * copy) 
{
  assert(A);
  assert(p <= q);
  assert((q + 1) <= r);
  int n1 = q - p + 1;
  int n2 = r - q;

  data_t * left = copy + p;
  data_t * right = A + q + 1;
  copy_f(A + p, left, n1);

  int i = 0;
  int j = 0;
  int k = p;

  while(i < n1 && j < n2)
  {
    data_t temp = *(right + j);
    if(left[i] <= temp)
    {
      A[k] = left[i];
      ++i;
    }
    else
    {
      A[k] = temp;
      ++j;
    }
    ++k;
  }
  while(i < n1)
  {
    A[k] = left[i];
    ++k;
    ++i;
  }
}
```

这个效果不是很明显， 跟上一个优化效果差不多

```
sort_a          : Elapsed execution time: 0.011170 sec
sort_a repeated : Elapsed execution time: 0.011206 sec
sort_i          : Elapsed execution time: 0.011361 sec
sort_p          : Elapsed execution time: 0.011383 sec
sort_c          : Elapsed execution time: 0.007901 sec
sort_m          : Elapsed execution time: 0.006626 sec
sort_f          : Elapsed execution time: 0.006683 sec
```

把数据开大一点看看(n = 1000000)：

```
sort_a          : Elapsed execution time: 0.129777 sec
sort_a repeated : Elapsed execution time: 0.128467 sec
sort_i          : Elapsed execution time: 0.127372 sec
sort_p          : Elapsed execution time: 0.128041 sec
sort_c          : Elapsed execution time: 0.094897 sec
sort_m          : Elapsed execution time: 0.078282 sec
sort_f          : Elapsed execution time: 0.080853 sec
```

n = 10000000:

```
sort_a          : Elapsed execution time: 1.475610 sec
sort_a repeated : Elapsed execution time: 1.467517 sec
sort_i          : Elapsed execution time: 1.505387 sec
sort_p          : Elapsed execution time: 1.491677 sec
sort_c          : Elapsed execution time: 1.172428 sec
sort_m          : Elapsed execution time: 0.959497 sec
sort_f          : Elapsed execution time: 0.974663 sec
```

所以还是倒数第二个优化最好。

## 总结

所以， 对于一个 mergesort, 我的优化方向有哪些？

+ 函数内联
+ 数组操作 -> 指针
+ coarsening 递归函数（递归到小规模的时候跑非递归算法）
+ 减少内存分配
+ 提前内存分配



