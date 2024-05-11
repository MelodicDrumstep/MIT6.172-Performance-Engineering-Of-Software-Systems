/**
 * Copyright (c) 2015 MIT License by 6.172 Staff
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

#include <stdio.h>

#include "./allocator_interface.h"
#include "./fasttime.h"
#include "./memlib.h"

#define NUM_ALLOCS 17
#define NUM_ITERATIONS 1 << 17

const malloc_impl_t* mem_impl;
int verbose = 0;

int main() {
  mem_init();

  mem_impl = &my_impl;
  mem_impl->init();

  void* allocs[NUM_ALLOCS];

  fasttime_t begin = gettime();
  for (int iter = 0; iter < NUM_ITERATIONS; iter++) {
    for (int i = 0; i < NUM_ALLOCS; i++) {
      allocs[i] = mem_impl->malloc(1 << i);
    }
    for (int i = 0; i < NUM_ALLOCS; i++) {
      mem_impl->free(allocs[i]);
    }
  }
  fasttime_t end = gettime();

  mem_deinit();

  printf("total runtime: %fs\n", tdiff(begin, end));
  printf("total mem usage: %d bytes\n", (int)mem_heapsize());
}
