/**
 * Copyright (c) 2012 MIT License by 6.172 Staff
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 **/

// Implements the ADT specified in bitarray.h as a packed array of bits; a bit
// array containing bit_sz bits will consume roughly bit_sz/8 bytes of
// memory.


#include "./bitarray.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "lut.h"

#include <sys/types.h>


//#define DEBUG

// ********************************* Types **********************************

// Concrete data type representing an array of bits.
struct bitarray {
  // The number of bits represented by this bit array.
  // Need not be divisible by 8.
  size_t bit_sz;

  // The underlying memory buffer that stores the bits in
  // packed form (8 per byte).
  char* buf;
};


// ******************** Prototypes for static functions *********************


void print_uint8_in_binary(uint8_t value) {
    printf("Binary representation of %u is: ", value);
    for (int i = 7; i >= 0; i--) {
        printf("%d", (value >> i) & 1); // 右移i位，然后与1做AND运算来获取第i位的值
    }
    printf("\n");
}

//This function will create a bitmask whose 
//prior a bits are 1 and others are 0
//for example, when a = 2, it will
//generate 0b11000000
uint8_t bitarray_create_mask_head(size_t a)
{
  return ~(((uint8_t)1 << (8 - a)) - 1);
}

//This function will create a bitmask whose 
//rightmost a bits are 1 and others are 0
//for example, when a = 2, it will
//generate 0b00000011
uint8_t bitarray_create_mask_tail(size_t a)
{
  if(a == 8)
  {
    return (uint8_t)-1;//This is 11111111
  }
  return ((uint8_t)1 << a) - 1;
}

// Rotates a subarray left by an arbitrary number of bits.
//
// bit_offset is the index of the start of the subarray
// bit_length is the length of the subarray, in bits
// bit_left_amount is the number of places to rotate the
//                    subarray left
//
// The subarray spans the half-open interval
// [bit_offset, bit_offset + bit_length)
// That is, the start is inclusive, but the end is exclusive.
static void bitarray_rotate_left(bitarray_t* const bitarray,
                                 const size_t bit_offset,
                                 const size_t bit_length,
                                 const size_t bit_left_amount);

// Rotates a subarray left by one bit.
//
// bit_offset is the index of the start of the subarray
// bit_length is the length of the subarray, in bits
//
// The subarray spans the half-open interval
// [bit_offset, bit_offset + bit_length)
// That is, the start is inclusive, but the end is exclusive.
static void bitarray_rotate_left_one(bitarray_t* const bitarray,
                                     const size_t bit_offset,
                                     const size_t bit_length);

// Portable modulo operation that supports negative dividends.
//
// Many programming languages define modulo in a manner incompatible with its
// widely-accepted mathematical definition.
// http://stackoverflow.com/questions/1907565/c-python-different-behaviour-of-the-modulo-operation
// provides details; in particular, C's modulo
// operator (which the standard calls a "remainder" operator) yields a result
// signed identically to the dividend e.g., -1 % 10 yields -1.
// This is obviously unacceptable for a function which returns size_t, so we
// define our own.
//
// n is the dividend and m is the divisor
//
// Returns a positive integer r = n (mod m), in the range
// 0 <= r < m.
static size_t modulo(const ssize_t n, const size_t m);

// Produces a mask which, when ANDed with a byte, retains only the
// bit_index th byte.
//
// Example: bitmask(5) produces the byte 0b00100000.
//
// (Note that here the index is counted from right
// to left, which is different from how we represent bitarrays in the
// tests.  This function is only used by bitarray_get and bitarray_set,
// however, so as long as you always use bitarray_get and bitarray_set
// to access bits in your bitarray, this reverse representation should
// not matter.
static char bitmask(const size_t bit_index);


void bitarray_print(const bitarray_t* const bitarray);

// ******************************* Functions ********************************

