#include "./bitarray.h"
#include <stdio.h>
#include <stdlib.h>

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
    size_t test_lengths[6] = {8, 16, 16, 8, 16, 16}; // Lengths of the bitarrays for each test
    size_t reverse_offsets[6] = {0, 4, 8, 0, 6, 2}; // Offsets for reverse
    size_t reverse_lengths[6] = {8, 8, 8, 0, 8, 8}; // Lengths for reverse

    for (int test = 0; test < 6; test++) {
        bitarray_original = bitarray_new(test_lengths[test]);
        bitarray_naive = bitarray_new(test_lengths[test]);
        
        // Set up bit patterns
        for (size_t i = 0; i < test_lengths[test]; i++) {
            bool value = ((i % 2) == 0); // Alternate bits pattern
            bitarray_set(bitarray_original, i, value);
            bitarray_set(bitarray_naive, i, value);
        }

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
    }
}

int main() {
    run_tests();
    return 0;
}