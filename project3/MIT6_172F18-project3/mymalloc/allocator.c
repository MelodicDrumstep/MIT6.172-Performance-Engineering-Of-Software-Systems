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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "./allocator_interface.h"
#include "./memlib.h"
#include <assert.h>

// Don't call libc malloc!
#define malloc(...) (USE_MY_MALLOC)
#define free(...) (USE_MY_FREE)
#define realloc(...) (USE_MY_REALLOC)

// All blocks must have a specified minimum alignment.
// The alignment requirement (from config.h) is >= 8 bytes.
#ifndef ALIGNMENT
  #define ALIGNMENT 8
#endif

// Rounds up to the nearest multiple of ALIGNMENT.
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))

// The smallest aligned size that will hold a size_t value.
#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

// ------------------------------------------
// ------------------------------------------
// ------------------------------------------
// Defined By me

#define MYDEBUG

typedef struct free_mem_block FREE_MEM_BLOCK;

//Here's my data structure: a linked list node
struct free_mem_block
{
  size_t size;
  FREE_MEM_BLOCK * next;
  FREE_MEM_BLOCK * prev;
};

//I will create a linked list of free memory blocks

FREE_MEM_BLOCK * mem_list_head;
FREE_MEM_BLOCK * mem_list_tail;
//These will reside on the .bss segment

#define INIT_SIZE 2
// INIT_SIZE * ALIGNMENT will be the initial size of the heap

typedef struct alloced_mem_block ALLOC_MEM_BLOCK;

struct alloced_mem_block
{
  size_t size;
  FREE_MEM_BLOCK * next;
  FREE_MEM_BLOCK * prev;
  char * data;
};


// Defined by me
// ------------------------------------------
// ------------------------------------------
// ------------------------------------------


// check - This checks our invariant that the size_t header before every
// block points to either the beginning of the next block, or the end of the
// heap.
int my_check() 
{
  char* p;
  char* lo = (char*)mem_heap_lo();
  char* hi = (char*)mem_heap_hi() + 1;
  size_t size = 0;

  p = lo;
  while (lo <= p && p < hi) {
    size = ALIGN(*(size_t*)p + SIZE_T_SIZE);
    p += size;
  }

  if (p != hi) {
    printf("Bad headers did not end at heap_hi!\n");
    printf("heap_lo: %p, heap_hi: %p, size: %lu, p: %p\n", lo, hi, size, p);
    return -1;
  }

  return 0;
}

// init - Initialize the malloc package.  Called once before any other
// calls are made.  
// In my implementation, I allocate a head block_node and insert it there
// Then I allocate "ALIGNMENT * INIT_SIZE" memory as the initial size
// of the heap
int my_init() 
{
  mem_sbrk(sizeof(FREE_MEM_BLOCK));
  mem_list_head = (FREE_MEM_BLOCK * )mem_heap_lo();
  // I will locate any block_node at the beginning of this free memory block
  // Notice that the block_node itself takes memory

  mem_sbrk(ALIGNMENT * INIT_SIZE);
  mem_list_head -> size = ALIGNMENT * INIT_SIZE;
  mem_list_head -> next = NULL;
  mem_list_head -> prev = NULL;
  mem_list_tail = mem_list_head;
  // Allocate the initial size of the heap
  // And let the first node to be the head & tail of the list

  //DEBUGING
  #ifdef MYDEBUG
    printf("\n~~~~~~~~~~~~~~~~` Inside my_init():~~~~~~~~~~~~~~\n");
    printf("sizeof(FREE_MEM_BLOCK) = %lx\n", sizeof(FREE_MEM_BLOCK));
    printf("mem_list_head = %p\n", mem_list_head);
    printf("mem_heap_lo = %p\n", mem_heap_lo());
    printf("mem_heap_hi = %p\n", mem_heap_hi());
    printf("mem_heap_hi + 1 = %p\n", (char * )mem_heap_hi() + 1);
    printf("mem_heap_hi - mem_heap_lo = %lx\n", mem_heap_hi() - mem_heap_lo());
    printf("\n~~~~~~~~~~~~~~~~` Ending my_init():~~~~~~~~~~~~~~\n");
  #endif
  //DEBUGING

  return 0;
}

