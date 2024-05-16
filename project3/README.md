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

