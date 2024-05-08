
#include <stdio.h>
#include "./everybit/bitarray.h"
#include <math.h>
int reverse_bits(unsigned int x) 
{
    unsigned int reversed = 0;
    unsigned int bits = 8; 
    while(bits--) 
    {
        reversed = (reversed << 1) | (x & 1);
        x >>= 1;
    }
    return reversed;
}

int main() {
    printf("static const uint8_t REVERSE_BYTE_LOOKUP[256] = \n{\n");
    for(int i = 0; i < 256; i++) 
    {
        int to_be_reversed = i;
        int reversed = reverse_bits(to_be_reversed);
        printf("0x%02X", reversed); 

        if(i < 255) 
        {
            printf(", ");
        }
        if((i + 1) % 8 == 0) 
        {
            printf("\n");
        }
    }
    printf("};\n");
    return 0;
}