bitarray_t* bitarray_new(const size_t bit_sz) 
{
  // Allocate an underlying buffer of ceil(bit_sz/8) bytes.
  char* const buf = calloc(1, (bit_sz+7) / 8);
  if (buf == NULL) {
    return NULL;
  }

  // Allocate space for the struct.
  bitarray_t* const bitarray = malloc(sizeof(struct bitarray));
  if (bitarray == NULL) {
    free(buf);
    return NULL;
  }

  bitarray->buf = buf;
  bitarray->bit_sz = bit_sz;
  return bitarray;
}

bitarray_t* bitarray_new_with_2byte_value(u_int8_t value1, u_int8_t value2) 
{
  // Allocate an underlying buffer of ceil(bit_sz/8) bytes.
  char* const buf = calloc(1, (16 + 7) / 8);
  if (buf == NULL) 
  {
    return NULL;
  }

  // Allocate space for the struct.
  bitarray_t* const bitarray = malloc(sizeof(struct bitarray));
  if (bitarray == NULL) 
  {
    free(buf);
    return NULL;
  }

  bitarray->buf = buf;
  bitarray->bit_sz = 16;

  bitarray -> buf[0] = value1;
  bitarray -> buf[1] = value2;

  return bitarray;
}

void bitarray_free(bitarray_t* const bitarray) 
{
  if (bitarray == NULL) {
    return;
  }
  free(bitarray->buf);
  bitarray->buf = NULL;
  free(bitarray);
}

size_t bitarray_get_bit_sz(const bitarray_t* const bitarray) {
  return bitarray->bit_sz;
}

bool bitarray_get(const bitarray_t* const bitarray, const size_t bit_index) 
{
  assert(bit_index < bitarray->bit_sz);

  // We're storing bits in packed form, 8 per byte.  So to get the nth
  // bit, we want to look at the (n mod 8)th bit of the (floor(n/8)th)
  // byte.
  //
  // In C, integer division is floored explicitly, so we can just do it to
  // get the byte; we then bitwise-and the byte with an appropriate mask
  // to produce either a zero byte (if the bit was 0) or a nonzero byte
  // (if it wasn't).  Finally, we convert that to a boolean.
  return (bitarray->buf[bit_index / 8] & bitmask(bit_index)) ?
         true : false;
}

void bitarray_set(bitarray_t* const bitarray,
                  const size_t bit_index,
                  const bool value) 
{
  assert(bit_index < bitarray->bit_sz);

  // We're storing bits in packed form, 8 per byte.  So to set the nth
  // bit, we want to set the (n mod 8)th bit of the (floor(n/8)th) byte.
  //
  // In C, integer division is floored explicitly, so we can just do it to
  // get the byte; we then bitwise-and the byte with an appropriate mask
  // to clear out the bit we're about to set.  We bitwise-or the result
  // with a byte that has either a 1 or a 0 in the correct place.
  bitarray->buf[bit_index / 8] =
    (bitarray->buf[bit_index / 8] & ~bitmask(bit_index)) |
    (value ? bitmask(bit_index) : 0);
}

void bitarray_randfill(bitarray_t* const bitarray)
{
  int32_t *ptr = (int32_t *)bitarray->buf;
  for (int64_t i=0; i<bitarray->bit_sz/32 + 1; i++)
  {
    ptr[i] = rand();
  }
}

