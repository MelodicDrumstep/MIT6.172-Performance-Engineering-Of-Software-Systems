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
#include <math.h>
int reverse_bits(unsigned int x) 
{
    unsigned int reversed = 0;
    unsigned int bits = 8; 
    while(bits--) 
    {
        reversed = (reversed << 1) | (x & 1);
        x >>= 1;
    }
    return reversed;
}

int main() {
    printf("static const uint8_t REVERSE_BYTE_LOOKUP[256] = \n{\n");
    for(int i = 0; i < 256; i++) 
    {
        int to_be_reversed = i;
        int reversed = reverse_bits(to_be_reversed);
        printf("0x%02X", reversed); 

        if(i < 255) 
        {
            printf(", ");
        }
        if((i + 1) % 8 == 0) 
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
0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 
0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0, 
0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 
0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8, 
0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 
0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4, 
0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 
0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC, 
0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 
0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2, 
0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 
0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA, 
0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 
0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6, 
0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 
0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE, 
0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 
0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1, 
0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 
0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9, 
0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 
0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5, 
0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 
0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD, 
0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 
0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3, 
0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 
0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB, 
0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 
0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7, 
0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 
0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
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