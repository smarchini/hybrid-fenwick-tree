#ifndef __TEST_FENWICK_SAME_BEHAVIOR_H__
#define __TEST_FENWICK_SAME_BEHAVIOR_H__

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

    dyn::FixedF<S> naive(increments, size);
    dyn::FixedF<S> lnaive(increments, size);
    dyn::BitF<S> bit(increments, size);
    dyn::BitL<S> lbit(increments, size);
    dyn::ByteL<S> lbyte(increments, size);
    dyn::ByteF<S> byte(increments, size);
    dyn::TypeF<S> type(increments, size);
    dyn::TypeL<S> ltype(increments, size);

    // prefix
    for (size_t i = 0; i < size; i++) {
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
    for (std::uint64_t i = 0; i < size; i++) {
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
    for (size_t i = 0; i < size; i++) {
        naive.add(i, add_updates[i]);
        lnaive.add(i, add_updates[i]);
        bit.add(i,   add_updates[i]);
        lbit.add(i,  add_updates[i]);
        byte.add(i,  add_updates[i]);
        lbyte.add(i, add_updates[i]);
        type.add(i,  add_updates[i]);
        ltype.add(i, add_updates[i]);
    }

    // prefix
    for (size_t i = 0; i < size; i++) {
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
    for (std::uint64_t i = 0; i < size; i++) {
        std::uint64_t naive_findcomplement = naive.find_complement(i);

        EXPECT_EQ(naive_findcomplement, lnaive.find_complement(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate parameter: " << S;
        EXPECT_EQ(naive_findcomplement, bit.find_complement(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate parameter: " << S;
        EXPECT_EQ(naive_findcomplement, lbit.find_complement(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate parameter: " << S;
        EXPECT_EQ(naive_findcomplement, byte.find_complement(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate parameter: " << S;
        EXPECT_EQ(naive_findcomplement, lbyte.find_complement(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate parameter: " << S;
        EXPECT_EQ(naive_findcomplement, type.find_complement(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate parameter: " << S;
        EXPECT_EQ(naive_findcomplement, ltype.find_complement(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate parameter: " << S;
    }

    delete[] increments;
    delete[] add_updates;
}


TEST(fenwicktree, perfect_tree)
{
    for (size_t i = 0; i < 10000; i++)
        fenwick_random_test<64>(10);

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
}

#endif // __TEST_FENWICK_SAME_BEHAVIOR_H__
