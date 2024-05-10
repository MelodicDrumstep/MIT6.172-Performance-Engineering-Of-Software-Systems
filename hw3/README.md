这个 hw 主要是做 SIMD 向量化。 

## 编译阶段的向量化

首先来看看编译器会怎样自动做向量化：

对于这个简单的程序

```c
#define SIZE (1L << 16)

void test(uint8_t * a,  uint8_t * b) {
  uint64_t i;

  for (i = 0; i < SIZE; i++) {
    a[i] += b[i];
  }
}
```

可以进去看眼汇编：

```asm
.LBB0_3:                                # =>This Inner Loop Header: Depth=1
	#DEBUG_VALUE: test:a <- $rdi
	#DEBUG_VALUE: test:b <- $rsi
	#DEBUG_VALUE: test:i <- 0
	.loc	0 13 13 is_stmt 1               # example1.c:13:13
	movdqu	(%rsi,%rax), %xmm0
	movdqu	16(%rsi,%rax), %xmm1
	.loc	0 13 10 is_stmt 0               # example1.c:13:10
	movdqu	(%rdi,%rax), %xmm2
	paddb	%xmm0, %xmm2
	movdqu	16(%rdi,%rax), %xmm0
	paddb	%xmm1, %xmm0
	movdqu	32(%rdi,%rax), %xmm1
	movdqu	48(%rdi,%rax), %xmm3
	movdqu	%xmm2, (%rdi,%rax)
	movdqu	%xmm0, 16(%rdi,%rax)
	.loc	0 13 13                         # example1.c:13:13
	movdqu	32(%rsi,%rax), %xmm0
	.loc	0 13 10                         # example1.c:13:10
	paddb	%xmm1, %xmm0
	.loc	0 13 13                         # example1.c:13:13
	movdqu	48(%rsi,%rax), %xmm1
	.loc	0 13 10                         # example1.c:13:10
	paddb	%xmm3, %xmm1
	movdqu	%xmm0, 32(%rdi,%rax)
	movdqu	%xmm1, 48(%rdi,%rax)
```

可以看到编译器确实用向量寄存器做了向量化处理。

这里编译器实际上是这么做的：我先看看你的 array a 和 array b 内存上也没有重叠， 如果有， 那我不向量化，jmp 到一个非向量化的函数版本， 如果没有， 我就做向量化， jmp 到一个向量化的函数版本。

但是如果我已经确保了它们内存不会重叠， 我怎样告诉编译器这个事实，从而不要让它产生冗余代码呢？ 可以用 C 支持的 restrict 特性！

```c
void test(uint8_t * restrict a,  uint8_t * restrict b) {
  uint64_t i;

  for (i = 0; i < SIZE; i++) {
    a[i] += b[i];
  }
}
```

这个时候再进去看汇编， 可以发现直接上向量化了：

```asm
.Lfunc_begin0:
	.file	0 "/mnt/d/MIT6.172/hw3/recitation3" "example1.c" md5 0x094e920f5f12746d7cc7ddf73cde262d
	.loc	0 9 0                           # example1.c:9:0
	.cfi_startproc
# %bb.0:
	#DEBUG_VALUE: test:a <- $rdi
	#DEBUG_VALUE: test:b <- $rsi
	xorl	%eax, %eax
.Ltmp0:
	#DEBUG_VALUE: test:i <- 0
	.p2align	4, 0x90
.LBB0_1:                                # =>This Inner Loop Header: Depth=1
	#DEBUG_VALUE: test:a <- $rdi
	#DEBUG_VALUE: test:b <- $rsi
	#DEBUG_VALUE: test:i <- 0
	.loc	0 13 13 prologue_end            # example1.c:13:13
	movdqu	(%rsi,%rax), %xmm0
	movdqu	16(%rsi,%rax), %xmm1
	.loc	0 13 10 is_stmt 0               # example1.c:13:10
	movdqu	(%rdi,%rax), %xmm2
	paddb	%xmm0, %xmm2
	movdqu	16(%rdi,%rax), %xmm0
	paddb	%xmm1, %xmm0
	movdqu	32(%rdi,%rax), %xmm1
	movdqu	48(%rdi,%rax), %xmm3
	movdqu	%xmm2, (%rdi,%rax)
	movdqu	%xmm0, 16(%rdi,%rax)
	.loc	0 13 13                         # example1.c:13:13
	movdqu	32(%rsi,%rax), %xmm0
	.loc	0 13 10                         # example1.c:13:10
	paddb	%xmm1, %xmm0
	.loc	0 13 13                         # example1.c:13:13
	movdqu	48(%rsi,%rax), %xmm1
	.loc	0 13 10                         # example1.c:13:10
	paddb	%xmm3, %xmm1
	movdqu	%xmm0, 32(%rdi,%rax)
	movdqu	%xmm1, 48(%rdi,%rax)
.Ltmp1:
	.loc	0 12 26 is_stmt 1               # example1.c:12:26
	addq	$64, %rax
	cmpq	$65536, %rax                    # imm = 0x10000
	jne	.LBB0_1
.Ltmp2:
# %bb.2:
	#DEBUG_VALUE: test:a <- $rdi
	#DEBUG_VALUE: test:b <- $rsi
	#DEBUG_VALUE: test:i <- 0
	.loc	0 15 1                          # example1.c:15:1
	retq
```

