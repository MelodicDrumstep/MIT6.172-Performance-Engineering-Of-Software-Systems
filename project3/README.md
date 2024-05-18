这个 project 是编写一个高性能的单线程 heap allocator.

## 前置知识

### sbrk & mmap

`sbrk` 是用于调整 heap 大小的一个 system call， 可以理解为调整管理 heap 大小的指针。 最初版本的内存分配都使用 `brk / sbrk`.

`mmap` 用于将文件映射到进程的地址空间中， 或者创建一个匿名映射（没有备份的文件的映射， 相当于直接分配内存）。

#### brk

对于 `brk / sbrk`, 可以看这个例子：

``` c
#include <stdio.h>
#include <unistd.h>

int main(int argc, char * argv[])
{
    long int page_size = sysconf(_SC_PAGESIZE);
    printf("Page size : %ld\n", page_size);
    void * c1 = sbrk(0);
    printf("program break address : %p\n", c1);
    printf("size of char : %lu\n", sizeof(char));
    c1 = (void * ) ((char * ) c1 + 1);
    printf("now the new c1 is : %p\n", c1);
    brk(c1);
    void * c2 = sbrk(0);
    printf("the new program break address is : %p\n", c2);
}
```

它的输出是这样的：

```
Page size : 4096
program break address : 0x55c5b3efe000
size of char : 1
now the new c1 is : 0x55c5b3efe001
the new program break address is : 0x55c5b3efe001
```

#### mmap

mmap 的原型是这样的:

```c
#include <sys/mman.h> 

void *mmap(void *start, size_t length, int prot, int flags, 
           int fd, off_t offset); 


int munmap(void *start, size_t length); 
```

mmap 通过修改 Page table, 创建新的虚拟空间， 这块空间用于映射某一文件。 

mmap 使用的是 demand paging / lazy loading 的思想。 我们在一开始没有真正把 disk 里的内容读进内存， 而是程序需要使用这块内容的时候再做真正的 Loading.

另外， 如果参数加上 `MAP_ANONYMOUS` 这个 flag， 那就是一个匿名映射， 不对应任何文件， 相当于直接分配内存了。

## 主体任务

这个 Lab 需要我自己实现 `malloc`, `free`, `realloc`。 最初已经写好了一个 naive 版本的 malloc, 它长这样：

```c

// All blocks must have a specified minimum alignment.
// The alignment requirement (from config.h) is >= 8 bytes.
#ifndef ALIGNMENT
  #define ALIGNMENT 8
#endif

// Rounds up to the nearest multiple of ALIGNMENT.
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))

// The smallest aligned size that will hold a size_t value.
#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

//  malloc - Allocate a block by incrementing the brk pointer.
//  Always allocate a block whose size is a multiple of the alignment.
void* my_malloc(size_t size) {
  // We allocate a little bit of extra memory so that we can store the
  // size of the block we've allocated.  Take a look at realloc to see
  // one example of a place where this can come in handy.
  int aligned_size = ALIGN(size + SIZE_T_SIZE);

  // Expands the heap by the given number of bytes and returns a pointer to
  // the newly-allocated area.  This is a slow call, so you will want to
  // make sure you don't wind up calling it on every malloc.
  void* p = mem_sbrk(aligned_size);

  if (p == (void*) - 1) {
    // Whoops, an error of some sort occurred.  We return NULL to let
    // the client code know that we weren't able to allocate memory.
    return NULL;
  } else {
    // We store the size of the block we've allocated in the first
    // SIZE_T_SIZE bytes.
    *(size_t*)p = size;

    // Then, we return a pointer to the rest of the block of memory,
    // which is at least size bytes long.  We have to cast to uint8_t
    // before we try any pointer arithmetic because voids have no size
    // and so the compiler doesn't know how far to move the pointer.
    // Since a uint8_t is always one byte, adding SIZE_T_SIZE after
    // casting advances the pointer by SIZE_T_SIZE bytes.
    return (void*)((char*)p + SIZE_T_SIZE);
  }
}
```

可以看到， 这里面有些思想还是很可贵的：

(1) 记录 metadata, 比如这里多开一个 `size_t` 记录块大小， 方便 free。

(2) size 给 round 到对齐的大小， 保持内存对齐。

但是这个实现有一些明显缺陷：

(1) 每次分配都是调用 `mm_sbrk` 拓展整个堆的大小， 开销很大。

