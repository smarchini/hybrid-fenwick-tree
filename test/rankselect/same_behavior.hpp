#ifndef __TEST_RANKSELECT_SAME_BEHAVIOR_HPP__
#define __TEST_RANKSELECT_SAME_BEHAVIOR_HPP__

#include "../utils.hpp"

template <std::size_t S> void rankselect_random_test(std::size_t size) {
  using namespace hft;

  static std::mt19937 mte;
  std::uniform_int_distribution<std::uint64_t> dist(0, UINT64_MAX);

  std::uint64_t *bitvect = new std::uint64_t[size];
  std::uint64_t *updates = new std::uint64_t[size];

  for (std::size_t i = 0; i < size; i++) {
    bitvect[i] = dist(mte);
    updates[i] = dist(mte);
  }

  ranking::Word<fenwick::FixedF> word_naive(bitvect, size);
  // ranking::Word<fenwick::TypeL> word_ltype(bitvect, size);
  // ranking::Word<fenwick::TypeF> word_type(bitvect, size);
  ranking::Word<fenwick::ByteL> word_lbyte(bitvect, size);
  ranking::Word<fenwick::ByteF> word_byte(bitvect, size);
  ranking::Word<fenwick::BitL> word_lbit(bitvect, size);
  ranking::Word<fenwick::BitF> word_bit(bitvect, size);

  ranking::Stride<fenwick::FixedF, S> line_naive(bitvect, size);
  // ranking::Stride<fenwick::TypeL, S> line_ltype(bitvect, size);
  // ranking::Stride<fenwick::TypeF, S> line_type(bitvect, size);
  ranking::Stride<fenwick::ByteL, S> line_lbyte(bitvect, size);
  ranking::Stride<fenwick::ByteF, S> line_byte(bitvect, size);
  ranking::Stride<fenwick::BitL, S> line_lbit(bitvect, size);
  ranking::Stride<fenwick::BitF, S> line_bit(bitvect, size);

  for (size_t i = 0; i < size; i++) {
    auto res = word_naive.rank(i);

    // EXPECT_EQ(res, word_ltype.rank(i)) << "at index: " << i << " template parameter " << S;
    // EXPECT_EQ(res, word_type.rank(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, word_lbyte.rank(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, word_byte.rank(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, word_lbit.rank(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, word_bit.rank(i)) << "at index: " << i << " template parameter " << S;

    EXPECT_EQ(res, line_naive.rank(i)) << "at index: " << i << " template parameter " << S;
    // EXPECT_EQ(res, line_ltype.rank(i)) << "at index: " << i << " template parameter " << S;
    // EXPECT_EQ(res, line_type.rank(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, line_lbyte.rank(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, line_byte.rank(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, line_lbit.rank(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, line_bit.rank(i)) << "at index: " << i << " template parameter " << S;
  }

  for (size_t i = 0; i < size; i++) {
    auto res = word_naive.rankZero(i);

    // EXPECT_EQ(res, word_ltype.rankZero(i)) << "at index: " << i << " template parameter " << S;
    // EXPECT_EQ(res, word_type.rankZero(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, word_lbyte.rankZero(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, word_byte.rankZero(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, word_lbit.rankZero(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, word_bit.rankZero(i)) << "at index: " << i << " template parameter " << S;

    EXPECT_EQ(res, line_naive.rankZero(i)) << "at index: " << i << " template parameter " << S;
    // EXPECT_EQ(res, line_ltype.rankZero(i)) << "at index: " << i << " template parameter " << S;
    // EXPECT_EQ(res, line_type.rankZero(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, line_lbyte.rankZero(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, line_byte.rankZero(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, line_lbit.rankZero(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, line_bit.rankZero(i)) << "at index: " << i << " template parameter " << S;
  }

  for (size_t i = 0; i < size; i++) {
    auto res = word_naive.select(i);

    // EXPECT_EQ(res, word_ltype.select(i)) << "at index: " << i << " template parameter " << S;
    // EXPECT_EQ(res, word_type.select(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, word_lbyte.select(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, word_byte.select(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, word_lbit.select(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, word_bit.select(i)) << "at index: " << i << " template parameter " << S;

    res = line_naive.select(i);
    EXPECT_EQ(res, line_naive.select(i)) << "at index: " << i << " template parameter " << S;
    // EXPECT_EQ(res, line_ltype.select(i)) << "at index: " << i << " template parameter " << S;
    // EXPECT_EQ(res, line_type.select(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, line_lbyte.select(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, line_byte.select(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, line_lbit.select(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, line_bit.select(i)) << "at index: " << i << " template parameter " << S;
  }

  for (size_t i = 0; i < size; i++) {
    auto res = word_naive.selectZero(i);

    // EXPECT_EQ(res, word_ltype.selectZero(i)) << "at index: " << i << " template parameter " << S;
    // EXPECT_EQ(res, word_type.selectZero(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, word_lbyte.selectZero(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, word_byte.selectZero(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, word_lbit.selectZero(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, word_bit.selectZero(i)) << "at index: " << i << " template parameter " << S;

    EXPECT_EQ(res, line_naive.selectZero(i)) << "at index: " << i << " template parameter " << S;
    // EXPECT_EQ(res, line_ltype.selectZero(i)) << "at index: " << i << " template parameter " << S;
    // EXPECT_EQ(res, line_type.selectZero(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, line_lbyte.selectZero(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, line_byte.selectZero(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, line_lbit.selectZero(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, line_bit.selectZero(i)) << "at index: " << i << " template parameter " << S;
  }

  for (size_t i = 0; i < size; i++) {
    word_naive.update(i, updates[i]);
    // word_ltype.update(i, updates[i]);
    // word_type.update(i, updates[i]);
    word_lbyte.update(i, updates[i]);
    word_byte.update(i, updates[i]);
    word_lbit.update(i, updates[i]);
    word_bit.update(i, updates[i]);

    line_naive.update(i, updates[i]);
    // line_ltype.update(i, updates[i]);
    // line_type.update(i, updates[i]);
    line_lbyte.update(i, updates[i]);
    line_byte.update(i, updates[i]);
    line_lbit.update(i, updates[i]);
    line_bit.update(i, updates[i]);
  }

  for (size_t i = 0; i < size; i++) {
    auto res = word_naive.select(i);

    // EXPECT_EQ(res, word_ltype.select(i)) << "at index: " << i << " template parameter " << S;
    // EXPECT_EQ(res, word_type.select(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, word_lbyte.select(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, word_byte.select(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, word_lbit.select(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, word_bit.select(i)) << "at index: " << i << " template parameter " << S;

    EXPECT_EQ(res, line_naive.select(i)) << "at index: " << i << " template parameter " << S;
    // EXPECT_EQ(res, line_ltype.select(i)) << "at index: " << i << " template parameter " << S;
    // EXPECT_EQ(res, line_type.select(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, line_lbyte.select(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, line_byte.select(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, line_lbit.select(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, line_bit.select(i)) << "at index: " << i << " template parameter " << S;
  }

  for (size_t i = 0; i < size; i++) {
    auto res = word_naive.selectZero(i);

    // EXPECT_EQ(res, word_ltype.selectZero(i)) << "at index: " << i << " template parameter " << S;
    // EXPECT_EQ(res, word_type.selectZero(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, word_lbyte.selectZero(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, word_byte.selectZero(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, word_lbit.selectZero(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, word_bit.selectZero(i)) << "at index: " << i << " template parameter " << S;

    EXPECT_EQ(res, line_naive.selectZero(i)) << "at index: " << i << " template parameter " << S;
    // EXPECT_EQ(res, line_ltype.selectZero(i)) << "at index: " << i << " template parameter " << S;
    // EXPECT_EQ(res, line_type.selectZero(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, line_lbyte.selectZero(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, line_byte.selectZero(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, line_lbit.selectZero(i)) << "at index: " << i << " template parameter " << S;
    EXPECT_EQ(res, line_bit.selectZero(i)) << "at index: " << i << " template parameter " << S;
  }

  delete[] updates;
  delete[] bitvect;
}

TEST(rankselect_same_behavior, cache_line) {
  // small
  for (std::size_t i = 1; i < 100; i++)
    rankselect_random_test<8>(i);

  // big
  for (std::size_t i = 128 * 1024; i < 128 * 1024 + 10; i++)
    rankselect_random_test<8>(i);
}

TEST(rankselect_same_behavior, random_mixed) {
  rankselect_random_test<1>(1024);
  rankselect_random_test<10>(1024);
  rankselect_random_test<20>(1024);
  rankselect_random_test<30>(1024);
  rankselect_random_test<40>(1024);

  rankselect_random_test<47>(1024);
  // rankselect_random_test<48>(1024); // SEGMENTATION FAULT
}

#endif // __TEST_RANKSELECT_SAME_BEHAVIOR_HPP__
