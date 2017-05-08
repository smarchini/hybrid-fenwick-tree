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


TEST(same_behavior, random_small)
{
    fenwick_random_test<7>(2-1);
    fenwick_random_test<7>(4-1);
    fenwick_random_test<7>(8-1);
    fenwick_random_test<7>(16-1);
    fenwick_random_test<7>(32-1);
    fenwick_random_test<7>(64-1);
    fenwick_random_test<7>(128-1);
    fenwick_random_test<7>(256-1);
    fenwick_random_test<7>(512-1);
    fenwick_random_test<7>(1024-1);
}

TEST(same_behavior, random_big)
{
    fenwick_random_test<7>(2*1024-1);
    fenwick_random_test<7>(4*1024-1);
    fenwick_random_test<7>(8*1024-1);
    fenwick_random_test<7>(16*1024-1);
    fenwick_random_test<7>(32*1024-1);
    fenwick_random_test<7>(64*1024-1);
    fenwick_random_test<7>(128*1024-1);
    fenwick_random_test<7>(256*1024-1);
    fenwick_random_test<7>(512*1024-1);
    fenwick_random_test<7>(1024*1024-1);
}

TEST(same_behavior, partial_tree)
{
    for (std::size_t i = 1; i < 1000; i++)
        fenwick_random_test<7>(i);
}

TEST(same_behavior, random_mixed)
{
    fenwick_random_test<7>(1024-1);
    fenwick_random_test<8>(1024-1);
    fenwick_random_test<9>(1024-1);

    fenwick_random_test<10>(1024-1);
    fenwick_random_test<11>(1024-1);
    fenwick_random_test<12>(1024-1);
    fenwick_random_test<13>(1024-1);
    fenwick_random_test<14>(1024-1);
    fenwick_random_test<15>(1024-1);
    fenwick_random_test<16>(1024-1);
    fenwick_random_test<17>(1024-1);
    fenwick_random_test<18>(1024-1);
    fenwick_random_test<19>(1024-1);

    fenwick_random_test<20>(1024-1);
    fenwick_random_test<21>(1024-1);
    fenwick_random_test<22>(1024-1);
    fenwick_random_test<23>(1024-1);
    fenwick_random_test<24>(1024-1);
    fenwick_random_test<25>(1024-1);
    fenwick_random_test<26>(1024-1);
    fenwick_random_test<27>(1024-1);
    fenwick_random_test<28>(1024-1);
    fenwick_random_test<29>(1024-1);

    fenwick_random_test<30>(1024-1);
    fenwick_random_test<31>(1024-1);
    fenwick_random_test<32>(1024-1);
    fenwick_random_test<33>(1024-1);
    fenwick_random_test<34>(1024-1);
    fenwick_random_test<35>(1024-1);
    fenwick_random_test<36>(1024-1);
    fenwick_random_test<37>(1024-1);
    fenwick_random_test<38>(1024-1);
    fenwick_random_test<39>(1024-1);

    fenwick_random_test<40>(1024-1);
    fenwick_random_test<41>(1024-1);
    fenwick_random_test<42>(1024-1);
    fenwick_random_test<43>(1024-1);
    fenwick_random_test<44>(1024-1);
    fenwick_random_test<45>(1024-1);
    fenwick_random_test<46>(1024-1);
    fenwick_random_test<47>(1024-1);
    fenwick_random_test<48>(1024-1);
    fenwick_random_test<49>(1024-1);

    fenwick_random_test<50>(1024-1);
    fenwick_random_test<51>(1024-1);
    fenwick_random_test<52>(1024-1);
    fenwick_random_test<53>(1024-1);
    fenwick_random_test<54>(1024-1);
}

#endif // __TEST_FENWICK_SAME_BEHAVIOR_H__