void bitarray_reverse(bitarray_t* const bitarray, const size_t bit_offset, const size_t bit_length)
{
  //DEBUGING
  #ifdef DEBUG
  printf("bitarray_reverse(bitarray, %ld, %ld)\n", bit_offset, bit_length);
  printf("The original bitarray is:\n");
  bitarray_print(bitarray);
  printf("\n");
  #endif
  //DEBUGING

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

    //DEBUGING
    #ifdef DEBUG
    printf("The start and end are in the same byte\n");
    #endif
    //DEBUGING

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

    //DEBUGING
    #ifdef DEBUG
    printf("The start and end are aligned\n");
    #endif
    //DEBUGING

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
   
   //DEBUGING
   #ifdef DEBUG
   printf("The shift is %d\n", shift);
   printf("start byte is %ld and end byte is %ld\n", start_byte, end_byte);
   printf("start is %ld and end is %ld\n", start, end);
   #endif
   //DEBUGING
   
   //Left shift case
   if(shift > 0)
   {

      //DEBUGING
      #ifdef DEBUG
      printf("The shift is positive\n");
      #endif
      //DEBUGING

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

          //DEBUGING
          #ifdef DEBUG
          printf("\nInside the inner while loop\n");
          printf("the start byte is :\n");
          print_uint8_in_binary(bitarray -> buf[start_byte]);
          printf("the end byte is :\n");
          print_uint8_in_binary(bitarray -> buf[end_byte]);
          #endif
          //DEBUGING

          //Notice that I have to convert the index of array to be unsigned!!!
          //Otherwise it will fail
          uint8_t temp = REVERSE_BYTE_LOOKUP_8[(uint8_t)bitarray -> buf[start_byte]];
          bitarray -> buf[start_byte] = REVERSE_BYTE_LOOKUP_8[(uint8_t)bitarray -> buf[end_byte]];
          bitarray -> buf[end_byte] = temp;

                    //DEBUGING
          #ifdef DEBUG

          printf("the reverse of 133 is: \n");
          print_uint8_in_binary(REVERSE_BYTE_LOOKUP_8[133]);
          printf("the reverse of start byte is \n:");
          print_uint8_in_binary(REVERSE_BYTE_LOOKUP_8[(uint8_t)bitarray -> buf[start_byte]]);
          printf("temp is :\n");
          print_uint8_in_binary(temp);

          printf("the reverse of start byte is :");
          print_uint8_in_binary(temp);
          printf("the reverse of end byte is :");
          print_uint8_in_binary(bitarray -> buf[start_byte]);
          printf("\n");
          #endif
          //DEBUGING

          start_byte++;
          end_byte--;
      }

      //DEBUGING
      #ifdef DEBUG
      printf("After the inner while loop\n");
      bitarray_print(bitarray);
      printf("\n");
      #endif
      //DEBUGING

      //Now do the shifting!!
      //In that here is left shift, we start from the start byte 
      
      for(size_t i = start_byte_pointer_copy; i < end_byte_pointer_copy; i++)
      {
          bitarray -> buf[i] = ((u_int8_t)(bitarray -> buf[i]) >> shift) | ((u_int8_t)(bitarray -> buf[i + 1]) << (8 - shift));
      }

      //Now shift the end byte
      bitarray -> buf[end_byte_pointer_copy] = (u_int8_t)(bitarray -> buf[end_byte_pointer_copy]) >> shift;

      //DEBUGING
      #ifdef DEBUG
      printf("After the shifing\n");
      bitarray_print(bitarray);
      printf("\n");
      #endif
      //DEBUGING      

      //Now deal with the start one and the end one with special care

      //DEBUGING
      #ifdef DEBUG
      printf("start mod 8 is %ld and end mod 8 is %ld\n", start % 8, end % 8);
      printf("start byte copy:\n");
      print_uint8_in_binary(store_start_byte);
      printf("end byte copy:\n");
      print_uint8_in_binary(store_end_byte);
      printf("start byte right now:\n");
      print_uint8_in_binary(bitarray -> buf[start_byte_pointer_copy]);
      printf("end byte right now:\n");
      print_uint8_in_binary(bitarray -> buf[end_byte_pointer_copy]);
      printf("bitarray_create_mask_tail(6):\n");
      print_uint8_in_binary(bitarray_create_mask_tail(2));
      printf("bitarray_create_mask_head(2):\n");
      print_uint8_in_binary(bitarray_create_mask_head(6));
      #endif
      //DEBUGING

      //Now restore the start byte
      bitarray -> buf[start_byte_pointer_copy] = (bitarray_create_mask_tail(start % 8) & store_start_byte) 
                                              |  (bitarray_create_mask_head(8 - start % 8) & bitarray -> buf[start_byte_pointer_copy]);

      //DEBUGING
      #ifdef DEBUG
      printf("the first value is :");
      print_uint8_in_binary((bitarray_create_mask_tail(start % 8) & store_start_byte));
      printf("the second value is :");
      print_uint8_in_binary((bitarray_create_mask_head(8 - start % 8) & bitarray -> buf[start_byte_pointer_copy]));
      #endif
      //DEBUGING

      //And restore the end byte
      bitarray -> buf[end_byte_pointer_copy] = (bitarray_create_mask_tail(end % 8 + 1) & bitarray -> buf[end_byte_pointer_copy]) 
                                              |  (bitarray_create_mask_head(7 - end % 8) & store_end_byte);
   }

  //Right shift, it's just the same as the former one
  //invert "<<" and ">>"
  //and when shifting, start from the end byte to the start byte
   else
   {
      //DEBUGING
      #ifdef DEBUG
      printf("shift is negative: %d\n", shift);
      #endif
      //DEBUGING

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

          //DEBUGING
          #ifdef DEBUG
          printf("the reverse of start byte is :");
          print_uint8_in_binary(temp);
          printf("the reverse of end byte is :");
          print_uint8_in_binary(bitarray -> buf[start_byte]);
          #endif
          //DEBUGING

          bitarray -> buf[end_byte] = temp;
          start_byte++;
          end_byte--;
      }

      //DEBUGING
      #ifdef DEBUG
      printf("After the inner while loop\n");
      bitarray_print(bitarray);
      printf("\n");
      #endif
      //DEBUGING


      //Now do the shifting!! start from the end byte
      for(size_t i = end_byte_pointer_copy; i > start_byte_pointer_copy; i--)
      {
          bitarray -> buf[i] = ((u_int8_t)(bitarray -> buf[i]) << shift) | ((u_int8_t)(bitarray -> buf[i - 1]) >> (8 - shift));
      }

      //shift the start byte
      bitarray -> buf[start_byte_pointer_copy] = (u_int8_t)(bitarray -> buf[start_byte_pointer_copy]) << shift;

      //DEBUGING
      #ifdef DEBUG
      printf("After the shifing\n");
      bitarray_print(bitarray);
      printf("\n");
      #endif
      //DEBUGING      

      //Now deal with the start one and the end one with special care

      //DEBUGING
      #ifdef DEBUG
      printf("start mod 8 is %ld and end mod 8 is %ld\n", start % 8, end % 8);
      printf("start byte copy:\n");
      print_uint8_in_binary(store_start_byte);
      printf("end byte copy:\n");
      print_uint8_in_binary(store_end_byte);
      printf("start byte right now:\n");
      print_uint8_in_binary(bitarray -> buf[start_byte_pointer_copy]);
      printf("end byte right now:\n");
      print_uint8_in_binary(bitarray -> buf[end_byte_pointer_copy]);
      printf("bitarray_create_mask_tail(6):\n");
      print_uint8_in_binary(bitarray_create_mask_tail(2));
      printf("bitarray_create_mask_head(2):\n");
      print_uint8_in_binary(bitarray_create_mask_head(6));
      #endif
      //DEBUGING

      //restore the start byte
      bitarray -> buf[start_byte_pointer_copy] = (bitarray_create_mask_tail(start % 8) & store_start_byte) 
                                              |  (bitarray_create_mask_head(8 - start % 8) & bitarray -> buf[start_byte_pointer_copy]);

      //DEBUGING
      #ifdef DEBUG
      printf("the first value is :");
      print_uint8_in_binary((bitarray_create_mask_tail(start % 8) & store_start_byte));
      printf("the second value is :");
      print_uint8_in_binary((bitarray_create_mask_head(8 - start % 8) & bitarray -> buf[start_byte_pointer_copy]));
      #endif
      //DEBUGING

      //And restore the end byte
      bitarray -> buf[end_byte_pointer_copy] = (bitarray_create_mask_tail(end % 8 + 1) & bitarray -> buf[end_byte_pointer_copy]) 
                                              |  (bitarray_create_mask_head(7 - end % 8) & store_end_byte);   }
  

    //DEBUGING
  #ifdef DEBUG
  printf("bitarray_reverse(bitarray, %ld, %ld) done\n", bit_offset, bit_length);
  printf("And the new bitarray is:\n");
  bitarray_print(bitarray);
  printf("\n");
  #endif
  //DEBUGING

  return;
}