但是又有一个问题：这里是 128 位一算(这里用的 SSE)， 但是如果我不告诉编译器我是内存对齐的话， 编译器只会用非对齐版本的各种指令(比如 `movdqu` ). 这里如果我确保了我的数组是 128 位对齐的， 那我可以用 `__builtin_assume_aligned` 关键字告诉编译器， 比如：

```c
void test(uint8_t * restrict a,  uint8_t * restrict b) {
  uint64_t i;

  a = __builtin_assume_aligned(a, 16);
  //16 个 byte 对齐
  b = __builtin_assume_aligned(b, 16);

  for (i = 0; i < SIZE; i++) {
    a[i] += b[i];
  }
}
```

这时再看汇编， 就已经用了对齐版本的指令了

```asm
.LBB0_1:                                # =>This Inner Loop Header: Depth=1
	#DEBUG_VALUE: test:a <- $rdi
	#DEBUG_VALUE: test:b <- $rsi
	#DEBUG_VALUE: test:i <- 0
	.loc	0 16 10 prologue_end            # example1.c:16:10
	movdqa	(%rdi,%rax), %xmm0
	movdqa	16(%rdi,%rax), %xmm1
	movdqa	32(%rdi,%rax), %xmm2
	movdqa	48(%rdi,%rax), %xmm3
	paddb	(%rsi,%rax), %xmm0
	paddb	16(%rsi,%rax), %xmm1
	movdqa	%xmm0, (%rdi,%rax)
	movdqa	%xmm1, 16(%rdi,%rax)
	paddb	32(%rsi,%rax), %xmm2
	paddb	48(%rsi,%rax), %xmm3
	movdqa	%xmm2, 32(%rdi,%rax)
	movdqa	%xmm3, 48(%rdi,%rax)
```

现在我希望拓展为 256 位一算， 即使用 AVX 拓展。 可以把编译命令换一下:

```shell
make clean; make ASSEMBLE=1 VECTORIZE=1 AVX2=1 example1.o 
```

这时可以看到， 已经是用 `ymm` 向量寄存器了:

```asm
.LBB0_1:                                # =>This Inner Loop Header: Depth=1
	#DEBUG_VALUE: test:a <- $rdi
	#DEBUG_VALUE: test:b <- $rsi
	#DEBUG_VALUE: test:i <- 0
	.loc	0 16 10 prologue_end            # example1.c:16:10
	vmovdqu	(%rdi,%rax), %ymm0
	vmovdqu	32(%rdi,%rax), %ymm1
	vmovdqu	64(%rdi,%rax), %ymm2
	vmovdqu	96(%rdi,%rax), %ymm3
	vpaddb	(%rsi,%rax), %ymm0, %ymm0
	vpaddb	32(%rsi,%rax), %ymm1, %ymm1
	vpaddb	64(%rsi,%rax), %ymm2, %ymm2
	vpaddb	96(%rsi,%rax), %ymm3, %ymm3
	vmovdqu	%ymm0, (%rdi,%rax)
	vmovdqu	%ymm1, 32(%rdi,%rax)
	vmovdqu	%ymm2, 64(%rdi,%rax)
	vmovdqu	%ymm3, 96(%rdi,%rax)
```

不过这里需要 32 位对齐， 所以改一下 `__builtin_assume_aligned`

```c
  a = __builtin_assume_aligned(a, 32);
  b = __builtin_assume_aligned(b, 32);
```

再来看一下：

```asm
.LBB0_1:                                # =>This Inner Loop Header: Depth=1
	#DEBUG_VALUE: test:a <- $rdi
	#DEBUG_VALUE: test:b <- $rsi
	#DEBUG_VALUE: test:i <- 0
	.loc	0 16 10 prologue_end            # example1.c:16:10
	vmovdqa	(%rdi,%rax), %ymm0
	vmovdqa	32(%rdi,%rax), %ymm1
	vmovdqa	64(%rdi,%rax), %ymm2
	vmovdqa	96(%rdi,%rax), %ymm3
	vpaddb	(%rsi,%rax), %ymm0, %ymm0
	vpaddb	32(%rsi,%rax), %ymm1, %ymm1
	vpaddb	64(%rsi,%rax), %ymm2, %ymm2
	vpaddb	96(%rsi,%rax), %ymm3, %ymm3
	vmovdqa	%ymm0, (%rdi,%rax)
	vmovdqa	%ymm1, 32(%rdi,%rax)
	vmovdqa	%ymm2, 64(%rdi,%rax)
	vmovdqa	%ymm3, 96(%rdi,%rax)
```

现在再来看另一个例子:

```c
void test(uint8_t * restrict a, uint8_t * restrict b) {
  uint64_t i;

  uint8_t * x = __builtin_assume_aligned(a, 16);
  uint8_t * y = __builtin_assume_aligned(b, 16);

  for (i = 0; i < SIZE; i++) {
    /* max() */
    if (y[i] > x[i]) x[i] = y[i];
  }
}
```

