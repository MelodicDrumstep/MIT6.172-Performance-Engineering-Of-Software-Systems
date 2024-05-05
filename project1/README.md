这个 Project 是要优化一个 bit_array 的 rotate 函数的性能， 其他函数都大差不差。 
先来看看最初版的 rotate 函数是什么样：

原型是:

```c
// Rotates a subarray.
//
// bit_offset is the index of the start of the subarray
// bit_length is the length of the subarray, in bits
// bit_right_amount is the number of places to rotate the subarray right
//
// The subarray spans the half-open interval
// [bit_offset, bit_offset + bit_length)
// That is, the start is inclusive, but the end is exclusive.
//
// Note: bit_right_amount can be negative, in which case a left rotation is
// performed.
//
// Example:
// Let ba be a bit array containing the byte 0b10010110; then,
// bitarray_rotate(ba, 0, bitarray_get_bit_sz(ba), -1)
// left-rotates the entire bit array in place.  After the rotation, ba
// contains the byte 0b00101101.
//
// Example:
// Let ba be a bit array containing the byte 0b10010110; then,
// bitarray_rotate(ba, 2, 5, 2) rotates the third through seventh
// (inclusive) bits right two places.  After the rotation, ba contains the
// byte 0b10110100.
void bitarray_rotate(bitarray_t* const bitarray,
                     const size_t bit_offset,
                     const size_t bit_length,
                     const ssize_t bit_right_amount);
```


实现是:

```c
void bitarray_rotate(bitarray_t* const bitarray,
                     const size_t bit_offset,
                     const size_t bit_length,
                     const ssize_t bit_right_amount) {
  assert(bit_offset + bit_length <= bitarray->bit_sz);

  if (bit_length == 0) {
    return;
  }

  // Convert a rotate left or right to a left rotate only, and eliminate
  // multiple full rotations.
  bitarray_rotate_left(bitarray, bit_offset, bit_length,
                       modulo(-bit_right_amount, bit_length));
}

static void bitarray_rotate_left(bitarray_t* const bitarray,
                                 const size_t bit_offset,
                                 const size_t bit_length,
                                 const size_t bit_left_amount) {
  for (size_t i = 0; i < bit_left_amount; i++) {
    bitarray_rotate_left_one(bitarray, bit_offset, bit_length);
  }
}

static void bitarray_rotate_left_one(bitarray_t* const bitarray,
                                     const size_t bit_offset,
                                     const size_t bit_length) {
  // Grab the first bit in the range, shift everything left by one, and
  // then stick the first bit at the end.
  const bool first_bit = bitarray_get(bitarray, bit_offset);
  size_t i;
  for (i = bit_offset; i + 1 < bit_offset + bit_length; i++) {
    bitarray_set(bitarray, i, bitarray_get(bitarray, i + 1));
  }
  bitarray_set(bitarray, i, first_bit);
}
```


这确实有点太慢了。。。 居然是把 rotate n 拆成 n 次 rotate 1, 简直就是赶工期暴力实现 XD

来看看原版的速度：

```
root@LAPTOP-SV0KL6CH:/mnt/d/MIT6.172/project1/MIT6_172F18-project1/everybit# ./everybit -t tests/mytests 
Testing file tests/mytests.
Done testing file tests/mytests.
root@LAPTOP-SV0KL6CH:/mnt/d/MIT6.172/project1/MIT6_172F18-project1/everybit# ./everybit  -l
---- RESULTS ----
Tier 0 (≈0B) completed in 0.000000s
Tier 1 (≈0B) completed in 0.000000s
Tier 2 (≈1B) completed in 0.000000s
Tier 3 (≈1B) completed in 0.000000s
Tier 4 (≈2B) completed in 0.000000s
Tier 5 (≈4B) completed in 0.000001s
Tier 6 (≈6B) completed in 0.000002s
Tier 7 (≈11B) completed in 0.000005s
Tier 8 (≈18B) completed in 0.000014s
Tier 9 (≈29B) completed in 0.000036s
Tier 10 (≈47B) completed in 0.000095s
Tier 11 (≈76B) completed in 0.000258s
Tier 12 (≈123B) completed in 0.000632s
Tier 13 (≈199B) completed in 0.001664s
Tier 14 (≈323B) completed in 0.004381s
Tier 15 (≈522B) completed in 0.011811s
Tier 16 (≈845B) completed in 0.031272s
Tier 17 (≈1KB) completed in 0.079293s
Tier 18 (≈2KB) completed in 0.216904s
Tier 19 (≈3KB) completed in 0.568427s
Tier 20 (≈5KB) exceeded 1.00s cutoff with time 1.473616s
Succesfully completed tier: 19
---- END RESULTS ----
```

