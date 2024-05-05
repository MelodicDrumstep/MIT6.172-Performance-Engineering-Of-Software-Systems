#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./bitarray.h"  // Assuming your bitarray implementation is in this header

typedef struct {
    bitarray_t* bitarray;
    char expected[256];  // To hold the expected bit string for easy comparison
} TestCase;

void initialize_bitarray(TestCase* test, const char* bits) {
    size_t length = strlen(bits);
    if (test->bitarray) {
        //bitarray_free(test->bitarray);
    }
    test->bitarray = bitarray_new(length);
    for (size_t i = 0; i < length; i++) {
        bitarray_set(test->bitarray, i, bits[i] == '1');
    }
}

void rotate_bitarray(TestCase* test, int offset, int length, int amount) {
    bitarray_rotate(test->bitarray, offset, length, amount);
}

void expect_bitarray(TestCase* test, const char* expected) {
    strcpy(test->expected, expected);
    char actual[256];
    for (size_t i = 0; i < bitarray_get_bit_sz(test->bitarray); i++) {
        actual[i] = bitarray_get(test->bitarray, i) ? '1' : '0';
    }
    actual[bitarray_get_bit_sz(test->bitarray)] = '\0';

    if (strcmp(actual, expected) == 0) {
        printf("Test passed. Expected and actual bitarrays match: %s\n", actual);
    } else {
        printf("Test failed. Expected: %s, Got: %s\n", expected, actual);
    }
}

void run_tests() {
    TestCase tests[3];

    // Test 0
    initialize_bitarray(&tests[0], "10010110");
    rotate_bitarray(&tests[0], 0, 8, -1);
    expect_bitarray(&tests[0], "00101101");

    // Test 1
    initialize_bitarray(&tests[1], "10010110");
    rotate_bitarray(&tests[1], 2, 5, 2);
    expect_bitarray(&tests[1], "10110100");

    // Test 2
    initialize_bitarray(&tests[2], "10000101");
    rotate_bitarray(&tests[2], 0, 8, 0);
    expect_bitarray(&tests[2], "10000101");

    rotate_bitarray(&tests[2], 0, 8, 1);
    expect_bitarray(&tests[2], "11000010");

    rotate_bitarray(&tests[2], 0, 8, -1);
    expect_bitarray(&tests[2], "10000101");

    rotate_bitarray(&tests[2], 0, 8, -1);
    expect_bitarray(&tests[2], "00001011");

    rotate_bitarray(&tests[2], 0, 8, -11);
    expect_bitarray(&tests[2], "01011000");
}

int main() {
    run_tests();
    return 0;
}
