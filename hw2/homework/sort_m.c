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


#include "./util.h"

// Function prototypes
static inline void merge_m(data_t* A, int p, int q, int r);
static inline void copy_m(data_t* source, data_t* dest, int n);

#define NUM_BASE 16

void sort_m(data_t* A, int p, int r) 
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
    sort_m(A, p, q);
    sort_m(A, q + 1, r);
    merge_m(A, p, q, r);
  }
}

// A merge routine. Merges the sub-arrays A [p..q] and A [q + 1..r].
// Uses two arrays 'left' and 'right' in the merge operation.
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
  mem_free(&left);
}

static inline void copy_m(data_t* source, data_t* dest, int n) {
  assert(dest);
  assert(source);

  for (int i = 0 ; i < n ; i++) {
    dest[i] = source[i];
  }
}
