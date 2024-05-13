这个 project 是编写一个高性能的单线程 heap allocator.

## 前置知识

### sbrk & mmap

`sbrk` 是用于调整 heap 大小的一个 system call， 可以理解为调整管理 heap 大小的指针。 最初版本的内存分配都使用 `brk / sbrk`.

`mmap` 用于将文件映射到进程的地址空间中， 或者创建一个匿名映射（没有备份的文件的映射， 相当于直接分配内存）。

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