这个算法显然是不够好的， 在任务书的里也给了提示， 我据此想到一个新算法：

对于 rotate 这个操作， 我可以把它看成一个 swap.

比如， 对于 abcdefg（这里先拿字母举例子）

如果向右 rotate 1 个元素, 那就是 bcdefga, 可以看成 swap "a" and "bcdefg".

如果向右 rotate 3 个元素, 那就是 defgabc, 可以看成 swap "abc" and "defg".

如果向左 rotate 4 个元素, 那就是 efgabcd, 可以看成 swap "abcd" and "efg".

那我应该如何高效执行这个 swap 呢？ 要开额外空间吗？

其实不用的！ 可以发现这个性质: $(a^Rb^R)^R = ba$. 其中 R 表示 Reverse. 那我直接左边 Reverse, 右边 Reverse, 再全部 Reverse 不就行了吗？？ 原来这么简单！！

那我又怎么实现这个 reverse 呢？ 现在先考虑下这个问题

#### 方法1 ： 我写一个 for 循环

```c
n << = 1;
n = n | (x & 1);
x >> = 1;
```

注意到这里已经很贴心地为我们写好了一个 portable module 函数， 这样的话用它 -10 module 3 就是 2 而不是 C 自带的 -1了。

```c
static size_t modulo(const ssize_t n, const size_t m) 
{
  const ssize_t signed_m = (ssize_t)m;
  assert(signed_m > 0);
  const ssize_t result = ((n % signed_m) + signed_m) % signed_m;
  assert(result >= 0);
  return (size_t)result;
}
```

这里遇到的困难：

首先， reverse 的时候如果 length = 0 要特判。
然后要注意， 我应该用 `modulo(-bit_right_amount, bit_length)` 而不是 `modulo(bit_right_amount, bit_length)` 来求那个左右元素组的分界线， 这个一开始我给搞错了

看下我的实现：

```c
void bitarray_rotate(bitarray_t* const bitarray,
                     const size_t bit_offset,
                     const size_t bit_length,
                     const ssize_t bit_right_amount)
{
  if(bit_length == 0)
  {
    return;
  }

  assert(bit_offset + bit_length <= bitarray -> bit_sz);
  size_t bit_right_amount_after_module = modulo(-bit_right_amount, bit_length);
  bitarray_reverse(bitarray, bit_offset, bit_right_amount_after_module);
  bitarray_reverse(bitarray, bit_offset + bit_right_amount_after_module, bit_length - bit_right_amount_after_module);
  bitarray_reverse(bitarray, bit_offset, bit_length);
}
```

```c

void bitarray_reverse(bitarray_t* const bitarray, const size_t bit_offset, const size_t bit_length)
{
  if(bit_length == 0)
  {
    return;
  }

  assert(bit_offset + bit_length <= bitarray -> bit_sz);
  size_t start = bit_offset;
  size_t end = bit_offset + bit_length - 1;
  while(start < end)
  {
    size_t bit1 = bitarray_get(bitarray, start);
    size_t bit2 = bitarray_get(bitarray, end);
    bitarray_set(bitarray, start, bit2);
    bitarray_set(bitarray, end, bit1);
    start++;
    end--;
  }
}
```

来看下这时候我的速度：

