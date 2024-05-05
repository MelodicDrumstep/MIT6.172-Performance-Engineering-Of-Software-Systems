#include <stdio.h>
#include "./bitarray.h"  // Make sure this is the correct path to your bitarray header

// Function to print the bitarray for visualization
void bitarray_print(const bitarray_t* const bitarray);

// Function to run tests on the bitarray_rotate function
void run_rotate_tests() {
    printf("Running bitarray_rotate tests...\n");

    // Create a new bitarray
    bitarray_t* bitarray = bitarray_new(16);
    if (!bitarray) {
        printf("Failed to allocate bitarray.\n");
        return;
    }

    // Fill the bitarray for a clear pattern: 0101010101010101
    for (size_t i = 0; i < 16; i++) {
        bitarray_set(bitarray, i, i % 2);
    }

    // Print the original bitarray
    printf("Original bitarray: ");
    bitarray_print(bitarray);

    // Rotate right by 1 bit
    bitarray_rotate(bitarray, 0, 16, 1);
    printf("Rotate right by 1: ");
    bitarray_print(bitarray);

    // Rotate right by 15 bits, which should reverse the 1-bit rotation
    bitarray_rotate(bitarray, 0, 16, 15);
    printf("Rotate right by 15 (undo rotation): ");
    bitarray_print(bitarray);

    // Rotate right by 16 bits, should be the same as the original
    bitarray_rotate(bitarray, 0, 16, 16);
    printf("Rotate right by 16 (full rotation): ");
    bitarray_print(bitarray);

    // Rotate by a negative amount, rotate left by 2
    bitarray_rotate(bitarray, 0, 16, -2);
    printf("Rotate left by 2: ");
    bitarray_print(bitarray);

    // Rotate by larger than the bitarray size
    bitarray_rotate(bitarray, 0, 16, 32); // Same as no rotation
    printf("Rotate right by 32 (no change): ");
    bitarray_print(bitarray);

    // Free the bitarray
    bitarray_free(bitarray);

    printf("Rotation tests complete.\n");
}

int main() {
    run_rotate_tests();
    return 0;
}
