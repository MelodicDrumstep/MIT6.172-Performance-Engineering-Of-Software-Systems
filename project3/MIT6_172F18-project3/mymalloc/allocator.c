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
//#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))
#define SIZE_T_SIZE sizeof(size_t)

// ------------------------------------------
// ------------------------------------------
// ------------------------------------------
// Defined By me

//#define MYDEBUG
//#define TEST_ALIGNMENT

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

#define INIT_SIZE 4
// INIT_SIZE * ALIGNMENT will be the initial size of the heap

// Create this struct 
// for the convenience of manage Malloced Block
typedef struct alloced_mem_block ALLOC_MEM_BLOCK;

struct alloced_mem_block
{
  size_t size;
  // FREE_MEM_BLOCK * next;
  // FREE_MEM_BLOCK * prev;
  char * data;
};


// Defined by me
// ------------------------------------------
// ------------------------------------------
// ------------------------------------------

// check - This checks our invariant that the size_t header before every
// block points to either the beginning of the next block, or the end of the
// heap.

/*
This is my check function.

I will check :
(1) header will end at heap_hi or not
*/
int my_check() 
{

  #ifdef MYDEBUG
    printf("~~~~~~~~~~~~~~~~~~ Inside check function ~~~~~~~~~~~~~~~~\n");
  #endif

  char* p;
  char* lo = (char*)mem_heap_lo();
  char* hi = (char*)mem_heap_hi() + 1;
  size_t size = 0;

  p = lo;
  while (lo <= p && p < hi) 
  {
    size = ALIGN(*(size_t*)p + SIZE_T_SIZE);
    p += size;
  }

  if (p != hi) 
  {
    printf("Bad headers did not end at heap_hi!\n");
    printf("heap_lo: %p, heap_hi: %p, size: %lu, p: %p\n", lo, hi, size, p);

    #ifdef MYDEBUG
      printf("~~~~~~~~~~~~~~~~~~ Inside check function ~~~~~~~~~~~~~~~~\n");
    #endif

    return -1;
  }

  #ifdef MYDEBUG
    printf("~~~~~~~~~~~~~~~~~~ Outside check function ~~~~~~~~~~~~~~~~\n");
  #endif

  return 0;
}

/*
This is another check function to be inserted into the code
It will check :
(1) 
Whether there's a cycle in a list
(It should be no cycle)

(2)
Whether the linked list can form a linked list or not

(3)
Whether there's a 1-node cycle or not

*/
#ifdef MYDEBUG
void check_the_whole_list()
{
  printf("~~~~~~~~~~~~~~~~~~ Inside check_the_whole_list function ~~~~~~~~~~~~~~~~\n");
  FREE_MEM_BLOCK * fast = mem_list_head;
  FREE_MEM_BLOCK * slow = mem_list_head;
  while(fast != NULL && slow != NULL)
  {
    fast = fast -> next;
    if(fast == NULL)
    {
      break;
    }
    fast = fast -> next;
    slow = slow -> next;
    if(fast == NULL || slow == NULL)
    {
      break;
    }
    assert(fast != slow);
  }

  printf("Finish check 1\n");

  FREE_MEM_BLOCK * pointer2Block = mem_list_head;
  int cnt = 1;
  while(pointer2Block != NULL)
  {
    printf("This is the %d-th block.\n", cnt);
    printf("pointer2Block: %p\n", pointer2Block);
    printf("This time, pointer2Block -> size: %ld\n", pointer2Block -> size);
    if(pointer2Block -> next != NULL)
    {
      //if(pointer2Block -> next -> prev != pointer2Block)
      {
        printf("OH NO\n");
        printf("pointer2Block: %p\n", pointer2Block);
        printf("pointer2Block -> prev: %p\n", pointer2Block -> prev);
        printf("pointer2Block -> next: %p\n", pointer2Block -> next);
        printf("pointer2Block -> next -> prev: %p\n", pointer2Block -> next -> prev);
        if(pointer2Block -> next -> next != NULL)
        {
          printf("pointer2Block -> next -> next: %p\n", pointer2Block -> next -> next);
          printf("pointer2Block -> next -> next -> prev: %p\n", pointer2Block -> next -> next -> prev);
        }
      }
      printf("OH NONONO\n");

      printf("pointer2Block -> next - pointer2Block : %ld\n", (size_t)(pointer2Block -> next - pointer2Block));
      printf("pointer2Block -> size: %ld\n", pointer2Block -> size);
      printf("pointer2Block -> size + sizeof(FREE_MEM_BLOCK) : %ld\n", pointer2Block -> size + sizeof(FREE_MEM_BLOCK));
      assert(pointer2Block -> size + sizeof(FREE_MEM_BLOCK) <= (size_t)(pointer2Block -> next - pointer2Block));

      assert(pointer2Block -> next -> prev == pointer2Block);
    }
    if(pointer2Block -> prev == pointer2Block || pointer2Block -> next == pointer2Block)
    {
      printf("Damn, and cnt is : %d\n", cnt);
    }
    assert(pointer2Block -> prev != pointer2Block);
    assert(pointer2Block -> next != pointer2Block);
    pointer2Block = pointer2Block -> next;
    cnt++;
  }

  printf("Finish check 2\n");

  printf("~~~~~~~~~~~~~~~~~~ Outside check_the_whole_list function ~~~~~~~~~~~~~~~~\n");
}
#endif

