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

用 metaprogramming 思想， 写个程序生成程序， 把 1 个 byte 内的 reverse 情况打表打出来:

``` c
#include <stdio.h>
#include "./everybit/bitarray.h"

int main()
{
    printf("static const uint8_t REVERSE_BYTE_LOOKUP[256] = \n{\n");
    for(int i = 0; i < 256; i++)
    {
        int to_be_reverse = i;
        bitarray_t * bits = bitarray_new(8);
        for(int j = 0; j < 8; j++)
        {
            bitarray_set(bits, 7 - j, (to_be_reverse & 1) == 1);
            to_be_reverse = to_be_reverse >> 1;
        }
        bitarray_reverse(bits, 0, 8);
        bitarray_print(bits);
        printf(", ");
        if(i % 8 == 7)
        {
            printf("\n");
        }
    }
    printf("};\n");
    return 0;
}
```

打出来是这样， 存在一个头文件里:

```c
#ifndef LUT_H
#define LUT_H

#include <stdint.h>

static const uint8_t REVERSE_BYTE_LOOKUP[256] = 
{
00000000, 10000000, 01000000, 11000000, 00100000, 10100000, 01100000, 11100000, 
00010000, 10010000, 01010000, 11010000, 00110000, 10110000, 01110000, 11110000, 
00001000, 10001000, 01001000, 11001000, 00101000, 10101000, 01101000, 11101000, 
00011000, 10011000, 01011000, 11011000, 00111000, 10111000, 01111000, 11111000, 
00000100, 10000100, 01000100, 11000100, 00100100, 10100100, 01100100, 11100100, 
00010100, 10010100, 01010100, 11010100, 00110100, 10110100, 01110100, 11110100, 
00001100, 10001100, 01001100, 11001100, 00101100, 10101100, 01101100, 11101100, 
00011100, 10011100, 01011100, 11011100, 00111100, 10111100, 01111100, 11111100, 
00000010, 10000010, 01000010, 11000010, 00100010, 10100010, 01100010, 11100010, 
00010010, 10010010, 01010010, 11010010, 00110010, 10110010, 01110010, 11110010, 
00001010, 10001010, 01001010, 11001010, 00101010, 10101010, 01101010, 11101010, 
00011010, 10011010, 01011010, 11011010, 00111010, 10111010, 01111010, 11111010, 
00000110, 10000110, 01000110, 11000110, 00100110, 10100110, 01100110, 11100110, 
00010110, 10010110, 01010110, 11010110, 00110110, 10110110, 01110110, 11110110, 
00001110, 10001110, 01001110, 11001110, 00101110, 10101110, 01101110, 11101110, 
00011110, 10011110, 01011110, 11011110, 00111110, 10111110, 01111110, 11111110, 
00000001, 10000001, 01000001, 11000001, 00100001, 10100001, 01100001, 11100001, 
00010001, 10010001, 01010001, 11010001, 00110001, 10110001, 01110001, 11110001, 
00001001, 10001001, 01001001, 11001001, 00101001, 10101001, 01101001, 11101001, 
00011001, 10011001, 01011001, 11011001, 00111001, 10111001, 01111001, 11111001, 
00000101, 10000101, 01000101, 11000101, 00100101, 10100101, 01100101, 11100101, 
00010101, 10010101, 01010101, 11010101, 00110101, 10110101, 01110101, 11110101, 
00001101, 10001101, 01001101, 11001101, 00101101, 10101101, 01101101, 11101101, 
00011101, 10011101, 01011101, 11011101, 00111101, 10111101, 01111101, 11111101, 
00000011, 10000011, 01000011, 11000011, 00100011, 10100011, 01100011, 11100011, 
00010011, 10010011, 01010011, 11010011, 00110011, 10110011, 01110011, 11110011, 
00001011, 10001011, 01001011, 11001011, 00101011, 10101011, 01101011, 11101011, 
00011011, 10011011, 01011011, 11011011, 00111011, 10111011, 01111011, 11111011, 
00000111, 10000111, 01000111, 11000111, 00100111, 10100111, 01100111, 11100111, 
00010111, 10010111, 01010111, 11010111, 00110111, 10110111, 01110111, 11110111, 
00001111, 10001111, 01001111, 11001111, 00101111, 10101111, 01101111, 11101111, 
00011111, 10011111, 01011111, 11011111, 00111111, 10111111, 01111111, 11111111, 
};

#endif
```

