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