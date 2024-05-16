#include <stdio.h>
#include <unistd.h>

int _mnned = 1024;

int main()
{
    int n;
    char * stg;
    for(n = 1; ; ++n)
    {
        stg = sbrk(8000);
        if(stg == (char *)(-1))
        {
            break;
        }
    }
    printf("%d 8000-byte blocks could be alllocated by sbrk. \n", n);
}