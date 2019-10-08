#ifndef __TEST_FENWICK_EXPANSION_HPP__
#define __TEST_FENWICK_EXPANSION_HPP__

#include "utils.hpp"

template <std::size_t S> void run_test(std::size_t n, std::size_t k) {
  using namespace std;
  static mt19937 mte;
  uniform_int_distribution<uint64_t> dist(0, 64);

  uint64_t *increments = new uint64_t[n];
  int64_t *add_updates = new int64_t[n];

  for (size_t i = 0; i < n; i++)
    increments[i] = dist(mte);

  for (std::size_t i = 0; i < n; i++) {
    uniform_int_distribution<int64_t> sdist(0, 64 - increments[i]);
    add_updates[i] = sdist(mte);
  }

  hft::fenwick::FixedF<S> hi(increments, n);
  hft::fenwick::FixedF<S> lo(increments, n - k);

  hft::fenwick::FixedF<S> fixedf;
  hft::fenwick::ByteF<S> bytef;
  hft::fenwick::BitF<S> bitf;

  hft::fenwick::FixedL<S> fixedl;
  hft::fenwick::ByteL<S> bytel;
  hft::fenwick::BitL<S> bitl;

  for (size_t i = 0; i < n; i++) {
    fixedf.push(increments[i]);
    bytef.push(increments[i]);
    bitf.push(increments[i]);

    fixedl.push(increments[i]);
    bytel.push(increments[i]);
    bitl.push(increments[i]);

    uint64_t hi_prefix = hi.prefix(i);

    EXPECT_EQ(hi_prefix, fixedf.prefix(i))
        << "At index: " << i << "\nsize: " << n << "\ntemplate parameter: " << S;
    EXPECT_EQ(hi_prefix, bytef.prefix(i))
        << "At index: " << i << "\nsize: " << n << "\ntemplate parameter: " << S;
    EXPECT_EQ(hi_prefix, bitf.prefix(i))
        << "At index: " << i << "\nsize: " << n << "\ntemplate parameter: " << S;

    EXPECT_EQ(hi_prefix, fixedl.prefix(i))
        << "At index: " << i << "\nsize: " << n << "\ntemplate parameter: " << S;
    EXPECT_EQ(hi_prefix, bytel.prefix(i))
        << "At index: " << i << "\nsize: " << n << "\ntemplate parameter: " << S;
    EXPECT_EQ(hi_prefix, bitl.prefix(i))
        << "At index: " << i << "\nsize: " << n << "\ntemplate parameter: " << S;
  }

  for (size_t i = 0; i < k; i++) {
    fixedf.pop();
    bytef.pop();
    bitf.pop();

    fixedl.pop();
    bytel.pop();
    bitl.pop();
  }

  for (size_t i = 0; i < n - k; i++) {
    uint64_t lo_prefix = lo.prefix(i);

    EXPECT_EQ(lo_prefix, fixedf.prefix(i))
        << "After pop, at index: " << i << "\nsize: " << n << "\ntemplate parameter: " << S;
    EXPECT_EQ(lo_prefix, bytef.prefix(i))
        << "After pop, at index: " << i << "\nsize: " << n << "\ntemplate parameter: " << S;
    EXPECT_EQ(lo_prefix, bitf.prefix(i))
        << "After pop, at index: " << i << "\nsize: " << n << "\ntemplate parameter: " << S;

    EXPECT_EQ(lo_prefix, fixedl.prefix(i))
        << "After pop, at index: " << i << "\nsize: " << n << "\ntemplate parameter: " << S;
    EXPECT_EQ(lo_prefix, bytel.prefix(i))
        << "After pop, at index: " << i << "\nsize: " << n << "\ntemplate parameter: " << S;
    EXPECT_EQ(lo_prefix, bitl.prefix(i))
        << "After pop, at index: " << i << "\nsize: " << n << "\ntemplate parameter: " << S;
  }

  delete[] increments;
  delete[] add_updates;
}

TEST(expansion, partial_tree) {
  // small
  for (std::size_t i = 10; i < 1000; i++)
    run_test<64>(i, i / 2);

  // big
  for (std::size_t i = 128 * 1024; i < 128 * 1024 + 10; i++)
    run_test<64>(i, i / 2);
}

#endif // __TEST_FENWICK_EXPANSION_HPP__
