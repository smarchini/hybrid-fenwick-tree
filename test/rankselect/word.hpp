#ifndef __TEST_RANKSELECT_WORD_H__
#define __TEST_RANKSELECT_WORD_H__

#include "../utils.hpp"

TEST(word_rank_select, all_ones_1024)
{
    constexpr size_t MAX = 16;
    std::uint64_t bitvect[MAX] = { -1ULL, -1ULL, -1ULL, -1ULL, -1ULL, -1ULL, -1ULL, -1ULL,
                                   -1ULL, -1ULL, -1ULL, -1ULL, -1ULL, -1ULL, -1ULL, -1ULL };

    dyn::WordRankSelect<dyn::NaiveFenwickTree> naive(bitvect, MAX);
    dyn::WordRankSelect<dyn::LTypeFenwickTree> ltype(bitvect, MAX);
    dyn::WordRankSelect<dyn::TypeFenwickTree> type(bitvect, MAX);
    dyn::WordRankSelect<dyn::LByteFenwickTree> lbyte(bitvect, MAX);
    dyn::WordRankSelect<dyn::ByteFenwickTree> byte(bitvect, MAX);
    dyn::WordRankSelect<dyn::LBitFenwickTree> lbit(bitvect, MAX);
    dyn::WordRankSelect<dyn::BitFenwickTree> bit(bitvect, MAX);

    for (size_t i = 0; i <= 1024; i++) {
        EXPECT_EQ(i, naive.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, ltype.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, type.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, lbyte.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, byte.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, lbit.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, bit.rank(i)) << "at index: " << i;
    }

    for (size_t i = 0; i <= 1024; i++) {
        EXPECT_EQ(0, naive.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(0, ltype.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(0, type.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(0, lbyte.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(0, byte.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(0, lbit.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(0, bit.rankZero(i)) << "at index: " << i;
    }

    for (size_t i = 0; i < 1024; i++) {
        EXPECT_EQ(i, naive.select(i)) << "at index: " << i;
        EXPECT_EQ(i, ltype.select(i)) << "at index: " << i;
        EXPECT_EQ(i, type.select(i)) << "at index: " << i;
        EXPECT_EQ(i, lbyte.select(i)) << "at index: " << i;
        EXPECT_EQ(i, byte.select(i)) << "at index: " << i;
        EXPECT_EQ(i, lbit.select(i)) << "at index: " << i;
        EXPECT_EQ(i, bit.select(i)) << "at index: " << i;
    }

    for (size_t i = 0; i < 10; i++) {
        EXPECT_EQ(-1ULL, naive.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(-1ULL, ltype.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(-1ULL, type.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(-1ULL, lbyte.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(-1ULL, byte.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(-1ULL, lbit.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(-1ULL, bit.selectZero(i)) << "at index: " << i;
    }

    for (size_t i = 0; i < MAX; i++) {
        naive.update(i, 1);
        ltype.update(i, 1);
        type.update(i, 1);
        lbyte.update(i, 1);
        byte.update(i, 1);
        lbit.update(i, 1);
        bit.update(i, 1);
    }

    for (size_t i = 0; i < MAX; i++) {
        EXPECT_EQ(i*64, naive.select(i)) << "at index: " << i;
        EXPECT_EQ(i*64, ltype.select(i)) << "at index: " << i;
        EXPECT_EQ(i*64, type.select(i)) << "at index: " << i;
        EXPECT_EQ(i*64, lbyte.select(i)) << "at index: " << i;
        EXPECT_EQ(i*64, byte.select(i)) << "at index: " << i;
        EXPECT_EQ(i*64, lbit.select(i)) << "at index: " << i;
        EXPECT_EQ(i*64, bit.select(i)) << "at index: " << i;
    }

    for (size_t i = 0; i < MAX; i++) {
        naive.update(i, -1ULL);
        ltype.update(i, -1ULL);
        type.update(i, -1ULL);
        lbyte.update(i, -1ULL);
        byte.update(i, -1ULL);
        lbit.update(i, -1ULL);
        bit.update(i, -1ULL);
    }

    for (size_t i = 0; i <= 1024; i++) {
        EXPECT_EQ(i, naive.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, ltype.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, type.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, lbyte.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, byte.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, lbit.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, bit.rank(i)) << "at index: " << i;
    }

}


TEST(word_rank_select, all_zeroes_1024)
{
    constexpr size_t MAX = 16;
    std::uint64_t bitvect[MAX] = { 0, 0, 0, 0, 0, 0, 0, 0,
                                   0, 0, 0, 0, 0, 0, 0, 0 };

    dyn::WordRankSelect<dyn::NaiveFenwickTree> naive(bitvect, MAX);
    dyn::WordRankSelect<dyn::LTypeFenwickTree> ltype(bitvect, MAX);
    dyn::WordRankSelect<dyn::TypeFenwickTree> type(bitvect, MAX);
    dyn::WordRankSelect<dyn::LByteFenwickTree> lbyte(bitvect, MAX);
    dyn::WordRankSelect<dyn::ByteFenwickTree> byte(bitvect, MAX);
    dyn::WordRankSelect<dyn::LBitFenwickTree> lbit(bitvect, MAX);
    dyn::WordRankSelect<dyn::BitFenwickTree> bit(bitvect, MAX);

    for (size_t i = 0; i <= 1024; i++) {
        EXPECT_EQ(0, naive.rank(i)) << "at index: " << i;
        EXPECT_EQ(0, ltype.rank(i)) << "at index: " << i;
        EXPECT_EQ(0, type.rank(i)) << "at index: " << i;
        EXPECT_EQ(0, lbyte.rank(i)) << "at index: " << i;
        EXPECT_EQ(0, byte.rank(i)) << "at index: " << i;
        EXPECT_EQ(0, lbit.rank(i)) << "at index: " << i;
        EXPECT_EQ(0, bit.rank(i)) << "at index: " << i;
    }

    for (size_t i = 0; i <= 1024; i++) {
        EXPECT_EQ(i, naive.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(i, ltype.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(i, type.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(i, lbyte.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(i, byte.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(i, lbit.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(i, bit.rankZero(i)) << "at index: " << i;
    }

    for (size_t i = 0; i < 1024; i++) {
        EXPECT_EQ(-1ULL, naive.select(i)) << "at index: " << i;
        EXPECT_EQ(-1ULL, ltype.select(i)) << "at index: " << i;
        EXPECT_EQ(-1ULL, type.select(i)) << "at index: " << i;
        EXPECT_EQ(-1ULL, lbyte.select(i)) << "at index: " << i;
        EXPECT_EQ(-1ULL, byte.select(i)) << "at index: " << i;
        EXPECT_EQ(-1ULL, lbit.select(i)) << "at index: " << i;
        EXPECT_EQ(-1ULL, bit.select(i)) << "at index: " << i;
    }

    for (size_t i = 0; i < 1024; i++) {
        EXPECT_EQ(i, naive.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(i, ltype.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(i, type.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(i, lbyte.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(i, byte.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(i, lbit.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(i, bit.selectZero(i)) << "at index: " << i;
    }

     for (size_t i = 0; i < MAX; i++) {
         naive.update(i, 0b10);
         ltype.update(i, 0b10);
         type.update(i, 0b10);
         lbyte.update(i, 0b10);
         byte.update(i, 0b10);
         lbit.update(i, 0b10);
         bit.update(i, 0b10);
     }

     for (size_t i = 0; i < MAX; i++) {
         EXPECT_EQ(i*64+1, naive.select(i)) << "at index: " << i;
         EXPECT_EQ(i*64+1, ltype.select(i)) << "at index: " << i;
         EXPECT_EQ(i*64+1, type.select(i)) << "at index: " << i;
         EXPECT_EQ(i*64+1, lbyte.select(i)) << "at index: " << i;
         EXPECT_EQ(i*64+1, byte.select(i)) << "at index: " << i;
         EXPECT_EQ(i*64+1, lbit.select(i)) << "at index: " << i;
         EXPECT_EQ(i*64+1, bit.select(i)) << "at index: " << i;
     }

     for (size_t i = 0; i < MAX; i++) {
         naive.update(i, 0);
         ltype.update(i, 0);
         type.update(i, 0);
         lbyte.update(i, 0);
         byte.update(i, 0);
         lbit.update(i, 0);
         bit.update(i, 0);
     }

     for (size_t i = 0; i <= 1024; i++) {
         EXPECT_EQ(0, naive.rank(i)) << "at index: " << i;
         EXPECT_EQ(0, ltype.rank(i)) << "at index: " << i;
         EXPECT_EQ(0, type.rank(i)) << "at index: " << i;
         EXPECT_EQ(0, lbyte.rank(i)) << "at index: " << i;
         EXPECT_EQ(0, byte.rank(i)) << "at index: " << i;
         EXPECT_EQ(0, lbit.rank(i)) << "at index: " << i;
         EXPECT_EQ(0, bit.rank(i)) << "at index: " << i;
     }

}

#endif // __TEST_RANKSELECT_WORD_H__