// init - Initialize the malloc package.  Called once before any other
// calls are made.  
// In my implementation, I allocate a head block_node and insert it there
// Then I allocate "ALIGNMENT * INIT_SIZE" memory as the initial size
// of the heap
int my_init() 
{

  // DEBUGING
  #ifdef TEST_ALIGNMENT
    printf("size of FREE_MEM_BLOCK: %ld\n", sizeof(FREE_MEM_BLOCK));
    printf("size of FREE_MEM_BLOCK * : %ld\n", sizeof(FREE_MEM_BLOCK * ));
    printf("size of ALIGN FREE_MEM_BLOCK: %ld\n", ALIGN(sizeof(FREE_MEM_BLOCK)));
    printf("size of ALLOC_MEM_BLOCK: %ld\n", sizeof(ALLOC_MEM_BLOCK));
    printf("size of ALLOC_MEM_BLOCK * : %ld\n", sizeof(ALLOC_MEM_BLOCK * ));
    printf("size of ALIGN ALLOC_MEM_BLOCK: %ld\n", ALIGN(sizeof(ALLOC_MEM_BLOCK)));
  #endif

  mem_sbrk(ALIGN(sizeof(FREE_MEM_BLOCK)));
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
  // DEBUGING
  #ifdef MYDEBUG
    printf("~~~~~~~~~~~~~~~~~~ Inside my_malloc function ~~~~~~~~~~~~~~~~\n");
    check_the_whole_list();
  #endif
  // DEBUGING

  // We allocate a little bit of extra memory so that we can store the
  // size of the block we've allocated.  Take a look at realloc to see
  // one example of a place where this can come in handy.

  // Here, I store the size of this block and 
  // a pointer to the previous free FREE_MEM_BLOCK node 
  // in order to facilitate the "free" function
  //int aligned_size = ALIGN(size + sizeof(size_t) + 2 * sizeof(FREE_MEM_BLOCK *));
  size_t aligned_size = ALIGN(size + SIZE_T_SIZE);

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
      check_the_whole_list();
    #endif
    // DEBUGING

    
    // DEBUGING
    #ifdef MYDEBUG
      printf("check size -4\n");
      printf("mem_list_head -> size : %ld\n", mem_list_head -> size);
      printf("mem_list_tail -> size : %ld\n", mem_list_tail -> size);
    #endif
    // DEBUGING

    // This means that we can find a suitable block
    char flag_head = pointer2Block == mem_list_head;
    // flag_head will record if it's the head of the list
    // If so, we have to renew the head of the list

    char flag_tail = pointer2Block == mem_list_tail;

    // DEBUGING
    #ifdef MYDEBUG
      printf("Finish -2\n");
      check_the_whole_list();
    #endif
    // DEBUGING

    new_block = (FREE_MEM_BLOCK * )((char * )pointer2Block + aligned_size);
    // Now I will place the new memory block node at "(char * )pointer2Block + aligned_size"
    // Notice that "aligned_size" is the number of BYTE!!!
    // Thus I have to cast "pointer2Block" to be char * and then increment it
    // Otherwise, pointer2Block will increment (aligned_size * sizeof(FREE_MEM_BLOCK))
    // which is definitely wrong!!!

    assert(pointer2Block != pointer2Block -> next);
    assert(pointer2Block != new_block);

    // DEBUGING
    #ifdef MYDEBUG
      printf("Finish -1\n");
      check_the_whole_list();
    #endif
    // DEBUGING

    //new_block -> size = pointer2Block -> size - aligned_size;

    // DEBUGING
    #ifdef MYDEBUG
      printf("Finish 0\n");
      check_the_whole_list();
    #endif
    // DEBUGING

    if(pointer2Block -> next != NULL)
    {
      assert(pointer2Block -> next != pointer2Block -> next -> prev);
    }

    // DEBUGING
    #ifdef MYDEBUG
      printf("Hey, pointer2Block: %p\n", pointer2Block);
      printf("Hey, pointer2Block -> next : %p\n", pointer2Block -> next);
      printf("Hey, new_block : %p\n", new_block);
    #endif 
    // DEBUGING

    assert(pointer2Block -> next != new_block);

    new_block -> next = pointer2Block -> next;

    // DEBUGING
    #ifdef MYDEBUG
      printf("Hey, pointer2Block: %p\n", pointer2Block);
      printf("Hey, pointer2Block -> next : %p\n", pointer2Block -> next);
      printf("Hey, new_block : %p\n", new_block);
    #endif 
    // DEBUGING

    if(pointer2Block -> next != NULL)
    {
      assert(pointer2Block -> next != pointer2Block -> next -> prev);
    }

    if(new_block -> next != NULL)
    {
      assert(new_block -> next != new_block -> next -> prev);
    }

    // // DEBUGING
    // #ifdef MYDEBUG
    //   printf("Finish 1\n");
    //   if(pointer2Block == mem_list_head)
    //   {
    //     printf("pointer2Block == mem_list_head\n");
    //   }
    //   if(pointer2Block == mem_list_tail)
    //   {
    //     printf("pointer2Block == mem_list_tail\n");
    //   }
    //   check_the_whole_list();
    // #endif
    // // DEBUGING

    if(new_block -> next != NULL)
    {
      assert(new_block -> next != new_block -> next -> prev);
    }

    assert(new_block != new_block -> next);
    new_block -> prev = pointer2Block -> prev;

    // // DEBUGING
    // #ifdef MYDEBUG
    //   printf("Finish 2\n");
    //   check_the_whole_list();
    // #endif
    // // DEBUGING

    
    // DEBUGING
    #ifdef MYDEBUG
      printf("check size -3\n");
      printf("mem_list_head -> size : %ld\n", mem_list_head -> size);
      printf("mem_list_tail -> size : %ld\n", mem_list_tail -> size);
    #endif
    // DEBUGING

    assert(new_block != new_block -> prev);

    if(new_block -> next != NULL)
    {
      assert(new_block -> next != new_block -> next -> prev);
    }
    // Now set the new_block information

    if(new_block -> prev != NULL)
    {
      // Add this condition in case it's the head block
      new_block -> prev -> next = new_block;
    }

    
    // DEBUGING
    #ifdef MYDEBUG
      printf("check size -2\n");
      printf("mem_list_head -> size : %ld\n", mem_list_head -> size);
      printf("mem_list_tail -> size : %ld\n", mem_list_tail -> size);
    #endif
    // DEBUGING

    assert(new_block != new_block -> next);

    if(new_block -> next != NULL)
    {
      assert(new_block != new_block -> next);
      assert(new_block -> next != new_block -> next -> prev);
      assert(new_block -> next != new_block -> prev);

      new_block -> next -> prev = new_block;

      assert(new_block != new_block -> next);
      assert(new_block -> next != new_block -> prev);
    }

    assert(new_block != new_block -> next);


    
    // DEBUGING
    #ifdef MYDEBUG
      printf("check size -1\n");
      printf("mem_list_head -> size : %ld\n", mem_list_head -> size);
      printf("mem_list_tail -> size : %ld\n", mem_list_tail -> size);
    #endif
    // DEBUGING

    // // DEBUGING
    // #ifdef MYDEBUG
    //   printf("Finish 3\n");
    //   check_the_whole_list();
    // #endif
    // // DEBUGING

    if(flag_head)
    {
      // Remember to renew the head block if needed
      mem_list_head = new_block;
      assert(mem_list_head -> prev == NULL);
      assert(mem_list_head != mem_list_head -> next);
    }

    // // DEBUGING
    // #ifdef MYDEBUG
    //   printf("Finish 5\n");
    //   check_the_whole_list();
    // #endif
    // // DEBUGING

    if(flag_tail)
    {
      // Remember to renew the tail block if needed
      mem_list_tail = new_block;
      assert(mem_list_tail -> next == NULL);
    }




    // DEBUGING
    #ifdef MYDEBUG
      printf("Finish 6\n");
      check_the_whole_list();
    #endif
    // DEBUGING


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

    // size_t alloc_size = my_heap_hi() - my_heap_lo() + 1;
    // if(aligned_size > alloc_size)
    // {
    //   alloc_size = aligned_size;
    // }

    size_t aligned_heap_size = ALIGN((my_heap_hi() - my_heap_lo() + 1) / 2) + aligned_size;
    // Get the aligned version of the size of the heap

    mem_sbrk(aligned_heap_size);
    mem_list_tail -> size += aligned_heap_size;

    // DEBUGING
    #ifdef MYDEBUG
      printf("check size 0\n");
      printf("mem_list_head -> size : %ld\n", mem_list_head -> size);
      printf("mem_list_tail -> size : %ld\n", mem_list_tail -> size);
    #endif
    // DEBUGING

    pointer2Block = mem_list_tail;
    // expand the heap and renew the tail block information
    // Notice that there maybe some oppotunity for optimization here
    // To reduce the number of operations

    //Create a new FREE_MEM_BLOCK at the right location
    new_block = (FREE_MEM_BLOCK * )((char * )pointer2Block + aligned_size);
    new_block -> size = pointer2Block -> size - aligned_size;

    // DEBUGING
    #ifdef MYDEBUG
      printf("check size 1\n");
      printf("mem_list_head -> size : %ld\n", mem_list_head -> size);
      printf("mem_list_tail -> size : %ld\n", mem_list_tail -> size);
      printf("new_block -> size : %ld\n", new_block -> size);
    #endif
    // DEBUGING

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
      assert(mem_list_head -> prev == NULL);
    }

    mem_list_tail = new_block;
    assert(mem_list_tail -> next == NULL);
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

    // DEBUGING
    #ifdef MYDEBUG
      printf("Error\n");
      printf("~~~~~~~~~~~~~~~~~~ Outside my_malloc function ~~~~~~~~~~~~~~~~\n");
      check_the_whole_list();
    #endif
    // DEBUGING

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
    // newly_alloced_block -> prev = pointer2Block -> prev;
    // newly_alloced_block -> next = new_block;
    newly_alloced_block -> data = (char * )&(newly_alloced_block -> data);

    // DEBUGING
    #ifdef MYDEBUG
      printf("~~~~~~~~~~~~~~~~~~ Outside my_malloc function ~~~~~~~~~~~~~~~~\n");
      check_the_whole_list();
    #endif
    // DEBUGING

    return (void*)(newly_alloced_block -> data);
  }
}

