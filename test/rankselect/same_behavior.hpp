#ifndef __TEST_RANKSELECT_SAME_BEHAVIOR_H__
#define __TEST_RANKSELECT_SAME_BEHAVIOR_H__

#include "../utils.hpp"

template <std::size_t S>
void rankselect_random_test(std::size_t size)
{
    using namespace dyn;

    std::uint64_t *bitvect = new std::uint64_t[size];
    fill_with_random_values(bitvect, size);

    std::uint64_t *updates = new std::uint64_t[size];
    fill_with_random_values(updates, size);

    WordRankSelect<NaiveFenwickTree> word_naive(bitvect, size);
    WordRankSelect<LTypeFenwickTree> word_ltype(bitvect, size);
    WordRankSelect<TypeFenwickTree> word_type(bitvect, size);
    WordRankSelect<LByteFenwickTree> word_lbyte(bitvect, size);
    WordRankSelect<ByteFenwickTree> word_byte(bitvect, size);
    WordRankSelect<LBitFenwickTree> word_lbit(bitvect, size);
    WordRankSelect<BitFenwickTree> word_bit(bitvect, size);

    LineRankSelect<NaiveFenwickTree, S> line_naive(bitvect, size);
    LineRankSelect<LTypeFenwickTree, S> line_ltype(bitvect, size);
    LineRankSelect<TypeFenwickTree, S> line_type(bitvect, size);
    LineRankSelect<LByteFenwickTree, S> line_lbyte(bitvect, size);
    LineRankSelect<ByteFenwickTree, S> line_byte(bitvect, size);
    LineRankSelect<LBitFenwickTree, S> line_lbit(bitvect, size);
    LineRankSelect<BitFenwickTree, S> line_bit(bitvect, size);

    for (size_t i = 0; i < size; i++) {
        auto res = word_naive.rank(i);

        EXPECT_EQ(res, word_ltype.rank(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_type.rank(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_lbyte.rank(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_byte.rank(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_lbit.rank(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_bit.rank(i)) << "at index: " << i << " template parameter " << S;

        EXPECT_EQ(res, line_naive.rank(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_ltype.rank(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_type.rank(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_lbyte.rank(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_byte.rank(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_lbit.rank(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_bit.rank(i)) << "at index: " << i << " template parameter " << S;
    }

    for (size_t i = 0; i < size; i++) {
        auto res = word_naive.rankZero(i);

        EXPECT_EQ(res, word_ltype.rankZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_type.rankZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_lbyte.rankZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_byte.rankZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_lbit.rankZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_bit.rankZero(i)) << "at index: " << i << " template parameter " << S;

        EXPECT_EQ(res, line_naive.rankZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_ltype.rankZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_type.rankZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_lbyte.rankZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_byte.rankZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_lbit.rankZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_bit.rankZero(i)) << "at index: " << i << " template parameter " << S;
    }

    for (size_t i = 0; i < size; i++) {
        auto res = word_naive.select(i);

        EXPECT_EQ(res, word_ltype.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_type.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_lbyte.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_byte.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_lbit.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_bit.select(i)) << "at index: " << i << " template parameter " << S;

        res = line_naive.select(i);
        EXPECT_EQ(res, line_naive.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_ltype.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_type.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_lbyte.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_byte.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_lbit.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_bit.select(i)) << "at index: " << i << " template parameter " << S;
    }

    
    for (size_t i = 0; i < size; i++) {
        auto res = word_naive.selectZero(i);

        EXPECT_EQ(res, word_ltype.selectZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_type.selectZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_lbyte.selectZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_byte.selectZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_lbit.selectZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_bit.selectZero(i)) << "at index: " << i << " template parameter " << S;

        EXPECT_EQ(res, line_naive.selectZero(i)) << "at index: " << i << " template parameter " << S;
        // EXPECT_EQ(res, line_ltype.selectZero(i)) << "at index: " << i << " template parameter " << S;
        // EXPECT_EQ(res, line_type.selectZero(i)) << "at index: " << i << " template parameter " << S;
        // EXPECT_EQ(res, line_lbyte.selectZero(i)) << "at index: " << i << " template parameter " << S;
        // EXPECT_EQ(res, line_byte.selectZero(i)) << "at index: " << i << " template parameter " << S;
        // EXPECT_EQ(res, line_lbit.selectZero(i)) << "at index: " << i << " template parameter " << S;
        // EXPECT_EQ(res, line_bit.selectZero(i)) << "at index: " << i << " template parameter " << S;
    }
    

    for (size_t i = 0; i < size; i++) {
        word_naive.update(i, updates[i]);
        word_ltype.update(i, updates[i]);
        word_type.update(i, updates[i]);
        word_lbyte.update(i, updates[i]);
        word_byte.update(i, updates[i]);
        word_lbit.update(i, updates[i]);
        word_bit.update(i, updates[i]);

        line_naive.update(i, updates[i]);
        line_ltype.update(i, updates[i]);
        line_type.update(i, updates[i]);
        line_lbyte.update(i, updates[i]);
        line_byte.update(i, updates[i]);
        line_lbit.update(i, updates[i]);
        line_bit.update(i, updates[i]);
    }

    for (size_t i = 0; i < size; i++) {
        auto res = word_naive.select(i);

        EXPECT_EQ(res, word_ltype.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_type.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_lbyte.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_byte.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_lbit.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_bit.select(i)) << "at index: " << i << " template parameter " << S;

        EXPECT_EQ(res, line_naive.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_ltype.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_type.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_lbyte.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_byte.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_lbit.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_bit.select(i)) << "at index: " << i << " template parameter " << S;
    }

    /*
    for (size_t i = 0; i < size; i++) {
        auto res = word_naive.selectZero(i);

        EXPECT_EQ(res, word_ltype.selectZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_type.selectZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_lbyte.selectZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_byte.selectZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_lbit.selectZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_bit.selectZero(i)) << "at index: " << i << " template parameter " << S;

        EXPECT_EQ(res, line_naive.selectZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_ltype.selectZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_type.selectZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_lbyte.selectZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_byte.selectZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_lbit.selectZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_bit.selectZero(i)) << "at index: " << i << " template parameter " << S;
    }
        */

    delete[] updates;
    delete[] bitvect;
}

TEST(rankselect_same_behavior, cache_line)
{
    for (std::size_t i = 1; i < 10000; i++)
        rankselect_random_test<3>(i);
}

TEST(rankselect_same_behavior, random_mixed)
{
    rankselect_random_test<1>(1024);
    rankselect_random_test<10>(1024);
    //rankselect_random_test<20>(1024);
    //rankselect_random_test<30>(1024);
    //rankselect_random_test<40>(1024);

    //rankselect_random_test<47>(1024);
    //rankselect_random_test<48>(1024); // SEGMENTATION FAULT
}


#endif // __TEST_RANKSELECT_SAME_BEHAVIOR_H__
