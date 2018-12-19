#ifndef __TEST_FENWICK_SAME_BEHAVIOR_HPP__
#define __TEST_FENWICK_SAME_BEHAVIOR_HPP__

#include "utils.hpp"

template <std::size_t S>
void fenwick_random_test(std::size_t size)
{
    static std::mt19937 mte;
    std::uniform_int_distribution<std::uint64_t> dist(0, 64);

    std::uint64_t *increments = new std::uint64_t[size];
    std::int64_t *add_updates = new std::int64_t[size];

    for (std::size_t i = 0; i < size; i++)
        increments[i] = dist(mte);

    for (std::size_t i = 0; i < size; i++) {
        std::uniform_int_distribution<std::int64_t> sdist(0, 64 - increments[i]);
        add_updates[i] = sdist(mte);
    }

    hft::fenwick::FixedF<S> naive(increments, size);
    hft::fenwick::FixedL<S> lnaive(increments, size);
    hft::fenwick::BitF<S> bit(increments, size);
    hft::fenwick::BitL<S> lbit(increments, size);
    hft::fenwick::ByteF<S> byte(increments, size);
    hft::fenwick::ByteL<S> lbyte(increments, size);
    hft::fenwick::TypeF<S> type(increments, size);
    hft::fenwick::TypeL<S> ltype(increments, size);

    // prefix
    for (size_t i = 0; i <= size; i++) {
        std::uint64_t naive_prefix = naive.prefix(i);

        EXPECT_EQ(naive_prefix, lnaive.prefix(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate parameter: " << S;
        EXPECT_EQ(naive_prefix, bit.prefix(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate parameter: " << S;
        EXPECT_EQ(naive_prefix, lbit.prefix(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate parameter: " << S;
        EXPECT_EQ(naive_prefix, byte.prefix(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate parameter: " << S;
        EXPECT_EQ(naive_prefix, lbyte.prefix(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate parameter: " << S;
        EXPECT_EQ(naive_prefix, type.prefix(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate parameter: " << S;
        EXPECT_EQ(naive_prefix, ltype.prefix(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate parameter: " << S;
   }

    // find
    for (std::uint64_t i = 0; i <= size; i++) {
        std::uint64_t naive_find = naive.find(i);

        EXPECT_EQ(naive_find, lnaive.find(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate parameter: " << S;
        EXPECT_EQ(naive_find, bit.find(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate parameter: " << S;
        EXPECT_EQ(naive_find, lbit.find(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate parameter: " << S;
        EXPECT_EQ(naive_find, byte.find(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate parameter: " << S;
        EXPECT_EQ(naive_find, lbyte.find(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate parameter: " << S;
        EXPECT_EQ(naive_find, type.find(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate parameter: " << S;
        EXPECT_EQ(naive_find, ltype.find(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate parameter: " << S;
    }

    // add
    for (size_t i = 1; i < size; i++) {
        naive.add(i+1,  add_updates[i]);
        lnaive.add(i+1, add_updates[i]);
        bit.add(i+1,    add_updates[i]);
        lbit.add(i+1,   add_updates[i]);
        byte.add(i+1,   add_updates[i]);
        lbyte.add(i+1,  add_updates[i]);
        type.add(i+1,   add_updates[i]);
        ltype.add(i+1,  add_updates[i]);
    }

    // prefix
    for (size_t i = 0; i <= size; i++) {
        std::uint64_t naive_prefix = naive.prefix(i);

        EXPECT_EQ(naive_prefix, lnaive.prefix(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate parameter: " << S;
        EXPECT_EQ(naive_prefix, bit.prefix(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate parameter: " << S;
        EXPECT_EQ(naive_prefix, lbit.prefix(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate parameter: " << S;
        EXPECT_EQ(naive_prefix, byte.prefix(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate parameter: " << S;
        EXPECT_EQ(naive_prefix, lbyte.prefix(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate parameter: " << S;
        EXPECT_EQ(naive_prefix, type.prefix(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate parameter: " << S;
        EXPECT_EQ(naive_prefix, ltype.prefix(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate parameter: " << S;
    }

    // find complement
    for (std::uint64_t i = 0; i <= size; i++) {
        std::uint64_t naive_findcomplement = naive.compFind(i);

        EXPECT_EQ(naive_findcomplement, lnaive.compFind(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate parameter: " << S;
        EXPECT_EQ(naive_findcomplement, bit.compFind(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate parameter: " << S;
        EXPECT_EQ(naive_findcomplement, lbit.compFind(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate parameter: " << S;
        EXPECT_EQ(naive_findcomplement, byte.compFind(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate parameter: " << S;
        EXPECT_EQ(naive_findcomplement, lbyte.compFind(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate parameter: " << S;
        EXPECT_EQ(naive_findcomplement, type.compFind(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate parameter: " << S;
        EXPECT_EQ(naive_findcomplement, ltype.compFind(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate parameter: " << S;
    }

    delete[] increments;
    delete[] add_updates;
}


TEST(fenwicktree, perfect_tree)
{
    // small
    fenwick_random_test<64>(2-1);
    fenwick_random_test<64>(4-1);
    fenwick_random_test<64>(8-1);
    fenwick_random_test<64>(16-1);
    fenwick_random_test<64>(32-1);
    fenwick_random_test<64>(64-1);
    fenwick_random_test<64>(128-1);

    // big
    fenwick_random_test<64>(1024-1);
    fenwick_random_test<64>(512*1024-1);
    fenwick_random_test<64>(1024*1024-1);
}

TEST(fenwicktree, partial_tree)
{
    // small
    for (std::size_t i = 1; i < 1000; i++)
        fenwick_random_test<64>(i);

    // big
    for (std::size_t i = 128*1024; i < 128*1024+10; i++)
        fenwick_random_test<64>(i);
}

TEST(fenwicktree, leafmaxval)
{
    fenwick_random_test<64*10>(1024-1);
    fenwick_random_test<64*20>(1024-1);
    fenwick_random_test<64*30>(1024-1);
    fenwick_random_test<64*40>(1024-1);
    fenwick_random_test<64*50>(1024-1);
    fenwick_random_test<64*60>(1024-1);

    fenwick_random_test<64*64>(1024-1);
    fenwick_random_test<64*128>(1024-1);
    fenwick_random_test<64*256>(1024-1);
    fenwick_random_test<64*512>(1024-1);
    fenwick_random_test<64*1024>(1024-1);

    fenwick_random_test<256>(1024-1); // 2^8
    fenwick_random_test<65536>(1024-1); // 2^16
    fenwick_random_test<16777216>(1024-1); // 2^24
    fenwick_random_test<4294967296>(1024-1); // 2^32
    fenwick_random_test<1099511627776>(1024-1); // 2^40
    fenwick_random_test<281474976710656>(1024-1); // 2^48
}

#endif // __TEST_FENWICK_SAME_BEHAVIOR_HPP__