void bitarray_rotate(bitarray_t* const bitarray,
                     const size_t bit_offset,
                     const size_t bit_length,
                     const ssize_t bit_right_amount)
{
  if(bit_length == 0)
  {
    return;
  }
  //DEBUGING
  #ifdef DEBUG
  printf("bitarray_rotate(bitarray, %ld, %ld, %ld)\n", bit_offset, bit_length, bit_right_amount);
  #endif
  //DEBUGING

  assert(bit_offset + bit_length <= bitarray -> bit_sz);
  size_t bit_right_amount_after_module = modulo(-bit_right_amount, bit_length);

  #ifdef DEBUG
  printf("bit_right_amount_after_module = %ld\n", bit_right_amount_after_module);
  #endif
  bitarray_reverse(bitarray, bit_offset, bit_right_amount_after_module);
  bitarray_reverse(bitarray, bit_offset + bit_right_amount_after_module, bit_length - bit_right_amount_after_module);
  bitarray_reverse(bitarray, bit_offset, bit_length);
}


static void bitarray_rotate_left(bitarray_t* const bitarray,
                                 const size_t bit_offset,
                                 const size_t bit_length,
                                 const size_t bit_left_amount) 
{
  for (size_t i = 0; i < bit_left_amount; i++) 
  {
    bitarray_rotate_left_one(bitarray, bit_offset, bit_length);
  }
}