//  malloc - Allocate a block by incrementing the brk pointer.
//  Always allocate a block whose size is a multiple of the alignment.
void* my_malloc(size_t size) 
{
  // We allocate a little bit of extra memory so that we can store the
  // size of the block we've allocated.  Take a look at realloc to see
  // one example of a place where this can come in handy.

  // Here, I store the size of this block and 
  // a pointer to the previous free FREE_MEM_BLOCK node 
  // in order to facilitate the "free" function
  int aligned_size = ALIGN(size + SIZE_T_SIZE + 2 * sizeof(FREE_MEM_BLOCK *));

  /* ~~~~~~~~~~~~~~ Newly Added ~~~~~~~~~~~~*/

  //search for a free block in the free list
  //if there's no such block, allocate it
  FREE_MEM_BLOCK * pointer2Block = mem_list_head;
  while(pointer2Block != NULL)

  {
    // DEBUGING
    #ifdef MYDEBUG
      printf("Searching for the free block\n");
    #endif
    // DEBUGING

    if(pointer2Block -> size > aligned_size)
    {
      // This means I find a free block which can contain the newly 
      // demanded memory
      break;
    }

    assert(pointer2Block != pointer2Block -> next);

    pointer2Block = pointer2Block -> next;
  }

    // DEBUGING
    #ifdef MYDEBUG
      printf("Finish searching for the free block\n");
    #endif
    // DEBUGING

  FREE_MEM_BLOCK * new_block;

  if(pointer2Block != NULL)
  {

    // DEBUGING
    #ifdef MYDEBUG
      printf("STEP in the First case\n");
    #endif
    // DEBUGING

    // This means that we can find a suitable block
    char flag_head = pointer2Block == mem_list_head;
    // flag_head will record if it's the head of the list
    // If so, we have to renew the head of the list

    char flag_tail = pointer2Block == mem_list_tail;

    new_block = (FREE_MEM_BLOCK * )((char * )pointer2Block + aligned_size);
    // Now I will place the new memory block node at "(char * )pointer2Block + aligned_size"
    // Notice that "aligned_size" is the number of BYTE!!!
    // Thus I have to cast "pointer2Block" to be char * and then increment it
    // Otherwise, pointer2Block will increment (aligned_size * sizeof(FREE_MEM_BLOCK))
    // which is definitely wrong!!!

    assert(pointer2Block != pointer2Block -> next);
    assert(pointer2Block != new_block);

    new_block -> size = pointer2Block -> size - aligned_size;
    new_block -> next = pointer2Block -> next;
    assert(new_block != new_block -> next);
    new_block -> prev = pointer2Block -> prev;
    // Now set the new_block information


    if(new_block -> prev != NULL)
    {
      // Add this condition in case it's the head block
      new_block -> prev -> next = new_block;
    }

    if(new_block -> next != NULL)
    {
      new_block -> next -> prev = new_block;
    }

    if(flag_head)
    {
      // Remember to renew the head block if needed
      mem_list_head = new_block;
    }

    if(flag_tail)
    {
      // Remember to renew the tail block if needed
      mem_list_tail = new_block;
    }


    // DEBUGING
    #ifdef MYDEBUG
      printf("STEP out the First case\n");
    #endif
    // DEBUGING
  }
  else
  {

    // DEBUGING
    #ifdef MYDEBUG
      printf("STEP in the Second case\n");
    #endif
    // DEBUGING

    // This means that we have to allocate more memory
    // Now I decide to double the size of the heap
    // This strategy will amortize the time complexity

    int alloc_size = my_heap_hi() - my_heap_lo() + 1;
    if(aligned_size > alloc_size)
    {
      alloc_size = aligned_size;
    }

    size_t aligned_heap_size = ALIGN(alloc_size);
    // Get the aligned version of the size of the heap

    mem_sbrk(aligned_heap_size);
    mem_list_tail -> size += aligned_heap_size;
    pointer2Block = mem_list_tail;
    // expand the heap and renew the tail block information
    // Notice that there maybe some oppotunity for optimization here
    // To reduce the number of operations

    new_block = (FREE_MEM_BLOCK * )((char * )pointer2Block + aligned_size);
    new_block -> size = pointer2Block -> size - aligned_size;
    new_block -> next = pointer2Block -> next;
    assert(new_block -> next == NULL);
    new_block -> prev = pointer2Block -> prev;

    if(new_block -> prev != NULL)
    {
      new_block -> prev -> next = new_block;
    }
    if(new_block -> next != NULL)
    {
      new_block -> next -> prev = new_block;
    }

    if(mem_list_head == mem_list_tail)
    {
      mem_list_head = new_block; 
    }

    mem_list_tail = new_block;
    // Change the information and maintain the head / tail of the list

    // DEBUGING
    #ifdef MYDEBUG
      printf("STEP out the Second case\n");
    #endif
    // DEBUGING

  }

  /* ~~~~~~~~~~~~~~ Newly Added ~~~~~~~~~~~~*/

  if ((void *)pointer2Block == (void*) - 1) 
  {
    // Whoops, an error of some sort occurred.  We return NULL to let
    // the client code know that we weren't able to allocate memory.
    return NULL;
  } 
  else 
  {
    // We store the size of the block we've allocated in the first
    // SIZE_T_SIZE bytes.

    // Then, we return a pointer to the rest of the block of memory,
    // which is at least size bytes long.  We have to cast to uint8_t
    // before we try any pointer arithmetic because voids have no size
    // and so the compiler doesn't know how far to move the pointer.
    // Since a uint8_t is always one byte, adding SIZE_T_SIZE after
    // casting advances the pointer by SIZE_T_SIZE bytes.


    ALLOC_MEM_BLOCK * newly_alloced_block = (ALLOC_MEM_BLOCK * )pointer2Block;
    newly_alloced_block -> size = aligned_size;
    newly_alloced_block -> prev = pointer2Block -> prev;
    newly_alloced_block -> next = new_block;
    newly_alloced_block -> data = (char * )&(newly_alloced_block -> data);
    return (void*)(newly_alloced_block -> data);
  }
}

