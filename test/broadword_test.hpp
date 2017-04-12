#include <gtest/gtest.h>

#include "../include/broadword.hpp"

/**
 * Some 64 bit random binary sequences for testing purpose. A random binary
 * sequence can be generated with the following python code:
 *
 * from random import randint
 * for idx in range(10):
 *     r = randint(0, 63)
 *     print('val{} = 0b'.format(idx), end='')
 *     for _ in range(63 - r): print('{}'.format(randint(0,1)), end='')
 *     print('1' + '0'*r)
 *
 * Note: Binary literals require C++14
 */
constexpr std::size_t TESTS_LENGTH = 10;
static std::uint64_t tests[] = {
    0b1111010100101011010101110101011010100111010001000000000000000000ULL,
    0b0111011100011111000011000111000100110110101001011110100000000000ULL,
    0b1001100000111110001000000000000000000000000000000000000000000000ULL,
    0b1000100000000000000000000000000000000000000000000000000000000000ULL,
    0b0010000110011100000000000000000000000000000000000000000000000000ULL,
    0b0010110110111111011111111100001100100000000000000000000000000000ULL,
    0b0010001100001111101011000010000111111100000000000000000000000000ULL,
    0b0101111111001110011100011011010010111101011010010101011101110000ULL,
    0b1100110011101111111001100011000000000000000000000000000000000000ULL,
    0b1111011010000100110011101111111111010000000000000000000000000000ULL
};

constexpr std::size_t SPECIAL_CASES_LENGTH = 6;
static std::uint64_t special_cases[] = {
    0b0000000000000000000000000000000000000000000000000000000000000000ULL,
    0b0000000000000000000000000000000011111111111111111111111111111111ULL,
    0b1111111111111111111111111111111100000000000000000000000000000000ULL,
    0b1111111111111111111111111111111111111111111111111111111111111111ULL,
    0b1000000000000000000000000000000000000000000000000000000000000000ULL,
    0b0000000000000000000000000000000010000000000000000000000000000000ULL
};


TEST(broadword, drop_first_set)
{
    using namespace dyn;
    std::uint64_t tests_solutions[] = {
        0b1111010100101011010101110101011010100111010000000000000000000000ULL,
        0b0111011100011111000011000111000100110110101001011110000000000000ULL,
        0b1001100000111110000000000000000000000000000000000000000000000000ULL,
        0b1000000000000000000000000000000000000000000000000000000000000000ULL,
        0b0010000110011000000000000000000000000000000000000000000000000000ULL,
        0b0010110110111111011111111100001100000000000000000000000000000000ULL,
        0b0010001100001111101011000010000111111000000000000000000000000000ULL,
        0b0101111111001110011100011011010010111101011010010101011101100000ULL,
        0b1100110011101111111001100010000000000000000000000000000000000000ULL,
        0b1111011010000100110011101111111111000000000000000000000000000000ULL
    };

    for (std::size_t i = 0; i < TESTS_LENGTH; i++)
        EXPECT_EQ(tests_solutions[i], drop_first_set(tests[i])) << " at index " << i;

    std::uint64_t special_cases_solutions[] = {
        0b0000000000000000000000000000000000000000000000000000000000000000ULL,
        0b0000000000000000000000000000000011111111111111111111111111111110ULL,
        0b1111111111111111111111111111111000000000000000000000000000000000ULL,
        0b1111111111111111111111111111111111111111111111111111111111111110ULL,
        0b0000000000000000000000000000000000000000000000000000000000000000ULL,
        0b0000000000000000000000000000000000000000000000000000000000000000ULL
    };

    for (std::size_t i = 0; i < SPECIAL_CASES_LENGTH; i++)
        EXPECT_EQ(special_cases_solutions[i], drop_first_set(special_cases[i])) << " at index " << i;
}


TEST(broadword, mask_first_set)
{
    using namespace dyn;
    std::uint64_t tests_solutions[] = {
        0b0000000000000000000000000000000000000000000001000000000000000000ULL,
        0b0000000000000000000000000000000000000000000000000000100000000000ULL,
        0b0000000000000000001000000000000000000000000000000000000000000000ULL,
        0b0000100000000000000000000000000000000000000000000000000000000000ULL,
        0b0000000000000100000000000000000000000000000000000000000000000000ULL,
        0b0000000000000000000000000000000000100000000000000000000000000000ULL,
        0b0000000000000000000000000000000000000100000000000000000000000000ULL,
        0b0000000000000000000000000000000000000000000000000000000000010000ULL,
        0b0000000000000000000000000001000000000000000000000000000000000000ULL,
        0b0000000000000000000000000000000000010000000000000000000000000000ULL
    };

    for (std::size_t i = 0; i < TESTS_LENGTH; i++)
        EXPECT_EQ(tests_solutions[i], mask_first_set(tests[i])) << " at index " << i;

    std::uint64_t special_cases_solutions[] = {
        0b0000000000000000000000000000000000000000000000000000000000000000ULL,
        0b0000000000000000000000000000000000000000000000000000000000000001ULL,
        0b0000000000000000000000000000000100000000000000000000000000000000ULL,
        0b0000000000000000000000000000000000000000000000000000000000000001ULL,
        0b1000000000000000000000000000000000000000000000000000000000000000ULL,
        0b0000000000000000000000000000000010000000000000000000000000000000ULL
    };

    for (std::size_t i = 1; i < SPECIAL_CASES_LENGTH; i++)
        EXPECT_EQ(special_cases_solutions[i], mask_first_set(special_cases[i])) << " at index " << i;
}


