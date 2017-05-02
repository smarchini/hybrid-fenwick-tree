#ifndef __TEST_FENWICK_COMPACT_H__
#define __TEST_FENWICK_COMPACT_H__

#include "../utils.hpp"

extern std::uint64_t inc1[];
extern std::uint64_t inc2[];

TEST(compact_fenwick_tree, increments_by_one)
{
    // heigth   3 |                   2 |                                   1 |                                                               0
    // node     8 |         12        4 |          14      10       6       2 |      15      13      11       9       7       5       3       1
    // 0000001000 | 000000100 000000100 | 00000010 00000010 00000010 00000010 | 0000001 0000001 0000001 0000001 0000001 0000001 0000001 0000001
    CompactFenwickTree_Test t(inc1, 15);

    std::string tree_str = "00000010000000001000000001000000001000000010000000100000001000000010000001000000100000010000001000000100000010000001";
    EXPECT_EQ(tree_str, tree_tostring(t.tree.get(), tree_str.length()));

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

    // find complement
    for (std::uint64_t i = 0; i < 63; i++)
        EXPECT_EQ(-1ULL, t.find(i, true)) << "at index " << i;
    for (std::uint64_t k = 1; k < 15; k++) {
        for (std::uint64_t i = 64*k-seq1[k-1]; i < 64*(k+1)-seq1[k]; i++)
            EXPECT_EQ(k-1, t.find(i, true)) << "at index " << i;
    }

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
    // heigth   3 |                   2 |                                   1 |                                                               0
    // node     8 |         12        4 |          14      10       6       2 |      15      13      11       9       7       5       3       1
    // 0000100100 | 000101010 000001010 | 00011011 00010011 00001011 00000011 | 0001111 0001101 0001011 0001001 0000111 0000101 0000011 0000001
    CompactFenwickTree_Test t(inc2, 15);

    std::string tree_str = "00001001000001010100000010100001101100010011000010110000001100011110001101000101100010010000111000010100000110000001";
    EXPECT_EQ(tree_str, tree_tostring(t.tree.get(), tree_str.length()));

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

    // find complement
    for (std::uint64_t i = 0; i < 63; i++)
        EXPECT_EQ(-1ULL, t.find(i, true)) << "at index " << i;
    for (std::uint64_t k = 1; k < 15; k++) {
        for (std::uint64_t i = 64*k-seq2[k-1]; i < 64*(k+1)-seq2[k]; i++)
            EXPECT_EQ(k-1, t.find(i, true)) << "at index " << i;
    }

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


TEST(compact_fenwick_tree, non_complete)
{
    // heigth    4 |          3 |                             2 |                                            1 |                                                                               0
    // node     16 |          8 |          20       12        4 |           18      14      10       6       2 |      19      17      15      13      11       9       7       5       3       1
    // 00000010000 | 0000001000 | 000000100 000000100 000000100 | 00000010 00000010 00000010 00000010 00000010 | 0000001 0000001 0000001 0000001 0000001 0000001 0000001 0000001 0000001 0000001
    CompactFenwickTree_Test t(inc3, 20);

    std::string tree_str = "00000010000000000100000000010000000010000000010000000010000000100000001000000010000000100000001000000100000010000001000000100000010000001000000100000010000001";
    EXPECT_EQ(tree_str, tree_tostring(t.tree.get(), tree_str.length()));

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

    // find complement
    for (std::uint64_t i = 0; i < 63; i++)
        EXPECT_EQ(-1ULL, t.find(i, true)) << "at index " << i;
    for (std::uint64_t k = 1; k < 20; k++) {
        for (std::uint64_t i = 64*k-seq3[k-1]; i < 64*(k+1)-seq3[k]; i++)
            EXPECT_EQ(k-1, t.find(i, true)) << "at index " << i;
    }

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

#endif // __TEST_FENWICK_COMPACT_H__
