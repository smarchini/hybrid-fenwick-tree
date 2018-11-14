#ifndef __TEST_RANKSELECT_HPP__
#define __TEST_RANKSELECT_HPP__

#include "utils.hpp"
#include <dynamic.hpp>


TEST(rankselect, all_ones)
{
    using namespace hft;
    constexpr size_t ELEMS = 16;
    constexpr size_t BITELEMS = ELEMS*64;
    uint64_t bitvect[ELEMS] = { UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX,
                                UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX,
                                UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX,
                                UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX };

    // naive
    ranking::Word<fenwick::FixedF>  naive(bitvect, ELEMS);
    ranking::Word<fenwick::FixedL> lnaive(bitvect, ELEMS);
    // bit
    ranking::Word<fenwick::BitF>    bit(bitvect, ELEMS);
    ranking::Word<fenwick::BitL>   lbit(bitvect, ELEMS);
    // byte
    ranking::Word<fenwick::ByteF>   byte(bitvect, ELEMS);
    ranking::Word<fenwick::ByteL>  lbyte(bitvect, ELEMS);
    // type
    ranking::Word<fenwick::TypeF>   type(bitvect, ELEMS);
    ranking::Word<fenwick::TypeL>  ltype(bitvect, ELEMS);

    // rank
    for (size_t i = 0; i <= BITELEMS; i++) {
        EXPECT_EQ(i, naive.rank(i))  << "at index: " << i;
        EXPECT_EQ(i, lnaive.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, bit.rank(i))    << "at index: " << i;
        EXPECT_EQ(i, lbit.rank(i))   << "at index: " << i;
        EXPECT_EQ(i, byte.rank(i))   << "at index: " << i;
        EXPECT_EQ(i, lbyte.rank(i))  << "at index: " << i;
        EXPECT_EQ(i, type.rank(i))   << "at index: " << i;
        EXPECT_EQ(i, ltype.rank(i))  << "at index: " << i;
    }

    // rankZero
    for (size_t i = 0; i <= BITELEMS; i++) {
        EXPECT_EQ(0, naive.rankZero(i))  << "at index: " << i;
        EXPECT_EQ(0, lnaive.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(0, bit.rankZero(i))    << "at index: " << i;
        EXPECT_EQ(0, lbit.rankZero(i))   << "at index: " << i;
        EXPECT_EQ(0, byte.rankZero(i))   << "at index: " << i;
        EXPECT_EQ(0, lbyte.rankZero(i))  << "at index: " << i;
        EXPECT_EQ(0, type.rankZero(i))   << "at index: " << i;
        EXPECT_EQ(0, ltype.rankZero(i))  << "at index: " << i;
    }

    // select
    for (size_t i = 0; i < BITELEMS; i++) {
        EXPECT_EQ(i, naive.select(i))  << "at index: " << i;
        EXPECT_EQ(i, lnaive.select(i)) << "at index: " << i;
        EXPECT_EQ(i, bit.select(i))    << "at index: " << i;
        EXPECT_EQ(i, lbit.select(i))   << "at index: " << i;
        EXPECT_EQ(i, byte.select(i))   << "at index: " << i;
        EXPECT_EQ(i, lbyte.select(i))  << "at index: " << i;
        EXPECT_EQ(i, type.select(i))   << "at index: " << i;
        EXPECT_EQ(i, ltype.select(i))  << "at index: " << i;
    }

    // selectZero
    for (size_t i = 0; i < BITELEMS; i++) {
        EXPECT_EQ(SIZE_MAX, naive.selectZero(i))  << "at index: " << i;
        EXPECT_EQ(SIZE_MAX, lnaive.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(SIZE_MAX, bit.selectZero(i))    << "at index: " << i;
        EXPECT_EQ(SIZE_MAX, lbit.selectZero(i))   << "at index: " << i;
        EXPECT_EQ(SIZE_MAX, byte.selectZero(i))   << "at index: " << i;
        EXPECT_EQ(SIZE_MAX, lbyte.selectZero(i))  << "at index: " << i;
        EXPECT_EQ(SIZE_MAX, type.selectZero(i))   << "at index: " << i;
        EXPECT_EQ(SIZE_MAX, ltype.selectZero(i))  << "at index: " << i;
    }

    // update
    for (size_t i = 0; i < ELEMS; i++) {
        naive.update(i, 1);
        lnaive.update(i, 1);
        bit.update(i, 1);
        lbit.update(i, 1);
        byte.update(i, 1);
        lbyte.update(i, 1);
        type.update(i, 1);
        ltype.update(i, 1);
    }

    // select
    for (size_t i = 0; i < ELEMS; i++) {
        EXPECT_EQ(i*64, naive.select(i))  << "at index: " << i;
        EXPECT_EQ(i*64, lnaive.select(i)) << "at index: " << i;
        EXPECT_EQ(i*64, bit.select(i))    << "at index: " << i;
        EXPECT_EQ(i*64, lbit.select(i))   << "at index: " << i;
        EXPECT_EQ(i*64, byte.select(i))   << "at index: " << i;
        EXPECT_EQ(i*64, lbyte.select(i))  << "at index: " << i;
        EXPECT_EQ(i*64, type.select(i))   << "at index: " << i;
        EXPECT_EQ(i*64, ltype.select(i))  << "at index: " << i;
    }

    // update
    for (size_t i = 0; i < ELEMS; i++) {
        naive.update(i, UINT64_MAX);
        lnaive.update(i, UINT64_MAX);
        bit.update(i, UINT64_MAX);
        lbit.update(i, UINT64_MAX);
        byte.update(i, UINT64_MAX);
        lbyte.update(i, UINT64_MAX);
        type.update(i, UINT64_MAX);
        ltype.update(i, UINT64_MAX);
    }

    // rank
    for (size_t i = 0; i <= BITELEMS; i++) {
        EXPECT_EQ(i, naive.rank(i))  << "at index: " << i;
        EXPECT_EQ(i, lnaive.rank(i)) << "at index: " << i;
        EXPECT_EQ(i, bit.rank(i))    << "at index: " << i;
        EXPECT_EQ(i, lbit.rank(i))   << "at index: " << i;
        EXPECT_EQ(i, byte.rank(i))   << "at index: " << i;
        EXPECT_EQ(i, lbyte.rank(i))  << "at index: " << i;
        EXPECT_EQ(i, type.rank(i))   << "at index: " << i;
        EXPECT_EQ(i, ltype.rank(i))  << "at index: " << i;
    }
}


TEST(rankselect, all_zeroes)
{
    using namespace hft;
    constexpr size_t ELEMS = 16;
    constexpr size_t BITELEMS = ELEMS*64;
    uint64_t bitvect[ELEMS] = { 0, 0, 0, 0,
                                0, 0, 0, 0,
                                0, 0, 0, 0,
                                0, 0, 0, 0 };

    // naive
    ranking::Word<fenwick::FixedF>  naive(bitvect, ELEMS);
    ranking::Word<fenwick::FixedL> lnaive(bitvect, ELEMS);
    // bit
    ranking::Word<fenwick::BitF>    bit(bitvect, ELEMS);
    ranking::Word<fenwick::BitL>   lbit(bitvect, ELEMS);
    // byte
    ranking::Word<fenwick::ByteF>   byte(bitvect, ELEMS);
    ranking::Word<fenwick::ByteL>  lbyte(bitvect, ELEMS);
    // type
    ranking::Word<fenwick::TypeF>   type(bitvect, ELEMS);
    ranking::Word<fenwick::TypeL>  ltype(bitvect, ELEMS);

    // rank
    for (size_t i = 0; i <= BITELEMS; i++) {
        EXPECT_EQ(0, naive.rank(i))  << "at index: " << i;
        EXPECT_EQ(0, lnaive.rank(i)) << "at index: " << i;
        EXPECT_EQ(0, bit.rank(i))    << "at index: " << i;
        EXPECT_EQ(0, lbit.rank(i))   << "at index: " << i;
        EXPECT_EQ(0, byte.rank(i))   << "at index: " << i;
        EXPECT_EQ(0, lbyte.rank(i))  << "at index: " << i;
        EXPECT_EQ(0, type.rank(i))   << "at index: " << i;
        EXPECT_EQ(0, ltype.rank(i))  << "at index: " << i;
    }

    // rankZero
    for (size_t i = 0; i <= BITELEMS; i++) {
        EXPECT_EQ(i, naive.rankZero(i))  << "at index: " << i;
        EXPECT_EQ(i, lnaive.rankZero(i)) << "at index: " << i;
        EXPECT_EQ(i, bit.rankZero(i))    << "at index: " << i;
        EXPECT_EQ(i, lbit.rankZero(i))   << "at index: " << i;
        EXPECT_EQ(i, byte.rankZero(i))   << "at index: " << i;
        EXPECT_EQ(i, lbyte.rankZero(i))  << "at index: " << i;
        EXPECT_EQ(i, type.rankZero(i))   << "at index: " << i;
        EXPECT_EQ(i, ltype.rankZero(i))  << "at index: " << i;
    }

    // select
    for (size_t i = 0; i < BITELEMS; i++) {
        EXPECT_EQ(SIZE_MAX, naive.select(i))  << "at index: " << i;
        EXPECT_EQ(SIZE_MAX, lnaive.select(i)) << "at index: " << i;
        EXPECT_EQ(SIZE_MAX, bit.select(i))    << "at index: " << i;
        EXPECT_EQ(SIZE_MAX, lbit.select(i))   << "at index: " << i;
        EXPECT_EQ(SIZE_MAX, byte.select(i))   << "at index: " << i;
        EXPECT_EQ(SIZE_MAX, lbyte.select(i))  << "at index: " << i;
        EXPECT_EQ(SIZE_MAX, type.select(i))   << "at index: " << i;
        EXPECT_EQ(SIZE_MAX, ltype.select(i))  << "at index: " << i;
    }

    // selectZero
    for (size_t i = 0; i < BITELEMS; i++) {
        EXPECT_EQ(i, naive.selectZero(i))  << "at index: " << i;
        EXPECT_EQ(i, lnaive.selectZero(i)) << "at index: " << i;
        EXPECT_EQ(i, bit.selectZero(i))    << "at index: " << i;
        EXPECT_EQ(i, lbit.selectZero(i))   << "at index: " << i;
        EXPECT_EQ(i, byte.selectZero(i))   << "at index: " << i;
        EXPECT_EQ(i, lbyte.selectZero(i))  << "at index: " << i;
        EXPECT_EQ(i, type.selectZero(i))   << "at index: " << i;
        EXPECT_EQ(i, ltype.selectZero(i))  << "at index: " << i;
    }

    // update
    for (size_t i = 0; i < ELEMS; i++) {
        naive.update(i, 0b10);
        lnaive.update(i, 0b10);
        bit.update(i, 0b10);
        lbit.update(i, 0b10);
        byte.update(i, 0b10);
        lbyte.update(i, 0b10);
        type.update(i, 0b10);
        ltype.update(i, 0b10);
    }

    // select
    for (size_t i = 0; i < ELEMS; i++) {
        EXPECT_EQ(i*64+1, naive.select(i))  << "at index: " << i;
        EXPECT_EQ(i*64+1, lnaive.select(i)) << "at index: " << i;
        EXPECT_EQ(i*64+1, bit.select(i))    << "at index: " << i;
        EXPECT_EQ(i*64+1, lbit.select(i))   << "at index: " << i;
        EXPECT_EQ(i*64+1, byte.select(i))   << "at index: " << i;
        EXPECT_EQ(i*64+1, lbyte.select(i))  << "at index: " << i;
        EXPECT_EQ(i*64+1, type.select(i))   << "at index: " << i;
        EXPECT_EQ(i*64+1, ltype.select(i))  << "at index: " << i;
    }

    // update
    for (size_t i = 0; i < ELEMS; i++) {
        naive.update(i, 0);
        lnaive.update(i, 0);
        bit.update(i, 0);
        lbit.update(i, 0);
        byte.update(i, 0);
        lbyte.update(i, 0);
        type.update(i, 0);
        ltype.update(i, 0);
    }

    // rank
    for (size_t i = 0; i <= BITELEMS; i++) {
        EXPECT_EQ(0, naive.rank(i))  << "at index: " << i;
        EXPECT_EQ(0, lnaive.rank(i)) << "at index: " << i;
        EXPECT_EQ(0, bit.rank(i))    << "at index: " << i;
        EXPECT_EQ(0, lbit.rank(i))   << "at index: " << i;
        EXPECT_EQ(0, byte.rank(i))   << "at index: " << i;
        EXPECT_EQ(0, lbyte.rank(i))  << "at index: " << i;
        EXPECT_EQ(0, type.rank(i))   << "at index: " << i;
        EXPECT_EQ(0, ltype.rank(i))  << "at index: " << i;
    }
}



template <std::size_t S>
void rankselect_random_test(std::size_t size)
{
    using namespace hft;

    static std::mt19937 mte;
    std::uniform_int_distribution<std::uint64_t> dist(0, UINT64_MAX);

    std::uint64_t *bitvect = new std::uint64_t[size];
    std::uint64_t *updates = new std::uint64_t[size];

    for (std::size_t i = 0; i < size; i++) {
        bitvect[i] = dist(mte);
        updates[i] = dist(mte);
    }


    // word
    ranking::Word<fenwick::FixedF>  naive(bitvect, size);
    ranking::Word<fenwick::FixedL> lnaive(bitvect, size);
    ranking::Word<fenwick::BitF>      bit(bitvect, size);
    ranking::Word<fenwick::BitL>     lbit(bitvect, size);
    ranking::Word<fenwick::ByteF>    byte(bitvect, size);
    ranking::Word<fenwick::ByteL>   lbyte(bitvect, size);
    ranking::Word<fenwick::TypeF>    type(bitvect, size);
    ranking::Word<fenwick::TypeL>   ltype(bitvect, size);

    // line
    ranking::Stride<fenwick::FixedF, S>  naiveS(bitvect, size);
    ranking::Stride<fenwick::FixedL, S> lnaiveS(bitvect, size);
    ranking::Stride<fenwick::BitF,   S>    bitS(bitvect, size);
    ranking::Stride<fenwick::BitL,   S>   lbitS(bitvect, size);
    ranking::Stride<fenwick::ByteF,  S>   byteS(bitvect, size);
    ranking::Stride<fenwick::ByteL,  S>  lbyteS(bitvect, size);
    ranking::Stride<fenwick::TypeF,  S>   typeS(bitvect, size);
    ranking::Stride<fenwick::TypeL,  S>  ltypeS(bitvect, size);


    // rank
    for (size_t i = 0; i <= size; i++) {
        auto res = naive.rank(i);

        EXPECT_EQ(res, lnaive.rank(i))  << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, bit.rank(i))     << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, lbit.rank(i))    << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, byte.rank(i))    << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, lbyte.rank(i))   << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, type.rank(i))    << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, ltype.rank(i))   << "at index: " << i << " template parameter " << S;

        EXPECT_EQ(res, naiveS.rank(i))  << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, lnaiveS.rank(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, bitS.rank(i))    << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, lbitS.rank(i))   << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, byteS.rank(i))   << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, lbyteS.rank(i))  << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, typeS.rank(i))   << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, ltypeS.rank(i))  << "at index: " << i << " template parameter " << S;
    }

    // rankZero
    for (size_t i = 0; i <= size; i++) {
        auto res = naive.rankZero(i);

        EXPECT_EQ(res, lnaive.rankZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, bit.rankZero(i))    << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, lbit.rankZero(i))   << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, byte.rankZero(i))   << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, lbyte.rankZero(i))  << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, type.rankZero(i))   << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, ltype.rankZero(i))  << "at index: " << i << " template parameter " << S;

        EXPECT_EQ(res, lnaiveS.rankZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, naiveS.rankZero(i))  << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, bitS.rankZero(i))    << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, lbitS.rankZero(i))   << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, byteS.rankZero(i))   << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, lbyteS.rankZero(i))  << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, typeS.rankZero(i))   << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, ltypeS.rankZero(i))  << "at index: " << i << " template parameter " << S;
    }

    // select
    for (size_t i = 0; i < size; i++) {
        auto res = naive.select(i);

        EXPECT_EQ(res, lnaive.select(i))  << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, bit.select(i))     << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, lbit.select(i))    << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, byte.select(i))    << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, lbyte.select(i))   << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, type.select(i))    << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, ltype.select(i))   << "at index: " << i << " template parameter " << S;

        EXPECT_EQ(res, naiveS.select(i))  << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, lnaiveS.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, bitS.select(i))    << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, lbitS.select(i))   << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, byteS.select(i))   << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, lbyteS.select(i))  << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, typeS.select(i))   << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, ltypeS.select(i))  << "at index: " << i << " template parameter " << S;
    }

    // selectZero
    for (size_t i = 0; i < size; i++) {
        auto res = naive.selectZero(i);

        EXPECT_EQ(res, lnaive.selectZero(i))  << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, bit.selectZero(i))     << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, lbit.selectZero(i))    << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, byte.selectZero(i))    << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, lbyte.selectZero(i))   << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, type.selectZero(i))    << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, ltype.selectZero(i))   << "at index: " << i << " template parameter " << S;

        EXPECT_EQ(res, naiveS.selectZero(i))  << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, lnaiveS.selectZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, bitS.selectZero(i))    << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, lbitS.selectZero(i))   << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, byteS.selectZero(i))   << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, lbyteS.selectZero(i))  << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, typeS.selectZero(i))   << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, ltypeS.selectZero(i))  << "at index: " << i << " template parameter " << S;
    }

    // update
    for (size_t i = 0; i < size; i++) {
        naive.update(i, updates[i]);
        lnaive.update(i, updates[i]);
        bit.update(i, updates[i]);
        lbit.update(i, updates[i]);
        byte.update(i, updates[i]);
        lbyte.update(i, updates[i]);
        type.update(i, updates[i]);
        ltype.update(i, updates[i]);

        naiveS.update(i, updates[i]);
        lnaiveS.update(i, updates[i]);
        bitS.update(i, updates[i]);
        lbitS.update(i, updates[i]);
        byteS.update(i, updates[i]);
        lbyteS.update(i, updates[i]);
        typeS.update(i, updates[i]);
        ltypeS.update(i, updates[i]);
    }

    // select
    for (size_t i = 0; i < size; i++) {
        auto res = naive.select(i);

        EXPECT_EQ(res, lnaive.select(i))  << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, bit.select(i))     << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, lbit.select(i))    << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, byte.select(i))    << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, lbyte.select(i))   << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, type.select(i))    << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, ltype.select(i))   << "at index: " << i << " template parameter " << S;

        EXPECT_EQ(res, naiveS.select(i))  << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, lnaiveS.select(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, bitS.select(i))    << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, lbitS.select(i))   << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, typeS.select(i))   << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, ltypeS.select(i))  << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, byteS.select(i))   << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, lbyteS.select(i))  << "at index: " << i << " template parameter " << S;
    }

    // selectZero
    for (size_t i = 0; i < size; i++) {
        auto res = naive.selectZero(i);

        EXPECT_EQ(res, lnaive.selectZero(i))  << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, bit.selectZero(i))     << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, lbit.selectZero(i))    << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, byte.selectZero(i))    << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, lbyte.selectZero(i))   << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, type.selectZero(i))    << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, ltype.selectZero(i))   << "at index: " << i << " template parameter " << S;

        EXPECT_EQ(res, naiveS.selectZero(i))  << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, lnaiveS.selectZero(i)) << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, bitS.selectZero(i))    << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, lbitS.selectZero(i))   << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, byteS.selectZero(i))   << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, lbyteS.selectZero(i))  << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, typeS.selectZero(i))   << "at index: " << i << " template parameter " << S;
        EXPECT_EQ(res, ltypeS.selectZero(i))  << "at index: " << i << " template parameter " << S;
    }


    delete[] updates;
    delete[] bitvect;
}

