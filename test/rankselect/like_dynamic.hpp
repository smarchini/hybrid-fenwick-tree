#ifndef __TEST_RANKSELECT_LIKE_DYNAMIC_H__
#define __TEST_RANKSELECT_LIKE_DYNAMIC_H__

#include "../utils.hpp"
#include <dynamic.hpp>


TEST(rankselect_like_dynamic, random)
{
    using namespace std;
    using namespace dyn;

    static std::mt19937 mte;

    constexpr size_t SIZE = 1000;

    std::uint64_t *bitvect = new std::uint64_t[SIZE];
    std::uint64_t *updates = new std::uint64_t[SIZE];

    std::uniform_int_distribution<std::uint64_t> dist(0, UINT64_MAX);
    for (size_t i = 0; i < SIZE; i++) {
        bitvect[i] = dist(mte);
        updates[i] = dist(mte);
    }


    size_t ones = 0;
    for (size_t i = 0; i < SIZE; i++)
        ones += popcount(bitvect[i]);

    size_t zeroes = 64*SIZE - ones;

    WordRankSelect<NaiveFenwickTree> internal(bitvect, SIZE);

    suc_bv dynamic;
	for (uint64_t i = 0; i < SIZE; ++i) {
        for (uint64_t j = 0; j < 64; ++j)
            dynamic.insert(64*i + j, bitvect[i] & (1ULL << j));
    }

    for (uint64_t i = 0; i < SIZE; i++)
        EXPECT_EQ(dynamic.rank(i), internal.rank(i)) << "at index: " << i;

    for (uint64_t i = 0; i < SIZE; i++)
        EXPECT_EQ(dynamic.rank(i, 0), internal.rankZero(i)) << "at index: " << i;

    for (uint64_t i = 0; i < ones; i++)
        EXPECT_EQ(dynamic.select(i, 1), internal.select(i)) << "at index: " << i;

    for (uint64_t i = 0; i < zeroes; i++)
        EXPECT_EQ(dynamic.select(i, 0), internal.selectZero(i)) << "at index: " << i;


    delete[] updates;
    delete[] bitvect;
}

#endif // __TEST_RANKSELECT_LIKE_DYNAMIC_H__