static void bitarray_rotate_left_one(bitarray_t* const bitarray,
                                     const size_t bit_offset,
                                     const size_t bit_length) 
{
  // Grab the first bit in the range, shift everything left by one, and
  // then stick the first bit at the end.
  const bool first_bit = bitarray_get(bitarray, bit_offset);
  size_t i;
  for (i = bit_offset; i + 1 < bit_offset + bit_length; i++) 
  {
    bitarray_set(bitarray, i, bitarray_get(bitarray, i + 1));
  }
  bitarray_set(bitarray, i, first_bit);
}

static size_t modulo(const ssize_t n, const size_t m) 
{
  const ssize_t signed_m = (ssize_t)m;
  assert(signed_m > 0);
  const ssize_t result = ((n % signed_m) + signed_m) % signed_m;
  assert(result >= 0);
  return (size_t)result;
}

static char bitmask(const size_t bit_index) 
{
  return 1 << (bit_index % 8);
}

void bitarray_print(const bitarray_t* const bitarray) 
{
    if (bitarray == NULL) 
    {
        printf("bitarray is NULL\n");
        return;
    }

    printf("bitarray (%ld bits): ", bitarray->bit_sz);

    bool result[bitarray -> bit_sz];
    //printf("bitarray (%ld bits): ", bitarray->bit_sz);
    for (int i = 0; i < bitarray -> bit_sz; i++) 
    {
        // Print each bit. Note that bitarray_get already handles bit packing.
        //printf("%d", bitarray_get(bitarray, i));
        // if ((i + 1) % 8 == 0 && i + 1 != bitarray->bit_sz) 
        // {
        //     printf(" "); // Add a space every 8 bits for easier reading
        // }
        result[i] = bitarray_get(bitarray, i);
    }
    for(int i = bitarray -> bit_sz - 1; i >= 0; i--)
    {
        printf("%d", result[i]);
        
        if((i) % 8 == 0 && i + 1 != bitarray->bit_sz) 
        {
            printf(" "); // Add a space every 8 bits for easier reading
        }
    }
}


void bitarray_reverse_naive(bitarray_t* const bitarray, const size_t bit_offset, const size_t bit_length)
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