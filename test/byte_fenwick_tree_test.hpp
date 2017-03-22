#ifndef __BYTE_FENWICK_TREE_TEST_H__
#define __BYTE_FENWICK_TREE_TEST_H__

#include <gtest/gtest.h>
#include <cstdint>
#include "./test_utils.hpp"

extern std::uint64_t inc1[];
extern std::uint64_t inc2[];

TEST(byte_fenwick_tree, increments_by_one)
{
    // heigth         3 |                 2 |                                   1 |                                                                       0
    // node           8 |       12        4 |       14       10        6        2 |       15       13       11        9        7        5        3        1
    // 0000000000001000 | 00000100 00000100 | 00000010 00000010 00000010 00000010 | 00000001 00000001 00000001 00000001 00000001 00000001 00000001 00000001
    ByteFenwickTree_Test t(inc1, 15);

    // bit_count
    EXPECT_EQ(8*14 + 16, t.bit_count());

    std::string tree_str = "00000000000010000000010000000100000000100000001000000010000000100000000100000001000000010000000100000001000000010000000100000001";
    EXPECT_EQ(tree_str, tree_tostring(t.tree, 1*8*8 + 1*8*4 + 1*8*2 + 2*8*1));

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


TEST(byte_fenwick_tree, increasing_increments)
{
    // heigth         3 |                 2 |                                   1 |                                                                       0
    // node           8 |       12        4 |       14       10        6        2 |       15       13       11        9        7        5        3        1
    // 0000000000100100 | 00101010 00001010 | 00011011 00010011 00001011 00000011 | 00001111 00001101 00001011 00001001 00000111 00000101 00000011 00000001
    ByteFenwickTree_Test t(inc2, 15);

    // bit_count
    EXPECT_EQ(8*14 + 16, t.bit_count());

    std::string tree_str = "00000000001001000010101000001010000110110001001100001011000000110000111100001101000010110000100100000111000001010000001100000001";
    EXPECT_EQ(tree_str, tree_tostring(t.tree, 1*8*8 + 1*8*4 + 1*8*2 + 2*8*1));

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


TEST(byte_fenwick_tree, non_complete)
{
    // heigth         4 |                3 |                          2 |                                            1 |                                                                                         0
    // node          16 |                8 |       20       12        4 |       18       14       10        6        2 |       19       17       15       13       11        9       7         5        3        1
    // 0000000000010000 | 0000000000001000 | 00000100 00000100 00000100 | 00000010 00000010 00000010 00000010 00000010 | 00000001 00000001 00000001 00000001 00000001 00000001 00000001 00000001 00000001 00000001
    ByteFenwickTree_Test t(inc3, 20);

    // bit_count
    EXPECT_EQ(8*10 + 8*5 + 8*3 + 16*1 + 16*1, t.bit_count());

    std::string tree_str = "00000000000100000000000000001000000001000000010000000100000000100000001000000010000000100000001000000001000000010000000100000001000000010000000100000001000000010000000100000001";
    EXPECT_EQ(tree_str, tree_tostring(t.tree, 16*1 + 16*1 + 8*10 + 8*5 + 8*3));

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

#endif // __BYTE_FENWICK_TREE_TEST_H__