然后我的 reverse 函数分情况： 如果 start 和 end 在同一个 byte 里面， 延用之前的算法， 否则， 直接用表里的元素：

```c
void bitarray_reverse(bitarray_t* const bitarray, const size_t bit_offset, const size_t bit_length)
{
  if(bit_length == 0 || bit_length == 1)
  {
    return;
  }
  size_t start_byte = bit_offset / 8;
  size_t end_byte = (bit_offset + bit_length - 1) / 8;

  // Reverse bytes where full byte reversal is possible
  while(start_byte < end_byte) 
  {
      uint8_t temp = REVERSE_BYTE_LOOKUP[bitarray -> buf[start_byte]];
      bitarray -> buf[start_byte] = REVERSE_BYTE_LOOKUP[bitarray -> buf[end_byte]];
      bitarray -> buf[end_byte] = temp;
      start_byte++;
      end_byte--;
  }

  // If reversing within a single byte
  if (start_byte == end_byte) 
  {
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
}

```

看下效果：

```
Tier 40 (≈83MB) completed in 0.055745s
Tier 41 (≈135MB) completed in 0.088650s
Tier 42 (≈218MB) completed in 0.142137s
Tier 43 (≈354MB) completed in 0.233100s
Tier 44 (≈573MB) completed in 0.384014s
Tier 45 (≈927MB) completed in 0.615932s
Tier 46 (≈1GB) exceeded 1.00s cutoff with time 1.005288s
Succesfully completed tier: 45
---- END RESULTS ----
```

这下真的加速了超级多了， 但是还是过不了所有测试

Ohhhhhhhhhhhhhh No! 紧急情况， 上方代码有 bug， 但是官方给的测试太弱了， 没测出来

是这样的: 我上方写的都是假设 start 和 end 是对齐 8 bit 的情况， 如果不对齐的话， 比如 start 是某个 byte 倒数第 5 个 bit 的位置， end 是某个 byte 正数第 1 个 bit 的位置， 那这样就没法用上面的查表算法了（毕竟是错位的嘛）

我又写了个测试程序，然后改了下 bug， 现在这个版本应该是正确的：

