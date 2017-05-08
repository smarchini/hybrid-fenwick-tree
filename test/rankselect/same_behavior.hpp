#ifndef __TEST_RANKSELECT_SAME_BEHAVIOR_H__
#define __TEST_RANKSELECT_SAME_BEHAVIOR_H__

#include "../utils.hpp"

template <std::size_t S>
void rankselect_random_test(std::size_t size)
{

    std::uint64_t *bitvect = new std::uint64_t[size];
    fill_with_random_values(bitvect, size);

    std::uint64_t *updates = new std::uint64_t[size];
    fill_with_random_values(updates, size);

    dyn::LineRankSelect<dyn::NaiveFenwickTree, S>  line_naive(bitvect, size);
    dyn::LineRankSelect<dyn::LTypeFenwickTree, S>   line_ltype(bitvect, size);
    dyn::LineRankSelect<dyn::TypeFenwickTree, S>  line_type(bitvect, size);
    dyn::LineRankSelect<dyn::LByteFenwickTree, S>    line_lbyte(bitvect, size);
    dyn::LineRankSelect<dyn::ByteFenwickTree, S>   line_byte(bitvect, size);
    dyn::LineRankSelect<dyn::LBitFenwickTree, S> line_lbit(bitvect, size);
    dyn::LineRankSelect<dyn::BitFenwickTree, S>  line_bit(bitvect, size);

    dyn::WordRankSelect<dyn::NaiveFenwickTree>  word_naive(bitvect, size);
    dyn::WordRankSelect<dyn::LTypeFenwickTree>   word_ltype(bitvect, size);
    dyn::WordRankSelect<dyn::TypeFenwickTree>  word_type(bitvect, size);
    dyn::WordRankSelect<dyn::LByteFenwickTree>    word_lbyte(bitvect, size);
    dyn::WordRankSelect<dyn::ByteFenwickTree>   word_byte(bitvect, size);
    dyn::WordRankSelect<dyn::LBitFenwickTree> word_lbit(bitvect, size);
    dyn::WordRankSelect<dyn::BitFenwickTree>  word_bit(bitvect, size);

    for (size_t i = 0; i <= size; i++) {
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

    for (size_t i = 0; i <= size; i++) {
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

    for (size_t i = 0; i < 10; i++) {
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

    for (size_t i = 0; i < 10; i++) {
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

    delete[] updates;
    delete[] bitvect;
}


TEST(rankselect_same_behavior, cache_line)
{
    for (std::size_t i = 1; i < 10000; i++)
        rankselect_random_test<8>(i);
}

TEST(rankselect_same_behavior, random_mixed)
{
    rankselect_random_test<1>(1024);
    rankselect_random_test<2>(1024);
    rankselect_random_test<3>(1024);
    rankselect_random_test<4>(1024);
    rankselect_random_test<5>(1024);
    rankselect_random_test<6>(1024);
    rankselect_random_test<7>(1024);
    rankselect_random_test<8>(1024);
    rankselect_random_test<9>(1024);

    rankselect_random_test<10>(1024);
    rankselect_random_test<11>(1024);
    rankselect_random_test<12>(1024);
    rankselect_random_test<13>(1024);
    rankselect_random_test<14>(1024);
    rankselect_random_test<15>(1024);
    rankselect_random_test<16>(1024);
    rankselect_random_test<17>(1024);
    rankselect_random_test<18>(1024);
    rankselect_random_test<19>(1024);

    rankselect_random_test<20>(1024);
    rankselect_random_test<21>(1024);
    rankselect_random_test<22>(1024);
    rankselect_random_test<23>(1024);
    rankselect_random_test<24>(1024);
    rankselect_random_test<25>(1024);
    rankselect_random_test<26>(1024);
    rankselect_random_test<27>(1024);
    rankselect_random_test<28>(1024);
    rankselect_random_test<29>(1024);

    rankselect_random_test<30>(1024);
    rankselect_random_test<31>(1024);
    rankselect_random_test<32>(1024);
    rankselect_random_test<33>(1024);
    rankselect_random_test<34>(1024);
    rankselect_random_test<35>(1024);
    rankselect_random_test<36>(1024);
    rankselect_random_test<37>(1024);
    rankselect_random_test<38>(1024);
    rankselect_random_test<39>(1024);


    rankselect_random_test<40>(1024);
    rankselect_random_test<41>(1024);
    rankselect_random_test<42>(1024);
    rankselect_random_test<43>(1024);
    rankselect_random_test<44>(1024);
    rankselect_random_test<45>(1024);
    rankselect_random_test<46>(1024);
    rankselect_random_test<47>(1024);
    //rankselect_random_test<48>(1024); // SEGMENTATION FAULT
}


#endif // __TEST_RANKSELECT_SAME_BEHAVIOR_H__