```
---- RESULTS ----
Tier 0 (≈0B) completed in 0.000001s
Tier 1 (≈0B) completed in 0.000000s
Tier 2 (≈1B) completed in 0.000000s
Tier 3 (≈1B) completed in 0.000000s
Tier 4 (≈2B) completed in 0.000000s
Tier 5 (≈4B) completed in 0.000000s
Tier 6 (≈6B) completed in 0.000001s
Tier 7 (≈11B) completed in 0.000001s
Tier 8 (≈18B) completed in 0.000001s
Tier 9 (≈29B) completed in 0.000001s
Tier 10 (≈47B) completed in 0.000002s
Tier 11 (≈76B) completed in 0.000003s
Tier 12 (≈123B) completed in 0.000004s
Tier 13 (≈199B) completed in 0.000007s
Tier 14 (≈323B) completed in 0.000011s
Tier 15 (≈522B) completed in 0.000017s
Tier 16 (≈845B) completed in 0.000027s
Tier 17 (≈1KB) completed in 0.000044s
Tier 18 (≈2KB) completed in 0.000070s
Tier 19 (≈3KB) completed in 0.000138s
Tier 20 (≈5KB) completed in 0.000184s
Tier 21 (≈9KB) completed in 0.000296s
Tier 22 (≈14KB) completed in 0.000497s
Tier 23 (≈23KB) completed in 0.000786s
Tier 24 (≈38KB) completed in 0.001274s
Tier 25 (≈62KB) completed in 0.002133s
Tier 26 (≈101KB) completed in 0.002887s
Tier 27 (≈164KB) completed in 0.005339s
Tier 28 (≈265KB) completed in 0.004848s
Tier 29 (≈430KB) completed in 0.008141s
Tier 30 (≈696KB) completed in 0.012588s
Tier 31 (≈1MB) completed in 0.020609s
Tier 32 (≈1MB) completed in 0.033941s
Tier 33 (≈2MB) completed in 0.059347s
Tier 34 (≈4MB) completed in 0.088527s
Tier 35 (≈7MB) completed in 0.142195s
Tier 36 (≈12MB) completed in 0.233902s
Tier 37 (≈19MB) completed in 0.375642s
Tier 38 (≈31MB) completed in 0.611690s
Tier 39 (≈51MB) completed in 0.979428s
Tier 40 (≈83MB) exceeded 1.00s cutoff with time 1.585953s
```

就最后一个没过了！ 我去加速试试看

手动函数内联下：

```c
while(start < end)
{
    size_t bit1 = (bitarray->buf[start / 8] & bitmask(start)) ? true : false;
    size_t bit2 = (bitarray->buf[end / 8] & bitmask(end)) ? true : false;
    bitarray->buf[start/ 8] = 
    (bitarray->buf[start / 8] & ~bitmask(start)) | (bit2 ? bitmask(start) : 0);
    bitarray->buf[end/ 8] = 
    (bitarray->buf[end / 8] & ~bitmask(end)) | (bit1 ? bitmask(end) : 0);
    start++;
    end--;
}
```

Oh 这样反而变慢了？ 可能这样写代码体积就大了， 指令缓存命中率下降了很多

```
Tier 28 (≈265KB) completed in 0.005717s
Tier 29 (≈430KB) completed in 0.009042s
Tier 30 (≈696KB) completed in 0.014702s
Tier 31 (≈1MB) completed in 0.026577s
Tier 32 (≈1MB) completed in 0.038883s
Tier 33 (≈2MB) completed in 0.062448s
Tier 34 (≈4MB) completed in 0.152930s
Tier 35 (≈7MB) completed in 0.163657s
Tier 36 (≈12MB) completed in 0.275645s
Tier 37 (≈19MB) completed in 0.449393s
Tier 38 (≈31MB) completed in 0.718950s
Tier 39 (≈51MB) exceeded 1.00s cutoff with time 1.165398s
```

接下来我就想继续优化一下， 比如增加临时变量

这里又有一个很坑的点， 下面这两个代码居然是不等价的:

```c
char temp_start = bitarray -> buf[start / 8];
char temp_end = bitarray -> buf[end / 8];
size_t bit1 = (temp_start & bitmask(start)) ? true : false;
size_t bit2 = (temp_end & bitmask(end)) ? true : false;
bitarray -> buf[start/ 8] = 
(temp_start & ~bitmask(start)) | (bit2 ? bitmask(start) : 0);
bitarray ->buf[end / 8] = 
(temp_end & ~bitmask(end)) | (bit1 ? bitmask(end) : 0);
start++;
end--;
```

和

