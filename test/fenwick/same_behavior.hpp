#ifndef __TEST_FENWICK_SAME_BEHAVIOR_H__
#define __TEST_FENWICK_SAME_BEHAVIOR_H__

#include "../utils.hpp"

template <std::size_t S>
void fenwick_random_test(std::size_t size)
{
    std::uint64_t *increments = new std::uint64_t[size];
    std::uint64_t *set_updates = new std::uint64_t[size];

    fill_with_random_values(increments, size);

    fill_with_random_values(set_updates, size);
    for (std::size_t i = 0; i < size; i++) {
        int inc = set_updates[i] - increments[i];
        if (inc < 0) inc = -inc;
        set_updates[i] = (increments[i] + inc) < 64 ? inc : 0;
    }

    dyn::NaiveFenwickTree<S> naive(increments, size);
    dyn::LBitFenwickTree<S> lbit(increments, size);
    dyn::LByteFenwickTree<S> lbyte(increments, size);
    dyn::LTypeFenwickTree<S> ltype(increments, size);
    dyn::BitFenwickTree<S> bit(increments, size);
    dyn::TypeFenwickTree<S> type(increments, size);
    dyn::ByteFenwickTree<S> byte(increments, size);

    // get
    for (size_t i = 0; i < size; i++) {
        std::uint64_t naive_get = naive.get(i);

        EXPECT_EQ(naive_get, lbit.get(i)) << "at index " << i << " template parameter " << S;
        EXPECT_EQ(naive_get, lbyte.get(i)) << "at index " << i << " template parameter " << S;
        EXPECT_EQ(naive_get, ltype.get(i)) << "at index " << i << " template parameter " << S;
        EXPECT_EQ(naive_get, bit.get(i)) << "at index " << i << " template parameter " << S;
        EXPECT_EQ(naive_get, type.get(i)) << "at index " << i << " template parameter " << S;
        EXPECT_EQ(naive_get, byte.get(i)) << "at index " << i << " template parameter " << S;
    }

    // find
    for (std::uint64_t i = 0; i < size; i++) {
        std::uint64_t naive_find = naive.find(i);

        EXPECT_EQ(naive_find, lbit.find(i)) << "at index " << i << " template parameter " << S;
        EXPECT_EQ(naive_find, lbyte.find(i)) << "at index " << i << " template parameter " << S;
        EXPECT_EQ(naive_find, ltype.find(i)) << "at index " << i << " template parameter " << S;
        EXPECT_EQ(naive_find, bit.find(i)) << "at index " << i << " template parameter " << S;
        EXPECT_EQ(naive_find, type.find(i)) << "at index " << i << " template parameter " << S;
        EXPECT_EQ(naive_find, byte.find(i)) << "at index " << i << " template parameter " << S;
    }

    // set
    for (size_t i = 0; i < size; i++) {
        naive.set(i, set_updates[i]);
        lbit.set(i, set_updates[i]);
        lbyte.set(i, set_updates[i]);
        ltype.set(i, set_updates[i]);
        bit.set(i, set_updates[i]);
        type.set(i, set_updates[i]);
        byte.set(i, set_updates[i]);
    }

    // get
    for (size_t i = 0; i < size; i++) {
        std::uint64_t naive_get = naive.get(i);

        EXPECT_EQ(naive_get, lbit.get(i)) << "at index " << i << " template parameter " << S;
        EXPECT_EQ(naive_get, lbyte.get(i)) << "at index " << i << " template parameter " << S;
        EXPECT_EQ(naive_get, ltype.get(i)) << "at index " << i << " template parameter " << S;
        EXPECT_EQ(naive_get, bit.get(i)) << "at index " << i << " template parameter " << S;
        EXPECT_EQ(naive_get, type.get(i)) << "at index " << i << " template parameter " << S;
        EXPECT_EQ(naive_get, byte.get(i)) << "at index " << i << " template parameter " << S;
    }

    // find complement
    for (std::uint64_t i = 0; i < size; i++) {
        std::uint64_t naive_findcomplement = naive.find_complement(i);

        EXPECT_EQ(naive_findcomplement, lbit.find_complement(i)) << "at index " << i << " template parameter " << S;
        EXPECT_EQ(naive_findcomplement, lbyte.find_complement(i)) << "at index " << i << " template parameter " << S;
        EXPECT_EQ(naive_findcomplement, ltype.find_complement(i)) << "at index " << i << " template parameter " << S;
        EXPECT_EQ(naive_findcomplement, bit.find_complement(i)) << "at index " << i << " template parameter " << S;
        EXPECT_EQ(naive_findcomplement, type.find_complement(i)) << "at index " << i << " template parameter " << S;
        EXPECT_EQ(naive_findcomplement, byte.find_complement(i)) << "at index " << i << " template parameter " << S;
    }

    delete[] increments;
    delete[] set_updates;
}


TEST(fenwick_same_behavior, perfect_tree)
{
    // small
    fenwick_random_test<64>(2-1);
    fenwick_random_test<64>(4-1);
    fenwick_random_test<64>(8-1);
    fenwick_random_test<64>(16-1);

    // big
    fenwick_random_test<64>(512*1024-1);
    fenwick_random_test<64>(1024*1024-1);
}

TEST(fenwick_same_behavior, partial_tree)
{
    // small
    for (std::size_t i = 1; i < 100; i++)
        fenwick_random_test<64>(i);

    // big
    for (std::size_t i = 128*1024; i < 128*1024+10; i++)
        fenwick_random_test<64>(i);
}

TEST(fenwick_same_behavior, different_leafvalue)
{
    fenwick_random_test<64*10>(1024-1);
    fenwick_random_test<64*20>(1024-1);
    fenwick_random_test<64*30>(1024-1);
    fenwick_random_test<64*40>(1024-1);
    fenwick_random_test<64*50>(1024-1);

    fenwick_random_test<64*54>(1024-1);
    // TODO: provare con altri valori togliendo la static_assert
}

#endif // __TEST_FENWICK_SAME_BEHAVIOR_H__