// free a memory block
// I have already store the size of the block in the first SIZE_T_SIZE bytes
// Then just add a new block_node to the free list
void my_free(void * ptr) 
{

  // DEBUGING
  #ifdef MYDEBUG
    printf("~~~~~~~~~~~~~~~~~~ Inside my_free function ~~~~~~~~~~~~~~~~\n");
    check_the_whole_list();
  #endif
  // DEBUGING

  ALLOC_MEM_BLOCK * alloced_mem_block_to_be_free = (ALLOC_MEM_BLOCK * )ptr;

    // Now create a new FREE_MEM_BLOCK right at "ptr"
  FREE_MEM_BLOCK * converted_free_mem_block = (FREE_MEM_BLOCK * )ptr;
  // FREE_MEM_BLOCK * prev_free_block = alloced_mem_block_to_be_free -> prev;
  // FREE_MEM_BLOCK * next_free_block = alloced_mem_block_to_be_free -> next;

  FREE_MEM_BLOCK * prev_free_block;
  FREE_MEM_BLOCK * next_free_block;

  FREE_MEM_BLOCK * search_block = mem_list_head;

  // Search for two free blocks containing the malloced block
  if(search_block >= converted_free_mem_block)
  {
    // This means the malloced block is at the beginning
    // prior to any FREE_MEM_BLOCK
    prev_free_block = NULL;
    next_free_block = search_block;
  }
  else
  {
    while(search_block -> next != NULL)
    {
      // DEBUGING
      #ifdef MYDEBUG
        printf("Searching for the free blocks containing this memory block\n");
      #endif
      // DEBUGING

      assert(search_block != search_block -> next);
      if(search_block < converted_free_mem_block && search_block -> next > converted_free_mem_block)
      {
        break;
      }
      search_block = search_block -> next;
    }

    prev_free_block = search_block;
    next_free_block = search_block -> next;
  }

  // DEBUGING
  #ifdef MYDEBUG
    printf("Finish 0 FREE\n");
    check_the_whole_list();
  #endif
  // DEBUGING
  

  size_t size = alloced_mem_block_to_be_free -> size;

  // DEBUGING
  #ifdef MYDEBUG
    printf("The size of the memory block to be free is :%ld \n", size);
  #endif
  // DEBUGING

  //char * data = alloced_mem_block_to_be_free -> data;

  // DEBUGING
  #ifdef MYDEBUG
    printf("Finish 1 FREE\n");
    check_the_whole_list();
  #endif
  // DEBUGING

  // Now I will create a branch new FREE_MEM_BLOCK
  // at "converted_free_mem_block".
  converted_free_mem_block -> size = size - ALIGN(sizeof(FREE_MEM_BLOCK));
  converted_free_mem_block -> prev = prev_free_block;
  converted_free_mem_block -> next = next_free_block;

  // DEBUGING
  #ifdef MYDEBUG
    printf("Finish 2 FREE\n");
    check_the_whole_list();
  #endif
  // DEBUGING

  //And I maintain the free memory block linked list
  if(prev_free_block == NULL)
  {
    mem_list_head = converted_free_mem_block;
    assert(converted_free_mem_block -> prev == NULL);
  }

  if(next_free_block == NULL)
  {
    mem_list_tail = converted_free_mem_block;
    assert(converted_free_mem_block -> next == NULL);
  } 

  // // DEBUGING
  // #ifdef MYDEBUG
  //   printf("Finish 3 FREE\n");
  //   check_the_whole_list();
  // #endif
  // // DEBUGING

  if(prev_free_block != NULL)
  {
    prev_free_block -> next = converted_free_mem_block;
  }
  if(next_free_block != NULL)
  {
    next_free_block -> prev = converted_free_mem_block;
  }

  // DEBUGING
  #ifdef MYDEBUG
    printf("~~~~~~~~~~~~~~~~~~ Outside my_free function ~~~~~~~~~~~~~~~~\n");
    check_the_whole_list();
  #endif
  // DEBUGING

}

// realloc - Implemented simply in terms of malloc and free
void* my_realloc(void* ptr, size_t size) 
{
  if(ptr == NULL)
  {
    return my_malloc(size);
  }
  if(size == 0)
  {
    my_free(ptr);
    return NULL;
  }

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
  if (size < copy_size) 
  {
    copy_size = size;
  }

  // This is a standard library call that performs a simple memory copy.
  memcpy(newptr, ptr, copy_size);

  // Release the old block.
  my_free(ptr);

  // Return a pointer to the new block.
  return newptr;
}