```c
static inline void bitarray_reverse(bitarray_t* const bitarray, const size_t bit_offset, const size_t bit_length)
{
  if(bit_length == 0 || bit_length == 1)
  {
    return;
  }

  size_t start = bit_offset;
  size_t end = bit_offset + bit_length - 1;
  size_t start_byte = bit_offset / 8;
  size_t end_byte = (bit_offset + bit_length - 1) / 8;

  // If reversing within a single byte
  if(start_byte == end_byte) 
  {
      while(start < end)
      {
        size_t bit1 = bitarray_get(bitarray, start);
        size_t bit2 = bitarray_get(bitarray, end);
        bitarray_set(bitarray, start, bit2);
        bitarray_set(bitarray, end, bit1);
        start++;
        end--;
      }
      return;
  }

  //If it's aligned 
  if((start + end) % 7 == 0)
  {
    char temp = 8 - (start % 8);
    for(size_t i = 0; i < temp; i++)
    {
      size_t bit1 = bitarray_get(bitarray, start);
      size_t bit2 = bitarray_get(bitarray, end);
      bitarray_set(bitarray, start, bit2);
      bitarray_set(bitarray, end, bit1);
      start++;
      end--;
    }
    start_byte++;
    end_byte--;
    // Reverse bytes where full byte reversal is possible
    while(start_byte < end_byte) 
    {
        uint8_t temp = REVERSE_BYTE_LOOKUP_8[bitarray -> buf[start_byte]];
        bitarray -> buf[start_byte] = REVERSE_BYTE_LOOKUP_8[bitarray -> buf[end_byte]];
        bitarray -> buf[end_byte] = temp;
        start_byte++;
        end_byte--;
    }
    return;
  }

//Oh no, now I have to use the slow regular swap algorithm because it's not aligned
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

性能测试大概是

```
Tier 32 (≈1MB) completed in 0.067680s
Tier 33 (≈2MB) completed in 0.086575s
Tier 34 (≈4MB) completed in 0.150487s
Tier 35 (≈7MB) completed in 0.211393s
Tier 36 (≈12MB) completed in 0.430775s
Tier 37 (≈19MB) completed in 0.709886s
Tier 38 (≈31MB) exceeded 1.00s cutoff with time 1.033241s
Succesfully completed tier: 37
```

看来还是不够好。 主要是内存不对齐的版本性能实在是太差了。

用 linux-perf 看下现在的瓶颈：

```
# Overhead  Command   Shared Object      Symbol                       
# ........  ........  .................  .............................
#
    47.26%  everybit  everybit           [.] bitarray_set
    19.47%  everybit  everybit           [.] bitarray_get
    16.01%  everybit  everybit           [.] bitmask
    13.82%  everybit  everybit           [.] bitarray_reverse
     2.38%  everybit  libc.so.6          [.] __random
     0.31%  everybit  libc.so.6          [.] __random_r
     0.25%  everybit  everybit           [.] bitarray_randfill
     0.07%  everybit  [kernel.kallsyms]  [k] _raw_spin_lock
     0.07%  everybit  everybit           [.] rand@plt
     0.04%  everybit  [kernel.kallsyms]  [k] clear_page_erms
