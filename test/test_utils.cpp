#include "unity.h"
#include "utils.h"  // Include the header file with your function

void setUp(void) {
    // This function will be called before each test
}

void tearDown(void) {
    // This function will be called after each test
}

void test_getBits(void) {
    // Test case 1: Basic case
    unsigned result1 = getBits(0b11011010, 3, 4);
    TEST_ASSERT_EQUAL_UINT(result1, 0b1010);

    // Test case 2: Another example
    unsigned result2 = getBits(0b10101010, 1, 3);
    TEST_ASSERT_EQUAL_UINT(result2, 0b010);

    // Test case 3: All bits requested
    unsigned result3 = getBits(0b1111, 0, 4);
    TEST_ASSERT_EQUAL_UINT(result3, 0b1111);

    // Test case 4: Offset exceeds bits
    unsigned result4 = getBits(0b10101010, 10, 3);
    TEST_ASSERT_EQUAL_UINT(result4, 0);

    // Add more test cases as needed
}

// Add more test functions if needed

int main(void) {
    UNITY_BEGIN();

    // Run the tests
    RUN_TEST(test_getBits);

    return UNITY_END();
}
