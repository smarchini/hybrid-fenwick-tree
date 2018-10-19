#ifndef __TEST_RANKSELECT_WORD_H__
#define __TEST_RANKSELECT_WORD_H__

#include "../utils.hpp"

TEST(word_rank_select, all_ones_1024)
{
    constexpr size_t MAX = 16;
    std::uint64_t bitvect[MAX] = { UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX,
                                   UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX };

    dyn::WordRankSelect<dyn::FixedF> naive(bitvect, MAX);
    dyn::WordRankSelect<dyn::FixedL> lnaive(bitvect, MAX);
    dyn::WordRankSelect<dyn::BitF> bit(bitvect, MAX);
    dyn::WordRankSelect<dyn::BitL> lbit(bitvect, MAX);
    dyn::WordRankSelect<dyn::ByteF> byte(bitvect, MAX);
    dyn::WordRankSelect<dyn::ByteL> lbyte(bitvect, MAX);
    dyn::WordRankSelect<dyn::TypeF> type(bitvect, MAX);
    dyn::WordRankSelect<dyn::TypeL> ltype(bitvect, MAX);

    for (size_t i = 0; i <= 1024; i++) {
        EXPECT_EQ(i, naive.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, lnaive.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, bit.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, lbit.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, byte.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, lbyte.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, type.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, ltype.rank(i)) << "at index: " << i;
    }

    for (size_t i = 0; i <= 1024; i++) {
        EXPECT_EQ(0, naive.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(0, lnaive.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(0, bit.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(0, lbit.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(0, byte.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(0, lbyte.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(0, type.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(0, ltype.rankZero(i)) << "at index: " << i;
    }

    for (size_t i = 0; i < 1024; i++) {
        EXPECT_EQ(i, naive.select(i)) << "at index: " << i;
        EXPECT_EQ(i, lnaive.select(i)) << "at index: " << i;
        EXPECT_EQ(i, bit.select(i)) << "at index: " << i;
        EXPECT_EQ(i, lbit.select(i)) << "at index: " << i;
        EXPECT_EQ(i, byte.select(i)) << "at index: " << i;
        EXPECT_EQ(i, lbyte.select(i)) << "at index: " << i;
        EXPECT_EQ(i, type.select(i)) << "at index: " << i;
        EXPECT_EQ(i, ltype.select(i)) << "at index: " << i;
    }

    for (size_t i = 0; i < 10; i++) {
        EXPECT_EQ(SIZE_MAX, naive.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(SIZE_MAX, lnaive.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(SIZE_MAX, bit.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(SIZE_MAX, lbit.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(SIZE_MAX, byte.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(SIZE_MAX, lbyte.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(SIZE_MAX, type.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(SIZE_MAX, ltype.selectZero(i)) << "at index: " << i;
    }

    for (size_t i = 0; i < MAX; i++) {
        naive.update(i, 1);
        lnaive.update(i, 1);
        bit.update(i, 1);
        lbit.update(i, 1);
        byte.update(i, 1);
        lbyte.update(i, 1);
        type.update(i, 1);
        ltype.update(i, 1);
    }

    for (size_t i = 0; i < MAX; i++) {
        EXPECT_EQ(i*64, naive.select(i)) << "at index: " << i;
        EXPECT_EQ(i*64, lnaive.select(i)) << "at index: " << i;
        EXPECT_EQ(i*64, bit.select(i)) << "at index: " << i;
        EXPECT_EQ(i*64, lbit.select(i)) << "at index: " << i;
        EXPECT_EQ(i*64, byte.select(i)) << "at index: " << i;
        EXPECT_EQ(i*64, lbyte.select(i)) << "at index: " << i;
        EXPECT_EQ(i*64, type.select(i)) << "at index: " << i;
        EXPECT_EQ(i*64, ltype.select(i)) << "at index: " << i;
    }

    for (size_t i = 0; i < MAX; i++) {
        naive.update(i, UINT64_MAX);
        lnaive.update(i, UINT64_MAX);
        bit.update(i, UINT64_MAX);
        lbit.update(i, UINT64_MAX);
        byte.update(i, UINT64_MAX);
        lbyte.update(i, UINT64_MAX);
        type.update(i, UINT64_MAX);
        ltype.update(i, UINT64_MAX);
    }

    for (size_t i = 0; i <= 1024; i++) {
        EXPECT_EQ(i, naive.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, lnaive.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, bit.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, lbit.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, byte.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, lbyte.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, type.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, ltype.rank(i)) << "at index: " << i;
    }

}


TEST(word_rank_select, all_zeroes_1024)
{
    constexpr size_t MAX = 16;
    std::uint64_t bitvect[MAX] = { 0, 0, 0, 0, 0, 0, 0, 0,
                                   0, 0, 0, 0, 0, 0, 0, 0 };

    dyn::WordRankSelect<dyn::FixedF> naive(bitvect, MAX);
    dyn::WordRankSelect<dyn::FixedL> lnaive(bitvect, MAX);
    dyn::WordRankSelect<dyn::BitF> bit(bitvect, MAX);
    dyn::WordRankSelect<dyn::BitL> lbit(bitvect, MAX);
    dyn::WordRankSelect<dyn::ByteF> byte(bitvect, MAX);
    dyn::WordRankSelect<dyn::ByteL> lbyte(bitvect, MAX);
    dyn::WordRankSelect<dyn::TypeF> type(bitvect, MAX);
    dyn::WordRankSelect<dyn::TypeL> ltype(bitvect, MAX);

    for (size_t i = 0; i <= 1024; i++) {
        EXPECT_EQ(0, naive.rank(i)) << "at index: " << i;
        EXPECT_EQ(0, lnaive.rank(i)) << "at index: " << i;
        EXPECT_EQ(0, bit.rank(i)) << "at index: " << i;
        EXPECT_EQ(0, lbit.rank(i)) << "at index: " << i;
        EXPECT_EQ(0, byte.rank(i)) << "at index: " << i;
        EXPECT_EQ(0, lbyte.rank(i)) << "at index: " << i;
        EXPECT_EQ(0, lbyte.rank(i)) << "at index: " << i;
        EXPECT_EQ(0, type.rank(i)) << "at index: " << i;
        EXPECT_EQ(0, ltype.rank(i)) << "at index: " << i;
    }

    for (size_t i = 0; i <= 1024; i++) {
        EXPECT_EQ(i, naive.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(i, lnaive.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(i, bit.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(i, lbit.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(i, byte.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(i, lbyte.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(i, type.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(i, ltype.rankZero(i)) << "at index: " << i;
    }

    for (size_t i = 0; i < 1024; i++) {
        EXPECT_EQ(SIZE_MAX, naive.select(i)) << "at index: " << i;
        EXPECT_EQ(SIZE_MAX, lnaive.select(i)) << "at index: " << i;
        EXPECT_EQ(SIZE_MAX, bit.select(i)) << "at index: " << i;
        EXPECT_EQ(SIZE_MAX, lbit.select(i)) << "at index: " << i;
        EXPECT_EQ(SIZE_MAX, byte.select(i)) << "at index: " << i;
        EXPECT_EQ(SIZE_MAX, lbyte.select(i)) << "at index: " << i;
        EXPECT_EQ(SIZE_MAX, type.select(i)) << "at index: " << i;
        EXPECT_EQ(SIZE_MAX, ltype.select(i)) << "at index: " << i;
    }

    for (size_t i = 0; i < 1024; i++) {
        EXPECT_EQ(i, naive.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(i, lnaive.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(i, bit.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(i, lbit.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(i, byte.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(i, lbyte.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(i, type.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(i, ltype.selectZero(i)) << "at index: " << i;
    }

     for (size_t i = 0; i < MAX; i++) {
         naive.update(i, 0b10);
         lnaive.update(i, 0b10);
         bit.update(i, 0b10);
         lbit.update(i, 0b10);
         byte.update(i, 0b10);
         lbyte.update(i, 0b10);
         type.update(i, 0b10);
         ltype.update(i, 0b10);
     }

     for (size_t i = 0; i < MAX; i++) {
         EXPECT_EQ(i*64+1, naive.select(i)) << "at index: " << i;
         EXPECT_EQ(i*64+1, lnaive.select(i)) << "at index: " << i;
         EXPECT_EQ(i*64+1, bit.select(i)) << "at index: " << i;
         EXPECT_EQ(i*64+1, lbit.select(i)) << "at index: " << i;
         EXPECT_EQ(i*64+1, byte.select(i)) << "at index: " << i;
         EXPECT_EQ(i*64+1, lbyte.select(i)) << "at index: " << i;
         EXPECT_EQ(i*64+1, type.select(i)) << "at index: " << i;
         EXPECT_EQ(i*64+1, ltype.select(i)) << "at index: " << i;
     }

     for (size_t i = 0; i < MAX; i++) {
         naive.update(i, 0);
         lnaive.update(i, 0);
         bit.update(i, 0);
         lbit.update(i, 0);
         byte.update(i, 0);
         lbyte.update(i, 0);
         type.update(i, 0);
         ltype.update(i, 0);
     }

     for (size_t i = 0; i <= 1024; i++) {
         EXPECT_EQ(0, naive.rank(i)) << "at index: " << i;
         EXPECT_EQ(0, lnaive.rank(i)) << "at index: " << i;
         EXPECT_EQ(0, bit.rank(i)) << "at index: " << i;
         EXPECT_EQ(0, lbit.rank(i)) << "at index: " << i;
         EXPECT_EQ(0, byte.rank(i)) << "at index: " << i;
         EXPECT_EQ(0, lbyte.rank(i)) << "at index: " << i;
         EXPECT_EQ(0, type.rank(i)) << "at index: " << i;
         EXPECT_EQ(0, ltype.rank(i)) << "at index: " << i;
     }

}

#endif // __TEST_RANKSELECT_WORD_H__
