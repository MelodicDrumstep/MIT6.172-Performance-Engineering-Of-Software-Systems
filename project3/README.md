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

mmap 使用的是 demand paging / lazy loading 的思想。 我们在一开始没有真正把 disk 里的内容读进内存， 而是程序需要使用这块内容的时候再做真正的 Loading.

