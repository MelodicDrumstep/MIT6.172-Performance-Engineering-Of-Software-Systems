#include "./bitarray.h"
#include <stdio.h>
#include <stdlib.h>

// Helper function to print the bits for verification
void print_bits(const char* message, const bitarray_t* bitarray) {
    size_t bit_sz = bitarray_get_bit_sz(bitarray);
    printf("%s: ", message);
    for (size_t i = 0; i < bit_sz; i++) {
        printf("%d", bitarray_get(bitarray, i));
        if ((i + 1) % 8 == 0 && i + 1 != bit_sz)
            printf(" ");
    }
    printf("\n");
}

// Function to run tests on the bitarray_reverse function
void run_tests() {
    // Test 1: Basic full reverse
    bitarray_t* bitarray1 = bitarray_new(8);
    bitarray_set(bitarray1, 1, true);
    bitarray_set(bitarray1, 3, true);
    bitarray_set(bitarray1, 5, true);
    bitarray_set(bitarray1, 7, true);
    bitarray_reverse(bitarray1, 0, 8);
    print_bits("Full reverse", bitarray1);
    bitarray_free(bitarray1);

    // Test 2: Reverse part of the array
    bitarray_t* bitarray2 = bitarray_new(16);
    for (size_t i = 0; i < 8; i++) {
        bitarray_set(bitarray2, i, true);  // Set first 8 bits
    }
    bitarray_reverse(bitarray2, 4, 8);
    print_bits("Partial reverse", bitarray2);
    bitarray_free(bitarray2);

    // Test 3: Reverse with offset and length resulting in no change
    bitarray_t* bitarray3 = bitarray_new(16);
    for (size_t i = 0; i < 16; i++) {
        bitarray_set(bitarray3, i, (i % 2) == 0);  // Set alternate bits
    }
    bitarray_reverse(bitarray3, 8, 8);
    print_bits("Offset no-change reverse", bitarray3);
    bitarray_free(bitarray3);

    // Test 4: Empty reverse (should handle gracefully)
    bitarray_t* bitarray4 = bitarray_new(8);
    bitarray_reverse(bitarray4, 0, 0);
    print_bits("Empty reverse", bitarray4);
    bitarray_free(bitarray4);
}

int main() {
    run_tests();
    return 0;
}