```

果然就是 set bit, 不对齐的情况一个一个 set 真的太慢了。

我想到了一个用左移 / 右移来处理最难处理的不对称情况的算法， 我正在实现它。

我首先实现了一个这样的辅助函数， 可以创建一个前 a 位为 1 其他位为 0 的 bitmask.

```c
//This function will create a bitmask whose 
//prior a bits are 1 and others are 0
//for example, when a = 2, it will
//generate 0b11000000
uint64_t bitarray_create_mask_head(size_t a)
{
  return ~(((uint64_t)1 << (8 - a)) - 1);
}
```

然后我的思路是， 我先假装是对称的，把逆序做了， 之后再移位。 我画了个图来示意：(为了简便起见， 图里我假设 4 个 bit 组成一个 Byte)

![](https://notes.sjtu.edu.cn/uploads/upload_b80c03742949fc01e577c09ff801d6de.jpg)

接下来直接来看看我的最终版本的实现吧！我写了非常详尽的注释

##### 算法代码详解

```c
void bitarray_reverse(bitarray_t* const bitarray, const size_t bit_offset, const size_t bit_length)
{
  if(bit_length <= 1)
  {
    return;
  }

  size_t start = bit_offset;
  size_t end = bit_offset + bit_length - 1;
  size_t start_byte = bit_offset / 8;
  size_t end_byte = (bit_offset + bit_length - 1) / 8;

  // If reversing within a single byte
  if(start_byte == end_byte) 
  {
      while(start < end)
      {
        size_t bit1 = bitarray_get(bitarray, start);
        size_t bit2 = bitarray_get(bitarray, end);
        bitarray_set(bitarray, start, bit2);
        bitarray_set(bitarray, end, bit1);
        start++;
        end--;
      }
      return;
  }

  //Now reversing within multiple bytes
  //If it's aligned, then I can use Lookup table straight away
  if((start + end + 1) % 8 == 0)
  {
    char temp = 8 - (start % 8);
    for(size_t i = 0; i < temp; i++)
    {
      size_t bit1 = bitarray_get(bitarray, start);
      size_t bit2 = bitarray_get(bitarray, end);
      bitarray_set(bitarray, start, bit2);
      bitarray_set(bitarray, end, bit1);
      start++;
      end--;
    }
    start_byte++;
    end_byte--;
    // Reverse bytes where full byte reversal is possible
    while(start_byte < end_byte) 
    {

          //Notice that I have to convert the index of array to be unsigned!!!
          //Otherwise it will fail
        uint8_t temp = REVERSE_BYTE_LOOKUP_8[(uint8_t)bitarray -> buf[start_byte]];
        bitarray -> buf[start_byte] = REVERSE_BYTE_LOOKUP_8[(uint8_t)bitarray -> buf[end_byte]];
        bitarray -> buf[end_byte] = temp;
        start_byte++;
        end_byte--;
    }
    return;
  }

// Now the reverse is not aligned, I have to do shift in a clever way
// Firstly reverse by BYTE, then do shift for the middle bytes. Notice that
// I have to deal with the first byte and the end byte as a special case


  //Compute the shift value
  //Notice!! "start" and "end" variables are size_t
  // MUST be converted to int8_t to get the shift value
   int8_t shift = (int8_t)(7 - (start % 8)) - (end % 8);

   //Left shift case
   if(shift > 0)
   {
      //Now I store the start byte and end byte
      //In the end I will restore them with correct value
      uint8_t store_start_byte = bitarray -> buf[start_byte];
      uint8_t store_end_byte = bitarray -> buf[end_byte];


      //And copy the start byte pointer and the end byte pointer for the same reason
      size_t start_byte_pointer_copy = start_byte;
      size_t end_byte_pointer_copy = end_byte;

      //Now just invert the middle ones as the last case
      while(start_byte < end_byte) 
      {
          //Notice that I have to convert the index of array to be unsigned!!!
          //Otherwise it will fail
          uint8_t temp = REVERSE_BYTE_LOOKUP_8[(uint8_t)bitarray -> buf[start_byte]];
          bitarray -> buf[start_byte] = REVERSE_BYTE_LOOKUP_8[(uint8_t)bitarray -> buf[end_byte]];
          bitarray -> buf[end_byte] = temp;
          start_byte++;
          end_byte--;
      }

      //Now do the shifting!!
      //In that here is left shift, we start from the start byte 
      
      for(size_t i = start_byte_pointer_copy; i < end_byte_pointer_copy; i++)
      {
          bitarray -> buf[i] = ((u_int8_t)(bitarray -> buf[i]) >> shift) | ((u_int8_t)(bitarray -> buf[i + 1]) << (8 - shift));
      }

      //Now shift the end byte
      bitarray -> buf[end_byte_pointer_copy] = (u_int8_t)(bitarray -> buf[end_byte_pointer_copy]) >> shift;

      //Now restore the start byte
      bitarray -> buf[start_byte_pointer_copy] = (bitarray_create_mask_tail(start % 8) & store_start_byte) 
                                              |  (bitarray_create_mask_head(8 - start % 8) & bitarray -> buf[start_byte_pointer_copy]);
      //And restore the end byte
      bitarray -> buf[end_byte_pointer_copy] = (bitarray_create_mask_tail(end % 8 + 1) & bitarray -> buf[end_byte_pointer_copy]) 
                                              |  (bitarray_create_mask_head(7 - end % 8) & store_end_byte);
   }

  //Right shift, it's just the same as the former one
  //invert "<<" and ">>"
  //and when shifting, start from the end byte to the start byte
   else
   {
      assert(shift < 0);
      shift = -shift;

      //Now I store the start byte and end byte
      //In the end I will restore them with correct value
      uint8_t store_start_byte = bitarray -> buf[start_byte];
      uint8_t store_end_byte = bitarray -> buf[end_byte];

      //And copy the start byte pointer and the end byte pointer for the same reason
      size_t start_byte_pointer_copy = start_byte;
      size_t end_byte_pointer_copy = end_byte;

      //Now just invert the middle ones as the last case
      while(start_byte < end_byte) 
      {
          uint8_t temp = REVERSE_BYTE_LOOKUP_8[(uint8_t)bitarray -> buf[start_byte]];
          bitarray -> buf[start_byte] = REVERSE_BYTE_LOOKUP_8[(uint8_t)bitarray -> buf[end_byte]];
          bitarray -> buf[end_byte] = temp;
          start_byte++;
          end_byte--;
      }

      //Now do the shifting!! start from the end byte
      for(size_t i = end_byte_pointer_copy; i > start_byte_pointer_copy; i--)
      {
          bitarray -> buf[i] = ((u_int8_t)(bitarray -> buf[i]) << shift) | ((u_int8_t)(bitarray -> buf[i - 1]) >> (8 - shift));
      }

      //shift the start byte
      bitarray -> buf[start_byte_pointer_copy] = (u_int8_t)(bitarray -> buf[start_byte_pointer_copy]) << shift;

      //restore the start byte
      bitarray -> buf[start_byte_pointer_copy] = (bitarray_create_mask_tail(start % 8) & store_start_byte) 
                                              |  (bitarray_create_mask_head(8 - start % 8) & bitarray -> buf[start_byte_pointer_copy]);

      //And restore the end byte
      bitarray -> buf[end_byte_pointer_copy] = (bitarray_create_mask_tail(end % 8 + 1) & bitarray -> buf[end_byte_pointer_copy]) 
                                              |  (bitarray_create_mask_head(7 - end % 8) & store_end_byte);   }
    return;
}
```

这个算法真的写得很开心！！

##### 实现这个算法中遇到过的重重困难

###### 1

首先出现了一个大坑： size_t / uint8 等等 unsigned 的类型是不会有负值的！！！

所以我算 shift 只能用

```c 
  int8_t shift = (int8_t)(7 - (start % 8)) - (end % 8);