```c
char temp_start = bitarray -> buf[start / 8];
char temp_end = bitarray -> buf[end / 8];
size_t bit1 = (temp_start & bitmask(start)) ? true : false;
size_t bit2 = (temp_end & bitmask(end)) ? true : false;
bitarray -> buf[start/ 8] = 
(bitarray->buf[start / 8] & ~bitmask(start)) | (bit2 ? bitmask(start) : 0);
bitarray->buf[end/ 8] = 
(bitarray->buf[end / 8] & ~bitmask(end)) | (bit1 ? bitmask(end) : 0);
start++;
end--;
```

原因是， 如果 start 和 end 指向同一个 byte, 那么改 bitarray -> buf[start/ 8] 就相当于改 bitarray -> buf[end / 8] !

所以增加临时变量我最多也只能写到这样：

```c

  assert(bit_offset + bit_length <= bitarray -> bit_sz);
  size_t start = bit_offset;
  size_t end = bit_offset + bit_length - 1;
  while(start < end)
  {
    char temp_start = bitarray -> buf[start / 8];
    char temp_end = bitarray -> buf[end / 8];
    size_t bit1 = (temp_start & bitmask(start)) ? true : false;
    size_t bit2 = (temp_end & bitmask(end)) ? true : false;
    bitarray->buf[start/ 8] = 
    (temp_start & ~bitmask(start)) | (bit2 ? bitmask(start) : 0);
    bitarray->buf[end/ 8] = 
    (bitarray->buf[end / 8] & ~bitmask(end)) | (bit1 ? bitmask(end) : 0);
    start++;
    end--;
  }
```

这样还是过不了 39：

```
Tier 30 (≈696KB) completed in 0.014571s
Tier 31 (≈1MB) completed in 0.023743s
Tier 32 (≈1MB) completed in 0.039351s
Tier 33 (≈2MB) completed in 0.063378s
Tier 34 (≈4MB) completed in 0.101288s
Tier 35 (≈7MB) completed in 0.167740s
Tier 36 (≈12MB) completed in 0.271853s
Tier 37 (≈19MB) completed in 0.439827s
Tier 38 (≈31MB) completed in 0.702893s
Tier 39 (≈51MB) exceeded 1.00s cutoff with time 1.137663s
Succesfully completed tier: 38
```

所以换回了不内联的写法， 那样起码 39 可以过。

我发现 ./everybit -s -m -l 这三个速度测试都有几个点没过， 感觉是我的 reverse 算法不够好， 我得改进一下。

#### 方法2 ： 还是循环， 但是开额外空间

大致就是先存在临时空间， 之后再 copy back. 感觉这种写法肯定不比 1 要快， 所以只是写了玩玩：

```c
void bitarray_reverse(bitarray_t* const bitarray, const size_t bit_offset, const size_t bit_length) 
{
    if(bit_length == 0 || bit_length == 1) 
    {
      return;
    }
    // Allocate temporary buffer for the bit range
    char* temp = malloc((bit_length + 7) / 8);
    if(!temp) 
    { 
      return; 
    }

    // Copy and reverse bits into temporary buffer
    for(size_t i = 0; i < bit_length; i++) 
    {
        size_t bit = bitarray_get(bitarray, bit_offset + i);
        size_t rev_idx = bit_length - 1 - i;
        if(bit) 
        {
            temp[rev_idx / 8] |= (1 << (rev_idx % 8));
        } 
        else 
        {
            temp[rev_idx / 8] &= ~(1 << (rev_idx % 8));
        }
    }

    // Copy back to the original bitarray
    for (size_t i = 0; i < bit_length; i++) 
    {
        size_t bit = (temp[i / 8] >> (i % 8)) & 1;
        bitarray_set(bitarray, bit_offset + i, bit);
    }

    free(temp);
}

```

果然， 这种 36 都过不了:

```
Tier 29 (≈430KB) completed in 0.040109s
Tier 30 (≈696KB) completed in 0.063387s
Tier 31 (≈1MB) completed in 0.103052s
Tier 32 (≈1MB) completed in 0.169656s
Tier 33 (≈2MB) completed in 0.274003s
Tier 34 (≈4MB) completed in 0.444727s
Tier 35 (≈7MB) completed in 0.734603s
Tier 36 (≈12MB) exceeded 1.00s cutoff with time 1.173164s
```

#### 方法3 ： 打表