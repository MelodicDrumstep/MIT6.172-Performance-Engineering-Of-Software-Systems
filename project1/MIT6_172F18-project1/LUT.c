#include <stdio.h>
#include "./everybit/bitarray.h"
#include <math.h>

int main()
{
    printf("static const uint8_t REVERSE_BYTE_LOOKUP[256] = \n{\n");
    for(int i = 0; i < pow(2, 7); i++)
    {
        int to_be_reverse = i;
        bitarray_t * bits = bitarray_new(8);
        for(int j = 0; j < 8; j++)
        {
            bitarray_set(bits, 7 - j, (to_be_reverse & 1) == 1);
            to_be_reverse = to_be_reverse >> 1;
        }
        bitarray_reverse(bits, 0, 8);
        bitarray_print(bits);
        printf(", ");
        if(i % 8 == 7)
        {
            printf("\n");
        }
    }
    printf("};\n");
    return 0;
}