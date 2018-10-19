#ifndef __TEST_FENWICK_TYPE_H__
#define __TEST_FENWICK_TYPE_H__

#include "../utils.hpp"

extern std::uint64_t inc1[];
extern std::uint64_t inc2[];

TEST(type_fenwick_tree, increments_by_one)
{
    // height 0        1        0                2        0        1        0                3        0        1        0                2        0        1        0
    // node  15       14       13               12       11       10        9                8        7        6        5                4        3        2        1
    // 00000001 00000010 00000001 0000000000000100 00000001 00000010 00000001 0000000000001000 00000001 00000010 00000001 0000000000000100 00000001 00000010 00000001
    TypeF_Test t(inc1, 15);

    std::string tree_str = "000000010000001000000001000000000000010000000001000000100000000100000000000010000000000100000010000000010000000000000100000000010000001000000001";

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
        EXPECT_EQ(SIZE_MAX, t.find_complement(i)) << "at index " << i;
    for (std::uint64_t k = 1; k < 15; k++) {
        for (std::uint64_t i = 64*k-seq1[k-1]; i < 64*(k+1)-seq1[k]; i++)
            EXPECT_EQ(k-1, t.find_complement(i)) << "at index " << i;
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


TEST(type_fenwick_tree, increasing_increments)
{
    // height 0        1        0                2        0        1        0                3        0        1        0                2        0        1        0
    // node  15       14       13               12       11       10        9                8        7        6        5                4        3        2        1
    // 00001111 00011011 00001101 0000000000101010 00001011 00010011 00001001 0000000000100100 00000111 00001011 00000101 0000000000001010 00000011 00000011 00000001
    TypeF_Test t(inc2, 15);

    std::string tree_str = "000011110001101100001101000000000010101000001011000100110000100100000000001001000000011100001011000001010000000000001010000000110000001100000001";

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
        EXPECT_EQ(SIZE_MAX, t.find_complement(i)) << "at index " << i;
    for (std::uint64_t k = 1; k < 15; k++) {
        for (std::uint64_t i = 64*k-seq2[k-1]; i < 64*(k+1)-seq2[k]; i++)
            EXPECT_EQ(k-1, t.find_complement(i)) << "at index " << i;
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

TEST(type_fenwick_tree, non_complete)
{
    // height 2       0        1       0           4       0        1       0         2       0        1       0          3       0        1       0         2       0        1       0
    // node  20      19       18      17          16      15       14      13        12      11       10       9          8       7        6       5         4       3        2       1
    // 00000100 00000001 00000010 00000001 0000000000010000 00000001 00000010 00000001 0000000000000100 00000001 00000010 00000001 0000000000001000 00000001 00000010 00000001 0000000000000100 00000001 00000010 00000001
    TypeF_Test t(inc3, 20);

    std::string tree_str = "000001000000000100000010000000010000000000010000000000010000001000000001000000000000010000000001000000100000000100000000000010000000000100000010000000010000000000000100000000010000001000000001";
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
        EXPECT_EQ(SIZE_MAX, t.find_complement(i)) << "at index " << i;
    for (std::uint64_t k = 1; k < 20; k++) {
        for (std::uint64_t i = 64*k-seq3[k-1]; i < 64*(k+1)-seq3[k]; i++)
            EXPECT_EQ(k-1, t.find_complement(i)) << "at index " << i;
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

#endif // __TEST_FENWICK_TYPE_H__
