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
    for (std::size_t i = 0; i < size; i++) {
        int inc = updates[i] - bitvect[i];
        if (inc < 0) inc = -inc;
        updates[i] = (bitvect[i] + inc) < 64 ? inc : 0;
    }

    dyn::LineRankSelect<dyn::SimpleFenwickTree, S>  line_simple(bitvect, size);
    dyn::LineRankSelect<dyn::TypedFenwickTree, S>   line_typed(bitvect, size);
    dyn::LineRankSelect<dyn::ITypedFenwickTree, S>  line_ityped(bitvect, size);
    dyn::LineRankSelect<dyn::ByteFenwickTree, S>    line_byte(bitvect, size);
    dyn::LineRankSelect<dyn::CompactFenwickTree, S> line_compact(bitvect, size);
    dyn::LineRankSelect<dyn::ShrankFenwickTree, S>  line_shrank(bitvect, size);

    dyn::WordRankSelect<dyn::SimpleFenwickTree>  word_simple(bitvect, size);
    dyn::WordRankSelect<dyn::TypedFenwickTree>   word_typed(bitvect, size);
    dyn::WordRankSelect<dyn::ITypedFenwickTree>  word_ityped(bitvect, size);
    dyn::WordRankSelect<dyn::ByteFenwickTree>    word_byte(bitvect, size);
    dyn::WordRankSelect<dyn::CompactFenwickTree> word_compact(bitvect, size);
    dyn::WordRankSelect<dyn::ShrankFenwickTree>  word_shrank(bitvect, size);

    for (size_t i = 0; i <= size; i++) {
        auto res = word_simple.rank(i);

        EXPECT_EQ(res, word_typed.rank(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_ityped.rank(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_byte.rank(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_compact.rank(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_shrank.rank(i)) << "at index: " << i << " template parameter " << S;

        EXPECT_EQ(res, line_simple.rank(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_typed.rank(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_ityped.rank(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_byte.rank(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_compact.rank(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_shrank.rank(i)) << "at index: " << i << " template parameter " << S;
    }

    for (size_t i = 0; i <= size; i++) {
        auto res = word_simple.rankZero(i);

        EXPECT_EQ(res, word_typed.rankZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_ityped.rankZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_byte.rankZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_compact.rankZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_shrank.rankZero(i)) << "at index: " << i << " template parameter " << S;

        EXPECT_EQ(res, line_simple.rankZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_typed.rankZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_ityped.rankZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_byte.rankZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_compact.rankZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_shrank.rankZero(i)) << "at index: " << i << " template parameter " << S;
    }

    // TODO: sistemare il comportamento della select se non ci sono uni
    for (size_t i = 0; i < size; i++) {
        auto res = word_simple.select(i);

        EXPECT_EQ(res, word_typed.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_ityped.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_byte.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_compact.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_shrank.select(i)) << "at index: " << i << " template parameter " << S;

        res = line_simple.select(i);
        EXPECT_EQ(res, line_simple.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_typed.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_ityped.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_byte.select(i)) << "at index: " << i << " template parameter " << S << " size: " << size;
        EXPECT_EQ(res, line_compact.select(i)) << "at index: " << i << " template parameter " << S << "size " << size;
        EXPECT_EQ(res, line_shrank.select(i)) << "at index: " << i << " template parameter " << S;
    }

    //TODO: sistemare il comportamento della selectZero se non ci sono zeri
    for (size_t i = 0; i < 10; i++) {
        auto res = word_simple.selectZero(i);

        EXPECT_EQ(res, word_typed.selectZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_ityped.selectZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_byte.selectZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_compact.selectZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_shrank.selectZero(i)) << "at index: " << i << " template parameter " << S;

        EXPECT_EQ(res, line_simple.selectZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_typed.selectZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_ityped.selectZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_byte.selectZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_compact.selectZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_shrank.selectZero(i)) << "at index: " << i << " template parameter " << S;
    }

    for (size_t i = 0; i < size; i++) {
        word_simple.update(i, updates[i]);
        word_typed.update(i, updates[i]);
        word_ityped.update(i, updates[i]);
        word_byte.update(i, updates[i]);
        word_compact.update(i, updates[i]);
        word_shrank.update(i, updates[i]);
        line_simple.update(i, updates[i]);
        line_typed.update(i, updates[i]);
        line_ityped.update(i, updates[i]);
        line_byte.update(i, updates[i]);
        line_compact.update(i, updates[i]);
        line_shrank.update(i, updates[i]);
    }


    // TODO: sistemare il comportamento della select se non ci sono uni
    for (size_t i = 0; i < size; i++) {
        auto res = word_simple.select(i);

        EXPECT_EQ(res, word_typed.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_ityped.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_byte.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_compact.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_shrank.select(i)) << "at index: " << i << " template parameter " << S;

        EXPECT_EQ(res, line_simple.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_typed.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_ityped.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_byte.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_compact.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_shrank.select(i)) << "at index: " << i << " template parameter " << S;
    }

    //TODO: sistemare il comportamento della selectZero se non ci sono zeri
    for (size_t i = 0; i < 10; i++) {
        auto res = word_simple.selectZero(i);

        EXPECT_EQ(res, word_typed.selectZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_ityped.selectZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_byte.selectZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_compact.selectZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, word_shrank.selectZero(i)) << "at index: " << i << " template parameter " << S;

        EXPECT_EQ(res, line_simple.selectZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_typed.selectZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_ityped.selectZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_byte.selectZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_compact.selectZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, line_shrank.selectZero(i)) << "at index: " << i << " template parameter " << S;
    }
 
    delete[] bitvect;
}


TEST(rankselect_same_behavior, cache_line)
{
    for (std::size_t i = 1; i < 10000; i++)
        rankselect_random_test<8>(i);
}

TEST(rankselect_same_behavior, random_mixed)
{
    // TODO caso <0> dovrà dare errore (stessa cosa per gli alberi)
    // TODO imporre anche un limite superiore (stessa cosa per gli alberi)
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
