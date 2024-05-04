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

#include "./matrix_multiply.h"

#include "tbassert.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <math.h>
#include <string.h>
#include <immintrin.h>


// #include "./tbassert.h"

// Allocates a row-by-cols matrix and returns it
matrix* make_matrix(int rows, int cols) {
  matrix* new_matrix = malloc(sizeof(matrix));

  // Set the number of rows and columns
  new_matrix->rows = rows;
  new_matrix->cols = cols;

  // Allocate a buffer big enough to hold the matrix.
  new_matrix->values = (int**)malloc(sizeof(int*) * rows);
  for (int i = 0; i < rows; i++) {
    new_matrix->values[i] = (int*)malloc(sizeof(int) * cols);
  }

  for(int i = 0; i < rows * cols; i++) 
  {
    new_matrix -> values[i / cols][i % cols] = 0;
  }

  return new_matrix;
}

// Frees an allocated matrix
void free_matrix(matrix* m) {
  for (int i = 0; i < m->rows; i++) {
    free(m->values[i]);
  }
  free(m->values);
  free(m);
}

// Print matrix
void print_matrix(const matrix* m) {
  printf("------------\n");
  for (int i = 0; i < m->rows; i++) {
    for (int j = 0; j < m->cols; j++) {
      printf("  %3d  ", m->values[i][j]);
    }
    printf("\n");
  }
  printf("------------\n");
}


// Multiply matrix A*B, store result in C.
int matrix_multiply_run(const matrix* A, const matrix* B, matrix* C) 
{
  tbassert(A->cols == B->rows,
           "A->cols = %d, B->rows = %d\n", A->cols, B->rows);
  tbassert(A->rows == C->rows,
           "A->rows = %d, C->rows = %d\n", A->rows, C->rows);
  tbassert(B->cols == C->cols,
           "B->cols = %d, C->cols = %d\n", B->cols, C->cols);

  int block_size = 32;

  if(A -> rows * B -> cols > 1024 * 1024)
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
  }

  else
  {
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
  }

  return 0;
}