```

而不是

```c
 size_t shift = (7 - (start % 8)) - (end % 8);
```

赋值语句右边也要强制类型转换， 因为这里的 start, end 都是 size_t, 如果不转换右边就算出 size_t 了。

###### 2

另外又发现了一个超级坑的点：我们的 buf 里面存的东西，第 0 位是LSB, 第 7 位是MSB， 但是给的函数打印的时候是从第 0 位打印到第 7 位， 所以打印出来的高位在右边！！这就很坑了，不注意的话左移右移就写反了！

###### 3

还有一点：这里存的是 char, 所以会进行算术右移， 但是我希望的是逻辑右移， 所以要这样写， 强转成 unsigned char:

```c
//Now do the shifting!!
for(size_t i = start_byte_pointer_copy; i < end_byte_pointer_copy; i++)
{
    bitarray -> buf[i] = ((u_int8_t)(bitarray -> buf[i]) >> shift) | ((u_int8_t)(bitarray -> buf[i + 1]) << (8 - shift));
}
bitarray -> buf[end_byte_pointer_copy] = (u_int8_t)(bitarray -> buf[end_byte_pointer_copy]) >> shift;
```

###### 4

又遇到了一个超级坑的点：我在数组索引的时候， 传入的东西要是一个 unsigned 值， 而我 buf 里面存的是 char. 所以我传入之前要先把这个 char 强制类型转换成 unsigned char, 或者叫 uint8_t, 不然数组索引会失效！这个 bug 真的很难找， 但是找到了之后感觉真的学到了很多很多！ 

```c
uint8_t temp = REVERSE_BYTE_LOOKUP_8[bitarray -> buf[start_byte]];
bitarray -> buf[start_byte] = REVERSE_BYTE_LOOKUP_8[bitarray -> buf[end_byte]];
```

要写成

```c
uint8_t temp = REVERSE_BYTE_LOOKUP_8[(uint8_t)bitarray -> buf[start_byte]];
bitarray -> buf[start_byte] = REVERSE_BYTE_LOOKUP_8[(uint8_t)bitarray -> buf[end_byte]];
```


终于全过了正确性测试，真的很开心，很不容易！！

```shell
everybit /mnt/d/MIT6.172/project1/MIT6_172F18-project1/everybit/tests/default at line 34PASSED
everybit /mnt/d/MIT6.172/project1/MIT6_172F18-project1/everybit/tests/default at line 41PASSED
everybit /mnt/d/MIT6.172/project1/MIT6_172F18-project1/everybit/tests/default at line 49PASSED
everybit /mnt/d/MIT6.172/project1/MIT6_172F18-project1/everybit/tests/default at line 52PASSED
everybit /mnt/d/MIT6.172/project1/MIT6_172F18-project1/everybit/tests/default at line 55PASSED
everybit /mnt/d/MIT6.172/project1/MIT6_172F18-project1/everybit/tests/default at line 58PASSED
everybit /mnt/d/MIT6.172/project1/MIT6_172F18-project1/everybit/tests/default at line 61PASSED
everybit /mnt/d/MIT6.172/project1/MIT6_172F18-project1/everybit/tests/default at line 67PASSED
everybit /mnt/d/MIT6.172/project1/MIT6_172F18-project1/everybit/tests/default at line 71PASSED
everybit /mnt/d/MIT6.172/project1/MIT6_172F18-project1/everybit/tests/default at line 75PASSED
everybit /mnt/d/MIT6.172/project1/MIT6_172F18-project1/everybit/tests/default at line 79PASSED
everybit /mnt/d/MIT6.172/project1/MIT6_172F18-project1/everybit/tests/default at line 83PASSED
Ran 4 test functions, 12 individual tests passed, 0 individual tests failed.
PASSED
```

##### 最终性能测试

最后再来看下运行时间：

```shell
./everybit -s:

