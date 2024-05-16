#include <stdio.h>
#include <sys/mman.h>

int main()
{
    int N = 5;
    int * ptr = mmap(NULL, N  * sizeof(int), PROT_READ | PROT_WRITE, MAP_PRIVATE, 0, 0);
    if(ptr == MAP_FAILED)
    {
        printf("Mapping Failed\n");
        return 1;
    }

    for(int i = 0; i < N; i++)
    {
        printf("[%d]", ptr[i]);
    }

    return 0;
}