现在进汇编可以发现没有完全向量化。 但是如果我们改成这样:

```c

void test(uint8_t * restrict a, uint8_t * restrict b) {
  uint64_t i;

  uint8_t * x = __builtin_assume_aligned(a, 16);
  uint8_t * y = __builtin_assume_aligned(b, 16);

  for (i = 0; i < SIZE; i++) {
    /* max() */
    x[i] = (y[i] > x[i]) ? y[i] : x[i];
  }
}

```

那么编译器会很好地进行向量化。 为什么会出现这样的情况？
可能是因为， 我写成三元运算符的话， 编译器就一下子可以差别出所有的赋值操作都是独立的， 没有数据依赖性， 也可以不使用条件分支来进行操作。 

接下来再来看一个例子:

```c
void test(uint8_t * restrict a, uint8_t * restrict b) {
  uint64_t i;

  for (i = 0; i < SIZE; i++) {
    a[i] = b[i + 1];
  }
}
```

进去看汇编可以发现根本没有向量化。 实际上这里向量化了也不会更快， 因为 $a$ 和 $b$ 的操作是不对齐的。

再来看一个例子:

```c
double test(double * restrict a) {
  size_t i;

  double *x = __builtin_assume_aligned(a, 16);

  double y = 0;

  for (i = 0; i < SIZE; i++) {
    y += x[i];
  }
  return y;
}
```

编译之后， 可以发现编译器没有完全向量化。这是因为浮点数的加法是没有结合律的， 所以编译器无法 reorder 这些浮点数加法运算。

如果我加一个 `-ffast-math` 编译选项， 允许浮点数加法的结合律的话， 这样编译器就可以向量化了:

```asm
.LBB0_1:                                # =>This Inner Loop Header: Depth=1
	#DEBUG_VALUE: test:a <- $rdi
	#DEBUG_VALUE: test:x <- $rdi
	#DEBUG_VALUE: test:y <- 0.000000e+00
	#DEBUG_VALUE: test:i <- 0
	.loc	0 18 7 prologue_end             # example4.c:18:7
	addpd	(%rdi,%rax,8), %xmm0
	addpd	16(%rdi,%rax,8), %xmm1
	addpd	32(%rdi,%rax,8), %xmm0
	addpd	48(%rdi,%rax,8), %xmm1
	addpd	64(%rdi,%rax,8), %xmm0
	addpd	80(%rdi,%rax,8), %xmm1
	addpd	96(%rdi,%rax,8), %xmm0
	addpd	112(%rdi,%rax,8), %xmm1
```


## 实验

最后是实验这个程序：

```c
for (i = 0; i < I; i++) {
    for (j = 0; j < N; j++) {
        C[j] = A[j] __OP__ B[j];
    }
}
```


首先是不加向量化编译选项：

```
Elapsed execution time: 0.022436 sec; N: 1024, I: 100000, __OP__: +, __TYPE__: uint32_t
```

然后是 SSE:

```
Elapsed execution time: 0.008753 sec; N: 1024, I: 100000, __OP__: +, __TYPE__: uint32_t
```

然后是 AVX2：

```
Elapsed execution time: 0.006480 sec; N: 1024, I: 100000, __OP__: +, __TYPE__: uint32_t
```

编译选项加这两个， 可以编译时输出信息，提示哪些循环被向量化了， 而哪些没有:

``` makefile
  CFLAGS += -Rpass=loop-vectorize -Rpass-missed=loop-vectorize
```

最后实验一下不同数组类型对于性能的影响:

```
Elapsed execution time: 0.013125 sec; N: 1024, I: 100000, __OP__: +, __TYPE__: uint64_t

Elapsed execution time: 0.010214 sec; N: 1024, I: 100000, __OP__: +, __TYPE__: uint32_t

Elapsed execution time: 0.004987 sec; N: 1024, I: 100000, __OP__: +, __TYPE__: uint16_t

Elapsed execution time: 0.001073 sec; N: 1024, I: 100000, __OP__: +, __TYPE__: uint8_t
```

可以发现数组的元素类型占位越小， 打包进入每个向量的元素个数就越多，自然速度也就越快。

接下来有一个新的问题： 如果循环次数不是编译时确定的怎么办？

比如， 如果 N 是这样得到的:

```c
    int N = atoi(argv[1]);
```

那编译器会怎么办呢？ 来试一下!

```
./loop 20000

With Vectorizaton:
Elapsed execution time: 0.719416 sec; N: 20000, I: 100000, __OP__: *, __TYPE__: uint64_t

Without Vectorization:
Elapsed execution time: 2.049693 sec; N: 20000, I: 100000, __OP__: *, __TYPE__: uint64_t
```

可以发现， 编译器仍然做了向量化， 只不过这时要特殊处理不是整数倍的剩余部分。(就跟手写向量化那样)

## 总结

这个 hw 自己写的地方很少， 主要是性能分析， 还是挺有意思的