Tier 31 (≈1MB) completed in 0.001960s
Tier 32 (≈1MB) completed in 0.003096s
Tier 33 (≈2MB) completed in 0.004906s
Tier 34 (≈4MB) completed in 0.007150s
Tier 35 (≈7MB) exceeded 0.01s cutoff with time 0.013421s
Succesfully completed tier: 34


./everybit -m


Tier 34 (≈4MB) completed in 0.007189s
Tier 35 (≈7MB) completed in 0.014021s
Tier 36 (≈12MB) completed in 0.021735s
Tier 37 (≈19MB) completed in 0.035607s
Tier 38 (≈31MB) completed in 0.057460s
Tier 39 (≈51MB) exceeded 0.10s cutoff with time 0.104643s
Succesfully completed tier: 38

./everybit -l

Tier 40 (≈83MB) completed in 0.171424s
Tier 41 (≈135MB) completed in 0.250380s
Tier 42 (≈218MB) completed in 0.399608s
Tier 43 (≈354MB) completed in 0.681227s
Tier 44 (≈573MB) exceeded 1.00s cutoff with time 1.015854s
Succesfully completed tier: 43
```

综上， -l 达到了 43 层， 真的效果已经超级好了！

## 总结

我感觉这个 project 让我学到的东西真的超级超级多。 首先， 我可以改进 rotate 算法， 把问题规约成计算 reverse. 然后我先实现朴素的按 bit 的 reverse. 之后我又可以想到， 为什么不直接按 byte 来 reverse 呢？ 也就是直接打表好了。 但是打表又有个问题： 如果 reverse 的序列不对齐（或者说不对称）怎么办？ 那我就要先 reverse, 结束之后再 shift!! 所以最终我就按照这个思路实现了整个算法， 加速了超级多倍。 

最终结果是 -l 测试达到了 43 层！！