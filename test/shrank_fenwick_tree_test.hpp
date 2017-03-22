#ifndef __SHRANK_FENWICK_TREE_TEST_H__
#define __SHRANK_FENWICK_TREE_TEST_H__

#include <gtest/gtest.h>
#include <cstdint>
#include "./test_utils.hpp"

extern std::uint64_t inc1[];
extern std::uint64_t inc2[];

TEST(shrank_fenwick_tree, increments_by_one)
{
    // height 0       1      0        2      0       1      0         3      0       1      0        2      0       1      0
    // node  15      14     13       12     11      10      9         8      7       6      5        4      3       2      1
    //   000001 0000010 000001 00000100 000001 0000010 000001 000001000 000001 0000010 000001 00000100 000001 0000010 000001
    ShrankFenwickTree_Test t(inc1, 15);

    // bit_count
    EXPECT_EQ(6*8 + 7*4 + 8*2 + 9, t.bit_count());

    std::string tree_str = "00000100000100000010000010000000100000100000010000010000000010000010000001000001000000010000010000001";
    EXPECT_EQ(tree_str, tree_tostring(t.tree, 6*8 + 7*4 + 8*2 + 9));

    std::uint64_t seq1[15];
    increments_to_sequence(inc1, seq1, 15);

    // get
    for (size_t i = 0; i < 15; i++)
        EXPECT_EQ(seq1[i], t.get(i)) << "at index " << i;

    // find
    for (std::uint64_t i = 0; i < 15; i++)
        EXPECT_EQ(i, t.find(seq1[i])) << "at index " << i;
    for (std::uint64_t i = seq1[14]; i < 100; i++)
        EXPECT_EQ(14, t.find(i)) << "at index " << i;

    // set
    for (size_t i = 0; i < 15; i++)
        t.set(i, i);

    size_t sum = 0;
    for (size_t i = 0; i < 15; i++) {
        sum += i;
        EXPECT_EQ(seq1[i]+sum, t.get(i)) << "at index " << i;
    }

    // find
    sum = 0;
    for (std::uint64_t i = 0; i < 15; i++) {
        sum += i;
        EXPECT_EQ(i, t.find(seq1[i]+sum)) << "at index " << i;
    }
}


TEST(shrank_fenwick_tree, increasing_increments)
{
    // height 0       1      0        2      0       1      0         3      0       1      0        2      0       1      0
    // node  15      14     13       12     11      10      9         8      7       6      5        4      3       2      1
    //   001111 0011011 001101 00101010 001011 0010011 001001 000100100 000111 0001011 000101 00001010 000011 0000011 000001
    ShrankFenwickTree_Test t(inc2, 15);

    // bit_count
    EXPECT_EQ(6*8 + 7*4 + 8*2 + 9, t.bit_count());

    std::string tree_str = "00111100110110011010010101000101100100110010010001001000001110001011000101000010100000110000011000001";
    EXPECT_EQ(tree_str, tree_tostring(t.tree, 6*8 + 7*4 + 8*2 + 9));

    std::uint64_t seq2[15];
    increments_to_sequence(inc2, seq2, 15);

    // get
    for (size_t i = 0; i < 15; i++)
        EXPECT_EQ(seq2[i], t.get(i)) << "at index " << i;

    // find
    for (std::uint64_t i = 0; i < 15; i++)
        EXPECT_EQ(i, t.find(seq2[i])) << "at index " << i;
    for (std::uint64_t i = seq2[14]; i < 1000; i++)
        EXPECT_EQ(14, t.find(i)) << "at index " << i;

    // set
    for (size_t i = 0; i < 15; i++)
        t.set(i, i);

    size_t sum = 0;
    for (size_t i = 0; i < 15; i++) {
        sum += i;
        EXPECT_EQ(seq2[i]+sum, t.get(i)) << "at index " << i;
    }

    // find
    sum = 0;
    for (std::uint64_t i = 0; i < 15; i++) {
        sum += i;
        EXPECT_EQ(i, t.find(seq2[i]+sum)) << "at index " << i;
    }
}


TEST(shrank_fenwick_tree, non_complete)
{
    // height 2      0       1      0          4      0       1      0        2      0       1      0         3      0       1      0        2      0       1      0
    // node  20     19      18     17         16     15      14     13       12     11      10      9         8      7       6      5        4      3       2      1
    // 00000100 000001 0000010 000001 0000010000 000001 0000010 000001 00000100 000001 0000010 000001 000001000 000001 0000010 000001 00000100 000001 0000010 000001
    ShrankFenwickTree_Test t(inc3, 20);

    // bit_count
    EXPECT_EQ(6*10 + 7*5 + 8*3 + 9*1 + 10*1, t.bit_count());

    std::string tree_str = "000001000000010000010000001000001000000000100000100000010000010000000100000100000010000010000000010000010000001000001000000010000010000001";
    EXPECT_EQ(tree_str, tree_tostring(t.tree, 6*10 + 7*5 + 8*3 + 9*1 + 10*1));

    std::uint64_t seq3[20];
    increments_to_sequence(inc3, seq3, 20);

    // get
    for (size_t i = 0; i < 20; i++)
        EXPECT_EQ(seq3[i], t.get(i)) << "at index " << i;

    // find
    for (std::uint64_t i = 0; i < 20; i++)
        EXPECT_EQ(i, t.find(seq3[i])) << "at index " << i;
    for (std::uint64_t i = seq3[19]; i < 1000; i++)
        EXPECT_EQ(19, t.find(i)) << "at index " << i;

    // set
    for (size_t i = 0; i < 20; i++)
        t.set(i, i);

    size_t sum = 0;
    for (size_t i = 0; i < 20; i++) {
        sum += i;
        EXPECT_EQ(seq3[i]+sum, t.get(i)) << "at index " << i;
    }

    // find
    sum = 0;
    for (std::uint64_t i = 0; i < 20; i++) {
        sum += i;
        EXPECT_EQ(i, t.find(seq3[i]+sum)) << "at index " << i;
    }
}


#endif // __SHRANK_FENWICK_TREE_TEST_H__
