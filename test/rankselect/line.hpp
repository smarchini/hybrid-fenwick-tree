#ifndef __TEST_RANKSELECT_LINE_H__
#define __TEST_RANKSELECT_LINE_H__

#include "../utils.hpp"

TEST(line_rank_select, all_ones_1024)
{
    constexpr size_t MAX = 16;
    std::uint64_t bitvect[MAX] = { -1ULL, -1ULL, -1ULL, -1ULL, -1ULL, -1ULL, -1ULL, -1ULL,
                                   -1ULL, -1ULL, -1ULL, -1ULL, -1ULL, -1ULL, -1ULL, -1ULL };

    dyn::LineRankSelect<dyn::SimpleFenwickTree, 8> simple(bitvect, MAX);
    dyn::LineRankSelect<dyn::TypedFenwickTree, 8> typed(bitvect, MAX);
    dyn::LineRankSelect<dyn::ITypedFenwickTree, 8> ityped(bitvect, MAX);
    dyn::LineRankSelect<dyn::ByteFenwickTree, 8> byte(bitvect, MAX);
    dyn::LineRankSelect<dyn::IByteFenwickTree, 8> ibyte(bitvect, MAX);
    dyn::LineRankSelect<dyn::CompactFenwickTree, 8> compact(bitvect, MAX);
    dyn::LineRankSelect<dyn::ShrankFenwickTree, 8> shrank(bitvect, MAX);


    for (size_t i = 0; i <= 1024; i++) {
        EXPECT_EQ(i, simple.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, typed.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, ityped.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, byte.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, ibyte.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, compact.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, shrank.rank(i)) << "at index: " << i;
    }

    for (size_t i = 0; i <= 1024; i++) {
        EXPECT_EQ(0, simple.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(0, typed.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(0, ityped.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(0, byte.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(0, ibyte.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(0, compact.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(0, shrank.rankZero(i)) << "at index: " << i;
    }

    for (size_t i = 0; i < 1024; i++) {
        EXPECT_EQ(i, simple.select(i)) << "at index: " << i;
        EXPECT_EQ(i, typed.select(i)) << "at index: " << i;
        EXPECT_EQ(i, ityped.select(i)) << "at index: " << i;
        EXPECT_EQ(i, byte.select(i)) << "at index: " << i;
        EXPECT_EQ(i, ibyte.select(i)) << "at index: " << i;
        EXPECT_EQ(i, compact.select(i)) << "at index: " << i;
        EXPECT_EQ(i, shrank.select(i)) << "at index: " << i;
    }

    for (size_t i = 0; i < 10; i++) {
        EXPECT_EQ(-1ULL, simple.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(-1ULL, typed.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(-1ULL, ityped.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(-1ULL, byte.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(-1ULL, ibyte.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(-1ULL, compact.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(-1ULL, shrank.selectZero(i)) << "at index: " << i;
    }

    for (size_t i = 0; i < MAX; i++) {
        simple.update(i, 1);
        typed.update(i, 1);
        ityped.update(i, 1);
        byte.update(i, 1);
        ibyte.update(i, 1);
        compact.update(i, 1);
        shrank.update(i, 1);
    }

    for (size_t i = 0; i < MAX; i++) {
        EXPECT_EQ(i*64, simple.select(i)) << "at index: " << i;
        EXPECT_EQ(i*64, typed.select(i)) << "at index: " << i;
        EXPECT_EQ(i*64, ityped.select(i)) << "at index: " << i;
        EXPECT_EQ(i*64, byte.select(i)) << "at index: " << i;
        EXPECT_EQ(i*64, ibyte.select(i)) << "at index: " << i;
        EXPECT_EQ(i*64, compact.select(i)) << "at index: " << i;
        EXPECT_EQ(i*64, shrank.select(i)) << "at index: " << i;
    }

    for (size_t i = 0; i < MAX; i++) {
        simple.update(i, -1ULL);
        typed.update(i, -1ULL);
        ityped.update(i, -1ULL);
        byte.update(i, -1ULL);
        ibyte.update(i, -1ULL);
        compact.update(i, -1ULL);
        shrank.update(i, -1ULL);
    }

    for (size_t i = 0; i <= 1024; i++) {
        EXPECT_EQ(i, simple.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, typed.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, ityped.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, byte.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, ibyte.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, compact.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, shrank.rank(i)) << "at index: " << i;
    }
}


TEST(line_rank_select, all_zeroes_1024)
{
    constexpr size_t MAX = 16;
    std::uint64_t bitvect[MAX] = { 0, 0, 0, 0, 0, 0, 0, 0,
                                   0, 0, 0, 0, 0, 0, 0, 0 };

    dyn::LineRankSelect<dyn::SimpleFenwickTree, 2> simple(bitvect, MAX);
    dyn::LineRankSelect<dyn::TypedFenwickTree, 2> typed(bitvect, MAX);
    dyn::LineRankSelect<dyn::ITypedFenwickTree, 2> ityped(bitvect, MAX);
    dyn::LineRankSelect<dyn::ByteFenwickTree, 2> byte(bitvect, MAX);
    dyn::LineRankSelect<dyn::IByteFenwickTree, 2> ibyte(bitvect, MAX);
    dyn::LineRankSelect<dyn::CompactFenwickTree, 2> compact(bitvect, MAX);
    dyn::LineRankSelect<dyn::ShrankFenwickTree, 2> shrank(bitvect, MAX);

    for (size_t i = 0; i <= 1024; i++) {
        EXPECT_EQ(0, simple.rank(i)) << "at index: " << i;
        EXPECT_EQ(0, typed.rank(i)) << "at index: " << i;
        EXPECT_EQ(0, ityped.rank(i)) << "at index: " << i;
        EXPECT_EQ(0, byte.rank(i)) << "at index: " << i;
        EXPECT_EQ(0, ibyte.rank(i)) << "at index: " << i;
        EXPECT_EQ(0, compact.rank(i)) << "at index: " << i;
        EXPECT_EQ(0, shrank.rank(i)) << "at index: " << i;
    }

    for (size_t i = 0; i <= 1024; i++) {
        EXPECT_EQ(i, simple.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(i, typed.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(i, ityped.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(i, byte.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(i, ibyte.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(i, compact.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(i, shrank.rankZero(i)) << "at index: " << i;
    }

    for (size_t i = 0; i < 1024; i++) {
        EXPECT_EQ(-1ULL, simple.select(i)) << "at index: " << i;
        EXPECT_EQ(-1ULL, typed.select(i)) << "at index: " << i;
        EXPECT_EQ(-1ULL, ityped.select(i)) << "at index: " << i;
        EXPECT_EQ(-1ULL, byte.select(i)) << "at index: " << i;
        EXPECT_EQ(-1ULL, ibyte.select(i)) << "at index: " << i;
        EXPECT_EQ(-1ULL, compact.select(i)) << "at index: " << i;
        EXPECT_EQ(-1ULL, shrank.select(i)) << "at index: " << i;
    }

    for (size_t i = 0; i < 1024; i++) {
        EXPECT_EQ(i, simple.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(i, typed.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(i, ityped.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(i, byte.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(i, ibyte.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(i, compact.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(i, shrank.selectZero(i)) << "at index: " << i;
    }

     for (size_t i = 0; i < MAX; i++) {
         simple.update(i, 0b10);
         typed.update(i, 0b10);
         ityped.update(i, 0b10);
         byte.update(i, 0b10);
         ibyte.update(i, 0b10);
         compact.update(i, 0b10);
         shrank.update(i, 0b10);
     }

     for (size_t i = 0; i < MAX; i++) {
         EXPECT_EQ(i*64+1, simple.select(i)) << "at index: " << i;
         EXPECT_EQ(i*64+1, typed.select(i)) << "at index: " << i;
         EXPECT_EQ(i*64+1, ityped.select(i)) << "at index: " << i;
         EXPECT_EQ(i*64+1, byte.select(i)) << "at index: " << i;
         EXPECT_EQ(i*64+1, ibyte.select(i)) << "at index: " << i;
         EXPECT_EQ(i*64+1, compact.select(i)) << "at index: " << i;
         EXPECT_EQ(i*64+1, shrank.select(i)) << "at index: " << i;
     }

     for (size_t i = 0; i < MAX; i++) {
         simple.update(i, 0);
         typed.update(i, 0);
         ityped.update(i, 0);
         byte.update(i, 0);
         ibyte.update(i, 0);
         compact.update(i, 0);
         shrank.update(i, 0);
     }

     for (size_t i = 0; i <= 1024; i++) {
         EXPECT_EQ(0, simple.rank(i)) << "at index: " << i;
         EXPECT_EQ(0, typed.rank(i)) << "at index: " << i;
         EXPECT_EQ(0, ityped.rank(i)) << "at index: " << i;
         EXPECT_EQ(0, byte.rank(i)) << "at index: " << i;
         EXPECT_EQ(0, ibyte.rank(i)) << "at index: " << i;
         EXPECT_EQ(0, compact.rank(i)) << "at index: " << i;
         EXPECT_EQ(0, shrank.rank(i)) << "at index: " << i;
     }

}

#endif // __TEST_RANKSELECT_LINE_H__
