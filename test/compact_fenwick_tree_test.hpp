#ifndef __COMPACT_FENWICK_TREE_TEST_H__
#define __COMPACT_FENWICK_TREE_TEST_H__

#include <gtest/gtest.h>
#include <cstdint>
#include "./test_utils.hpp"

extern std::uint64_t inc1[];
extern std::uint64_t inc2[];

TEST(compact_fenwick_tree, increments_by_one)
{
    // heigth  3 |                 2 |                               1 |                                                       0
    // node   15 |       14       13 |      12      11      10       9 |      8      7      6      5      4      3      2      1
    // 000001000 | 00000100 00000100 | 0000010 0000010 0000010 0000010 | 000001 000001 000001 000001 000001 000001 000001 000001
    CompactFenwickTree_Test t(inc1, 15);

    std::string tree_str = "00000100000000100000001000000010000001000000100000010000001000001000001000001000001000001000001000001";
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


TEST(compact_fenwick_tree, increasing_increments)
{
    // heigth  3 |                 2 |                               1 |                                                       0
    // node   15 |       14       13 |      12      11      10       9 |      8      7      6      5      4      3      2      1
    // 000100100 | 00101010 00001010 | 0011011 0010011 0001011 0000011 | 001111 001101 001011 001001 000111 000101 000011 000001
    CompactFenwickTree_Test t(inc2, 15);

    std::string tree_str = "00010010000101010000010100011011001001100010110000011001111001101001011001001000111000101000011000001";
    EXPECT_EQ(tree_str, tree_tostring(t.tree, 6*8 + 7*4 + 8*2 + 9));

    std::uint64_t seq2[15];
    increments_to_sequence(inc2, seq2, 15);

    // get
    for (size_t i = 0; i < 15; i++)
        EXPECT_EQ(seq2[i], t.get(i)) << "at index " << i;

    // find
    for (std::uint64_t i = 0; i < 15; i++)
        EXPECT_EQ(i, t.find(seq2[i])) << "at index " << i;
    //for (std::uint64_t i = seq2[14]; i < 1000; i++)
    //    EXPECT_EQ(14, t.find(i)) << "at index " << i;

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

#endif // __COMPACT_FENWICK_TREE_TEST_H__
