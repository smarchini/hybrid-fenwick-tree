#ifndef __TEST_FENWICK_NAIVE_H__
#define __TEST_FENWICK_NAIVE_H__

#include "../utils.hpp"

extern std::uint64_t inc1[];
extern std::uint64_t inc2[];

TEST(naive_fenwick_tree, increments_by_one)
{
    NaiveFenwickTree_Test t(inc1, 15);

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
        EXPECT_EQ(-1ULL, t.find_complement(i)) << "at index " << i;
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


TEST(naive_fenwick_tree, increasing_increments)
{
    NaiveFenwickTree_Test t(inc2, 15);

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
        EXPECT_EQ(-1ULL, t.find_complement(i)) << "at index " << i;
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


TEST(naive_fenwick_tree, non_complete)
{
    NaiveFenwickTree_Test t(inc3, 20);

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
        EXPECT_EQ(-1ULL, t.find_complement(i)) << "at index " << i;
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


#endif // __TEST_FENWICK_NAIVE_H__