TEST(broadword, mask_last_set)
{
    using namespace dyn;
    std::uint64_t tests_solutions[] = {
        0b1000000000000000000000000000000000000000000000000000000000000000ULL,
        0b0100000000000000000000000000000000000000000000000000000000000000ULL,
        0b1000000000000000000000000000000000000000000000000000000000000000ULL,
        0b1000000000000000000000000000000000000000000000000000000000000000ULL,
        0b0010000000000000000000000000000000000000000000000000000000000000ULL,
        0b0010000000000000000000000000000000000000000000000000000000000000ULL,
        0b0010000000000000000000000000000000000000000000000000000000000000ULL,
        0b0100000000000000000000000000000000000000000000000000000000000000ULL,
        0b1000000000000000000000000000000000000000000000000000000000000000ULL,
        0b1000000000000000000000000000000000000000000000000000000000000000ULL
    };

    for (std::size_t i = 0; i < TESTS_LENGTH; i++)
        EXPECT_EQ(tests_solutions[i], mask_last_set(tests[i])) << " at index " << i;

    std::uint64_t special_cases_solutions[] = {
        0b0000000000000000000000000000000000000000000000000000000000000001ULL,
        0b0000000000000000000000000000000010000000000000000000000000000000ULL,
        0b1000000000000000000000000000000000000000000000000000000000000000ULL,
        0b1000000000000000000000000000000000000000000000000000000000000000ULL,
        0b1000000000000000000000000000000000000000000000000000000000000000ULL,
        0b0000000000000000000000000000000010000000000000000000000000000000ULL
    };

    // special_cases[0] is undefined
    for (std::size_t i = 1; i < SPECIAL_CASES_LENGTH; i++)
        EXPECT_EQ(special_cases_solutions[i], mask_last_set(special_cases[i])) << " at index " << i;
}


TEST(broadword, find_first_set)
{
    using namespace dyn;
    std::uint64_t tests_solutions[] = { 19, 12, 46, 60, 51, 30, 27, 5, 37, 29 };

    for (std::size_t i = 0; i < TESTS_LENGTH; i++)
        EXPECT_EQ(tests_solutions[i], find_first_set(tests[i])) << " at index " << i;

    std::uint64_t special_cases_solutions[] = { 0, 1, 33, 1, 64, 32 };

    for (std::size_t i = 0; i < SPECIAL_CASES_LENGTH; i++)
        EXPECT_EQ(special_cases_solutions[i], find_first_set(special_cases[i])) << " at index " << i;
}


TEST(broadword, find_last_set)
{
    using namespace dyn;
    std::uint64_t tests_solutions[] = { 64, 63, 64, 64, 62, 62, 62, 63, 64, 64 };

    for (std::size_t i = 0; i < TESTS_LENGTH; i++)
        EXPECT_EQ(tests_solutions[i], find_last_set(tests[i])) << " at index " << i;

    std::uint64_t special_cases_solutions[] = { 1, 32, 64, 64, 64, 32 };

    // special_cases[0] is undefined
    for (std::size_t i = 1; i < SPECIAL_CASES_LENGTH; i++)
        EXPECT_EQ(special_cases_solutions[i], find_last_set(special_cases[i])) << " at index " << i;

    std::uint64_t other_cases[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::uint64_t other_cases_solutions[] = { 1, 2, 2, 3, 3, 3, 3, 4, 4 };

    for (std::size_t i = 0; i < sizeof(other_cases) / sizeof(std::uint64_t); i++)
        EXPECT_EQ(other_cases_solutions[i], find_last_set(other_cases[i])) << " at index " << i;
}


TEST(broadword, compact_bitmask)
{
    using namespace dyn;
    constexpr std::size_t LEN = 10;
    std::size_t count[] = {  64, 63, 62, 61, 60, 59, 58, 57, 56, 55 };
    std::size_t start[] = {   0,  0,  0,  1,  1,  2,  2,  3,  3,  4 };

    std::uint64_t solution[] = {
        0b1111111111111111111111111111111111111111111111111111111111111111ULL,
        0b0111111111111111111111111111111111111111111111111111111111111111ULL,
        0b0011111111111111111111111111111111111111111111111111111111111111ULL,
        0b0011111111111111111111111111111111111111111111111111111111111110ULL,
        0b0001111111111111111111111111111111111111111111111111111111111110ULL,
        0b0001111111111111111111111111111111111111111111111111111111111100ULL,
        0b0000111111111111111111111111111111111111111111111111111111111100ULL,
        0b0000111111111111111111111111111111111111111111111111111111111000ULL,
        0b0000011111111111111111111111111111111111111111111111111111111000ULL,
        0b0000011111111111111111111111111111111111111111111111111111110000ULL,
    };

    for (std::size_t i = 0; i < LEN; i++)
        EXPECT_EQ(solution[i], compact_bitmask(count[i], start[i])) << " at index " << i;
}