TEST(rankselect, same_behavior)
{
    // small 8
    for (std::size_t i = 1; i < 100; i++)
        rankselect_random_test<8>(i);

    // big 8
    for (std::size_t i = 128*1024; i < 128*1024+10; i++)
        rankselect_random_test<8>(i);
}


TEST(rankselect, same_behavior_wordsize)
{
    rankselect_random_test<1>(1024);
    rankselect_random_test<2>(1024);
    rankselect_random_test<4>(1024);
    rankselect_random_test<8>(1024);
    rankselect_random_test<16>(1024);
    rankselect_random_test<32>(1024);
    rankselect_random_test<64>(1024);
    rankselect_random_test<128>(1024);
    rankselect_random_test<256>(1024);
}


TEST(rankselect, like_dynamic)
{
    using namespace std;
    using namespace hft;

    static std::mt19937 mte;

    constexpr size_t SIZE = 1000;

    uint64_t *bitvect = new uint64_t[SIZE];
    uint64_t *updates = new uint64_t[SIZE];

    uniform_int_distribution<std::uint64_t> dist(0, UINT64_MAX);
    for (size_t i = 0; i < SIZE; i++) {
        bitvect[i] = dist(mte);
        updates[i] = dist(mte);
    }

    size_t ones = 0;
    for (size_t i = 0; i < SIZE; i++)
        ones += popcount(bitvect[i]);

    size_t zeroes = 64*SIZE - ones;

    ranking::Word<fenwick::FixedF> internal(bitvect, SIZE);

    dyn::suc_bv dynamic;
	for (uint64_t i = 0; i < SIZE; ++i) {
        for (uint64_t j = 0; j < 64; ++j)
            dynamic.insert(64*i + j, bitvect[i] & (1ULL << j));
    }

    for (uint64_t i = 0; i < SIZE; i++)
        EXPECT_EQ(dynamic.rank(i), internal.rank(i)) << "at index: " << i;

    for (uint64_t i = 0; i < SIZE; i++)
        EXPECT_EQ(dynamic.rank(i, 0), internal.rankZero(i)) << "at index: " << i;

    for (uint64_t i = 0; i < ones; i++)
        EXPECT_EQ(dynamic.select(i, 1), internal.select(i)) << "at index: " << i;

    for (uint64_t i = 0; i < zeroes; i++)
        EXPECT_EQ(dynamic.select(i, 0), internal.selectZero(i)) << "at index: " << i;


    delete[] updates;
    delete[] bitvect;
}


#endif // __TEST_RANKSELECT_HPP__
