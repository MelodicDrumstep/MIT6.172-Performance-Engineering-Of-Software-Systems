#include "./bitarray.h"
#include <stdio.h>
#include <stdlib.h>

#define num_test 9

// Helper function to print the bits for verification
void print_bits(const char* message, const bitarray_t* bitarray) {
    size_t bit_sz = bitarray_get_bit_sz(bitarray);
    printf("%s: \n", message);
    for (size_t i = 0; i < bit_sz; i++) {
        printf("%d", bitarray_get(bitarray, i));
        if ((i + 1) % 8 == 0 && i + 1 != bit_sz)
            printf(" ");
    }
    printf("\n");
}

// Function to run tests on the bitarray_reverse function
void run_tests() {
    bitarray_t *bitarray_original, *bitarray_naive;

    
    // Initialize test bitarrays
    size_t test_lengths[num_test] = {16, 16, 16, 16, 16, 16, 16, 16, 16}; // Lengths of the bitarrays for each test
    size_t reverse_offsets[num_test] = {0, 4, 8, 0, 6, 2, 0, 0, 15}; // Offsets for reverse
    size_t reverse_lengths[num_test] = {8, 8, 8, 0, 8, 8, 15, 16, 1}; // Lengths for reverse

    u_int8_t value1 = 0b10000101;
    u_int8_t value2 = 0b10000101;

    for (int test = 0; test < num_test; test++) 
    {
        bitarray_original = bitarray_new_with_2byte_value(value1, value2);
        bitarray_naive = bitarray_new_with_2byte_value(value1, value2);
    
        // Set up bit patterns

        // Reverse bits using both functions
        bitarray_reverse(bitarray_original, reverse_offsets[test], reverse_lengths[test]);
        bitarray_reverse_naive(bitarray_naive, reverse_offsets[test], reverse_lengths[test]);

        // Print results for comparison
        printf("Test %d:\n", test + 1);
        print_bits("Optimized Reverse", bitarray_original);
        print_bits("Naive Reverse", bitarray_naive);
        
        // Free resources
        bitarray_free(bitarray_original);
        bitarray_free(bitarray_naive);

        printf("\n\n\n");
    }
}

int main() {
    run_tests();
    return 0;
}