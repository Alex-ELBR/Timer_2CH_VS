#include <unity.h>


typedef struct {
    int8_t temp_min;
    int8_t temp_max;
} Range;

const Range test_modes[] = {
    {2, 5},
    {40, 45},
    {60, 65}
};

// Режим 0 (+2...+5 °C)
void test_mode_zero_limits(void) {
    TEST_ASSERT_EQUAL_INT8(2, test_modes[0].temp_min);
    TEST_ASSERT_EQUAL_INT8(5, test_modes[0].temp_max);
}

// Режим 2 (+60...+65 °C)
void test_mode_two_limits(void) {
    TEST_ASSERT_EQUAL_INT8(60, test_modes[2].temp_min);
    TEST_ASSERT_EQUAL_INT8(65, test_modes[2].temp_max);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_mode_zero_limits);
    RUN_TEST(test_mode_two_limits);
    
    return UNITY_END(); 
}
