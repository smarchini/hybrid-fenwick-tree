#ifndef __TEST_FENWICK_LBYTE_H__
#define __TEST_FENWICK_LBYTE_H__

#include "../utils.hpp"

extern std::uint64_t inc1[];
extern std::uint64_t inc2[];

TEST(lbyte_fenwick_tree, increments_by_one)
{
    // heigth         3 |                                 2 |                                   1 |                                                                       0
    // node           8 |               12                4 |       14       10        6        2 |       15       13       11        9        7        5        3        1
    // 0000000000001000 | 0000000000000100 0000000000000100 | 00000010 00000010 00000010 00000010 | 00000001 00000001 00000001 00000001 00000001 00000001 00000001 00000001
    ByteL_Test t(inc1, 15);

    std::string tree_str = "000000000000100000000000000001000000000000000100000000100000001000000010000000100000000100000001000000010000000100000001000000010000000100000001";
    EXPECT_EQ(tree_str, tree_tostring(t.tree.get(), tree_str.length()));

    std::uint64_t seq1[15];
    increments_to_sequence(inc1, seq1, 15);

    // prefix
    for (size_t i = 0; i < 15; i++)
        EXPECT_EQ(seq1[i], t.prefix(i)) << "at index " << i;

    // find
    for (std::uint64_t i = 0; i < 15; i++)
        EXPECT_EQ(i, t.find(seq1[i])) << "at index " << i;
    for (std::uint64_t i = seq1[14]; i < 100; i++)
        EXPECT_EQ(14, t.find(i)) << "at index " << i;

    // find complement
    for (std::uint64_t i = 0; i < 63; i++)
        EXPECT_EQ(SIZE_MAX, t.find_complement(i)) << "at index " << i;
    for (std::uint64_t k = 1; k < 15; k++) {
        for (std::uint64_t i = 64*k-seq1[k-1]; i < 64*(k+1)-seq1[k]; i++)
            EXPECT_EQ(k-1, t.find_complement(i)) << "at index " << i;
    }

    // add
    for (size_t i = 0; i < 15; i++)
        t.add(i, i);

    size_t sum = 0;
    for (size_t i = 0; i < 15; i++) {
        sum += i;
        EXPECT_EQ(seq1[i]+sum, t.prefix(i)) << "at index " << i;
    }

    // find
    sum = 0;
    for (std::uint64_t i = 0; i < 15; i++) {
        sum += i;
        EXPECT_EQ(i, t.find(seq1[i]+sum)) << "at index " << i;
    }
}


TEST(lbyte_fenwick_tree, increasing_increments)
{
    // heigth         3 |                                 2 |                                   1 |                                                                       0
    // node           8 |               12                4 |       14       10        6        2 |       15       13       11        9        7        5        3        1
    // 0000000000100100 | 0000000000101010 0000000000001010 | 00011011 00010011 00001011 00000011 | 00001111 00001101 00001011 00001001 00000111 00000101 00000011 00000001
    ByteL_Test t(inc2, 15);

    std::string tree_str = "000000000010010000000000001010100000000000001010000110110001001100001011000000110000111100001101000010110000100100000111000001010000001100000001";
    EXPECT_EQ(tree_str, tree_tostring(t.tree.get(), tree_str.length()));

    std::uint64_t seq2[15];
    increments_to_sequence(inc2, seq2, 15);

    // prefix
    for (size_t i = 0; i < 15; i++)
        EXPECT_EQ(seq2[i], t.prefix(i)) << "at index " << i;

    // find
    for (std::uint64_t i = 0; i < 15; i++)
        EXPECT_EQ(i, t.find(seq2[i])) << "at index " << i;
    for (std::uint64_t i = seq2[14]; i < 1000; i++)
        EXPECT_EQ(14, t.find(i)) << "at index " << i;

    // find complement
    for (std::uint64_t i = 0; i < 63; i++)
        EXPECT_EQ(SIZE_MAX, t.find_complement(i)) << "at index " << i;
    for (std::uint64_t k = 1; k < 15; k++) {
        for (std::uint64_t i = 64*k-seq2[k-1]; i < 64*(k+1)-seq2[k]; i++)
            EXPECT_EQ(k-1, t.find_complement(i)) << "at index " << i;
    }

    // add
    for (size_t i = 0; i < 15; i++)
        t.add(i, i);

    size_t sum = 0;
    for (size_t i = 0; i < 15; i++) {
        sum += i;
        EXPECT_EQ(seq2[i]+sum, t.prefix(i)) << "at index " << i;
    }

    // find
    sum = 0;
    for (std::uint64_t i = 0; i < 15; i++) {
        sum += i;
        EXPECT_EQ(i, t.find(seq2[i]+sum)) << "at index " << i;
    }
}


TEST(lbyte_fenwick_tree, non_complete)
{
    // heigth         4 |                3 |                                                  2 |                                            1 |                                                                                         0
    // node          16 |                8 |               20               12                4 |       18       14       10        6        2 |       19       17       15       13       11        9       7         5        3        1
    // 0000000000010000 | 0000000000001000 | 0000000000000100 0000000000000100 0000000000000100 | 00000010 00000010 00000010 00000010 00000010 | 00000001 00000001 00000001 00000001 00000001 00000001 00000001 00000001 00000001 00000001

    ByteL_Test t(inc3, 20);

    std::string tree_str = "00000000000100000000000000001000000000000000010000000000000001000000000000000100000000100000001000000010000000100000001000000001000000010000000100000001000000010000000100000001000000010000000100000001";
    EXPECT_EQ(tree_str, tree_tostring(t.tree.get(), tree_str.length()));

    std::uint64_t seq3[20];
    increments_to_sequence(inc3, seq3, 20);

    // prefix
    for (size_t i = 0; i < 20; i++)
        EXPECT_EQ(seq3[i], t.prefix(i)) << "at index " << i;

    // find
    for (std::uint64_t i = 0; i < 20; i++)
        EXPECT_EQ(i, t.find(seq3[i])) << "at index " << i;
    for (std::uint64_t i = seq3[19]; i < 1000; i++)
        EXPECT_EQ(19, t.find(i)) << "at index " << i;

    // find complement
    for (std::uint64_t i = 0; i < 63; i++)
        EXPECT_EQ(SIZE_MAX, t.find_complement(i)) << "at index " << i;
    for (std::uint64_t k = 1; k < 20; k++) {
        for (std::uint64_t i = 64*k-seq3[k-1]; i < 64*(k+1)-seq3[k]; i++)
            EXPECT_EQ(k-1, t.find_complement(i)) << "at index " << i;
    }

    // add
    for (size_t i = 0; i < 20; i++)
        t.add(i, i);

    size_t sum = 0;
    for (size_t i = 0; i < 20; i++) {
        sum += i;
        EXPECT_EQ(seq3[i]+sum, t.prefix(i)) << "at index " << i;
    }

    // find
    sum = 0;
    for (std::uint64_t i = 0; i < 20; i++) {
        sum += i;
        EXPECT_EQ(i, t.find(seq3[i]+sum)) << "at index " << i;
    }
}

#endif // __TEST_FENWICK_LBYTE_H__