// free a memory block
// I have already store the size of the block in the first SIZE_T_SIZE bytes
// Then just add a new block_node to the free list
void my_free(void * ptr) 
{
  ALLOC_MEM_BLOCK * alloced_mem_block_to_be_free = (ALLOC_MEM_BLOCK * )ptr;
  FREE_MEM_BLOCK * prev_free_block = alloced_mem_block_to_be_free -> prev;
  FREE_MEM_BLOCK * next_free_block = alloced_mem_block_to_be_free -> next;
  size_t size = alloced_mem_block_to_be_free -> size;
  //char * data = alloced_mem_block_to_be_free -> data;
  FREE_MEM_BLOCK * converted_free_mem_block = (FREE_MEM_BLOCK * )ptr;
  converted_free_mem_block -> size = size - sizeof(FREE_MEM_BLOCK);
  converted_free_mem_block -> prev = prev_free_block;
  converted_free_mem_block -> next = next_free_block;

  if(prev_free_block == NULL)
  {
    mem_list_head = converted_free_mem_block;
  }

  if(next_free_block == NULL)
  {
    mem_list_tail = converted_free_mem_block;
  }
}

// realloc - Implemented simply in terms of malloc and free
void* my_realloc(void* ptr, size_t size) 
{
  void* newptr;
  size_t copy_size;

  // Allocate a new chunk of memory, and fail if that allocation fails.
  newptr = my_malloc(size);
  if (NULL == newptr) 
  {
    return NULL;
  }

  // Get the size of the old block of memory.  Take a peek at my_malloc(),
  // where we stashed this in the SIZE_T_SIZE bytes directly before the
  // address we returned.  Now we can back up by that many bytes and read
  // the size.
  copy_size = *(size_t*)((uint8_t*)ptr - SIZE_T_SIZE);

  // If the new block is smaller than the old one, we have to stop copying
  // early so that we don't write off the end of the new block of memory.
  if (size < copy_size) {
    copy_size = size;
  }

  // This is a standard library call that performs a simple memory copy.
  memcpy(newptr, ptr, copy_size);

  // Release the old block.
  my_free(ptr);

  // Return a pointer to the new block.
  return newptr;
}