(2) 没有复用过 free 的内存块， 有很多碎片。

因此我很自然的一个改进方法就是， 我维护一个 free list， 然后每次找 first fit 的块。


### 主体代码

我给我的代码写了很详尽的注释：

#### init

```cpp
int my_init()
{
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

  return 0;
}
```

#### malloc

```cpp
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
  //int aligned_size = ALIGN(size + sizeof(size_t) + 2 * sizeof(FREE_MEM_BLOCK *));
  size_t aligned_size = ALIGN(size + SIZE_T_SIZE);

  /* ~~~~~~~~~~~~~~ Newly Added ~~~~~~~~~~~~*/

  //search for a free block in the free list
  //if there's no such block, allocate it
  FREE_MEM_BLOCK * pointer2Block = mem_list_head;
  while(pointer2Block != NULL)
  {
    if(pointer2Block -> size > aligned_size)
    {
      // This means I find a free block which can contain the newly 
      // demanded memory
      break;
    }

    assert(pointer2Block != pointer2Block -> next);

    pointer2Block = pointer2Block -> next;
  }
  FREE_MEM_BLOCK * new_block;

  if(pointer2Block != NULL)
  {
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

    new_block -> next = pointer2Block -> next; 
    new_block -> prev = pointer2Block -> prev;

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
    assert(new_block != new_block -> next);

    if(new_block -> next != NULL)
    {
      new_block -> next -> prev = new_block;
    }

    assert(new_block != new_block -> next);
    if(flag_head)
    {
      // Remember to renew the head block if needed
      mem_list_head = new_block;
      assert(mem_list_head -> prev == NULL);
      assert(mem_list_head != mem_list_head -> next);
    }
    if(flag_tail)
    {
      // Remember to renew the tail block if needed
      mem_list_tail = new_block;
      assert(mem_list_tail -> next == NULL);
    }
  }
  else
  {
    // This means that we have to allocate more memory
    // Now I decide to double the size of the heap
    // This strategy will amortize the time complexity

    size_t aligned_heap_size = ALIGN((my_heap_hi() - my_heap_lo() + 1) / 2) + aligned_size;
    // Get the aligned version of the size of the heap

    mem_sbrk(aligned_heap_size);
    mem_list_tail -> size += aligned_heap_size;
    pointer2Block = mem_list_tail;
    // expand the heap and renew the tail block information
    // Notice that there maybe some oppotunity for optimization here
    // To reduce the number of operations

    //Create a new FREE_MEM_BLOCK at the right location
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
      assert(mem_list_head -> prev == NULL);
    }

    mem_list_tail = new_block;
    assert(mem_list_tail -> next == NULL);
    // Change the information and maintain the head / tail of the list
  }

  if ((void *)pointer2Block == (void*) - 1) 
  {
    // Whoops, an error of some sort occurred.  We return NULL to let
    // the client code know that we weren't able to allocate memory.
    return NULL;
  } 
  else 
  {
    ALLOC_MEM_BLOCK * newly_alloced_block = (ALLOC_MEM_BLOCK * )pointer2Block;
    newly_alloced_block -> size = aligned_size;
    newly_alloced_block -> data = (char * )&(newly_alloced_block -> data);

    return (void*)(newly_alloced_block -> data);
  }
}
```

#### free

```cpp
// free a memory block
// I have already store the size of the block in the first SIZE_T_SIZE bytes
// Then just add a new block_node to the free list
void my_free(void * ptr) 
{
  ALLOC_MEM_BLOCK * alloced_mem_block_to_be_free = (ALLOC_MEM_BLOCK * )ptr;

    // Now create a new FREE_MEM_BLOCK right at "ptr"
  FREE_MEM_BLOCK * converted_free_mem_block = (FREE_MEM_BLOCK * )ptr;

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

  size_t size = alloced_mem_block_to_be_free -> size;

  // Now I will create a branch new FREE_MEM_BLOCK
  // at "converted_free_mem_block".
  converted_free_mem_block -> size = size - ALIGN(sizeof(FREE_MEM_BLOCK));
  converted_free_mem_block -> prev = prev_free_block;
  converted_free_mem_block -> next = next_free_block;

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
  if(prev_free_block != NULL)
  {
    prev_free_block -> next = converted_free_mem_block;
  }
  if(next_free_block != NULL)
  {
    next_free_block -> prev = converted_free_mem_block;
  }
}
```