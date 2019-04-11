#ifndef __TEST_FENWICK_NAIVE_HPP__
#define __TEST_FENWICK_NAIVE_HPP__

#include "../utils.hpp"

extern std::uint64_t inc1[];
extern std::uint64_t inc2[];

TEST(naive_fenwick_tree, increments_by_one)
{
    FixedF_Test t(inc1, 15);

    std::uint64_t seq1[15];
    increments_to_sequence(inc1, seq1, 15);

    // prefix
    for (size_t i = 0; i < 15; i++)
        EXPECT_EQ(seq1[i], t.prefix(i+1)) << "at index " << i;

    // find
    for (std::uint64_t i = 0; i < 15; i++)
        EXPECT_EQ(i+1, t.find(seq1[i])) << "at index " << i;
    for (std::uint64_t i = seq1[14]; i < 100; i++)
        EXPECT_EQ(15, t.find(i)) << "at index " << i;

    // find complement
    for (std::uint64_t i = 0; i < 63; i++)
        EXPECT_EQ(0, t.compFind(i)) << "at index " << i;
    for (std::uint64_t k = 1; k < 15; k++) {
        for (std::uint64_t i = 64*k-seq1[k-1]; i < 64*(k+1)-seq1[k]; i++)
            EXPECT_EQ(k, t.compFind(i)) << "at index " << i;
    }

    // add
    for (size_t i = 0; i < 15; i++)
        t.add(i+1, i);

    size_t sum = 0;
    for (size_t i = 0; i < 15; i++) {
        sum += i;
        EXPECT_EQ(seq1[i]+sum, t.prefix(i+1)) << "at index " << i;
    }

    // find
    sum = 0;
    for (std::uint64_t i = 0; i < 15; i++) {
        sum += i;
        EXPECT_EQ(i+1, t.find(seq1[i]+sum)) << "at index " << i;
    }
}


TEST(naive_fenwick_tree, increasing_increments)
{
    FixedF_Test t(inc2, 15);

    std::uint64_t seq2[15];
    increments_to_sequence(inc2, seq2, 15);

    // prefix
    for (size_t i = 0; i < 15; i++)
        EXPECT_EQ(seq2[i], t.prefix(i+1)) << "at index " << i;

    // find
    for (std::uint64_t i = 0; i < 15; i++)
        EXPECT_EQ(i+1, t.find(seq2[i])) << "at index " << i;
    for (std::uint64_t i = seq2[14]; i < 1000; i++)
        EXPECT_EQ(15, t.find(i)) << "at index " << i;

    // find complement
    for (std::uint64_t i = 0; i < 63; i++)
        EXPECT_EQ(0, t.compFind(i)) << "at index " << i;
    for (std::uint64_t k = 1; k < 15; k++) {
        for (std::uint64_t i = 64*k-seq2[k-1]; i < 64*(k+1)-seq2[k]; i++)
            EXPECT_EQ(k, t.compFind(i)) << "at index " << i;
    }

    // add
    for (size_t i = 0; i < 15; i++)
        t.add(i+1, i);

    size_t sum = 0;
    for (size_t i = 0; i < 15; i++) {
        sum += i;
        EXPECT_EQ(seq2[i]+sum, t.prefix(i+1)) << "at index " << i;
    }

    // find
    sum = 0;
    for (std::uint64_t i = 0; i < 15; i++) {
        sum += i;
        EXPECT_EQ(i+1, t.find(seq2[i]+sum)) << "at index " << i;
    }
}


TEST(naive_fenwick_tree, non_complete)
{
    FixedF_Test t(inc3, 20);

    std::uint64_t seq3[20];
    increments_to_sequence(inc3, seq3, 20);

    // prefix
    for (size_t i = 0; i < 20; i++)
        EXPECT_EQ(seq3[i], t.prefix(i+1)) << "at index " << i;

    // find
    for (std::uint64_t i = 0; i < 20; i++)
        EXPECT_EQ(i+1, t.find(seq3[i])) << "at index " << i;
    for (std::uint64_t i = seq3[19]; i < 1000; i++)
        EXPECT_EQ(20, t.find(i)) << "at index " << i;

    // find complement
    for (std::uint64_t i = 0; i < 63; i++)
        EXPECT_EQ(0, t.compFind(i)) << "at index " << i;
    for (std::uint64_t k = 1; k < 20; k++) {
        for (std::uint64_t i = 64*k-seq3[k-1]; i < 64*(k+1)-seq3[k]; i++)
            EXPECT_EQ(k, t.compFind(i)) << "at index " << i;
    }

    // add
    for (size_t i = 0; i < 20; i++)
        t.add(i+1, i);

    size_t sum = 0;
    for (size_t i = 0; i < 20; i++) {
        sum += i;
        EXPECT_EQ(seq3[i]+sum, t.prefix(i+1)) << "at index " << i;
    }

    // find
    sum = 0;
    for (std::uint64_t i = 0; i < 20; i++) {
        sum += i;
        EXPECT_EQ(i+1, t.find(seq3[i]+sum)) << "at index " << i;
    }
}


#endif // __TEST_FENWICK_NAIVE_HPP__
