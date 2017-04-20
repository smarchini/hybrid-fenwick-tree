#ifndef __SAME_BEHAVIOR_TEST_H__
#define __SAME_BEHAVIOR_TEST_H__

#include "test_utils.hpp"

template <std::size_t S>
void random_test(std::size_t size)
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

    dyn::SimpleFenwickTree<S> simple(increments, size);
    dyn::CompactFenwickTree<S> compact(increments, size);
    dyn::ByteFenwickTree<S> byte(increments, size);
    dyn::TypedFenwickTree<S> typed(increments, size);
    dyn::ShrankFenwickTree<S> shrank(increments, size);
    dyn::ITypedFenwickTree<S> ityped(increments, size);

    // get
    for (size_t i = 0; i < size; i++) {
        std::uint64_t simple_get = simple.get(i);

        EXPECT_EQ(simple_get, compact.get(i)) << "at index " << i << " template parameter " << S;
        EXPECT_EQ(simple_get, byte.get(i)) << "at index " << i << " template parameter " << S;
        EXPECT_EQ(simple_get, typed.get(i)) << "at index " << i << " template parameter " << S;
        EXPECT_EQ(simple_get, shrank.get(i)) << "at index " << i << " template parameter " << S;
        EXPECT_EQ(simple_get, ityped.get(i)) << "at index " << i << " template parameter " << S;
    }

    // find
    for (std::uint64_t i = 0; i < size; i++) {
        std::uint64_t simple_find = simple.find(i);

        EXPECT_EQ(simple_find, compact.find(i)) << "at index " << i << " template parameter " << S;
        EXPECT_EQ(simple_find, byte.find(i)) << "at index " << i << " template parameter " << S;
        EXPECT_EQ(simple_find, typed.find(i)) << "at index " << i << " template parameter " << S;
        EXPECT_EQ(simple_find, shrank.find(i)) << "at index " << i << " template parameter " << S;
        EXPECT_EQ(simple_find, ityped.find(i)) << "at index " << i << " template parameter " << S;
    }

    // set
    for (size_t i = 0; i < size; i++) {
        simple.set(i, set_updates[i]);
        compact.set(i, set_updates[i]);
        byte.set(i, set_updates[i]);
        typed.set(i, set_updates[i]);
        shrank.set(i, set_updates[i]);
        ityped.set(i, set_updates[i]);
    }

    // get
    for (size_t i = 0; i < size; i++) {
        std::uint64_t simple_get = simple.get(i);

        EXPECT_EQ(simple_get, compact.get(i)) << "at index " << i << " template parameter " << S;
        EXPECT_EQ(simple_get, byte.get(i)) << "at index " << i << " template parameter " << S;
        EXPECT_EQ(simple_get, typed.get(i)) << "at index " << i << " template parameter " << S;
        EXPECT_EQ(simple_get, shrank.get(i)) << "at index " << i << " template parameter " << S;
        EXPECT_EQ(simple_get, ityped.get(i)) << "at index " << i << " template parameter " << S;
    }

    // find complement
    for (std::uint64_t i = 0; i < size; i++) {
        std::uint64_t simple_findcomplement = simple.find(i, true);

        EXPECT_EQ(simple_findcomplement, compact.find(i, true)) << "at index " << i << " template parameter " << S;
        EXPECT_EQ(simple_findcomplement, byte.find(i, true)) << "at index " << i << " template parameter " << S;
        EXPECT_EQ(simple_findcomplement, typed.find(i, true)) << "at index " << i << " template parameter " << S;
        EXPECT_EQ(simple_findcomplement, shrank.find(i, true)) << "at index " << i << " template parameter " << S;
        EXPECT_EQ(simple_findcomplement, ityped.find(i, true)) << "at index " << i << " template parameter " << S;
    }

    delete[] increments;
    delete[] set_updates;
}


TEST(same_behavior, random_small)
{
    random_test<7>(2-1);
    random_test<7>(4-1);
    random_test<7>(8-1);
    random_test<7>(16-1);
    random_test<7>(32-1);
    random_test<7>(64-1);
    random_test<7>(128-1);
    random_test<7>(256-1);
    random_test<7>(512-1);
    random_test<7>(1024-1);
}

TEST(same_behavior, random_big)
{
    random_test<7>(2*1024-1);
    random_test<7>(4*1024-1);
    random_test<7>(8*1024-1);
    random_test<7>(16*1024-1);
    random_test<7>(32*1024-1);
    random_test<7>(64*1024-1);
    random_test<7>(128*1024-1);
    random_test<7>(256*1024-1);
    random_test<7>(512*1024-1);
    random_test<7>(1024*1024-1);
}

TEST(same_behavior, partial_tree)
{
    //for (std::size_t i = 1; i < 1000; i++)
    //    random_test<7>(i);
}

TEST(same_behavior, random_mixed)
{
    // random_test<8>(1024-1);
    // random_test<10>(16-1);

    // random_test<10>(1024-1);
    // random_test<11>(1024-1);
    // random_test<12>(1024-1);
    // random_test<13>(1024-1);
    // random_test<14>(1024-1);
    // random_test<15>(1024-1);
    // random_test<16>(1024-1);
    // random_test<17>(1024-1);
    // random_test<18>(1024-1);
    // random_test<19>(1024-1);

    // random_test<20>(1024-1);
    // random_test<31>(1024-1);
    // random_test<32>(1024-1);
    // random_test<33>(1024-1);
    // random_test<34>(1024-1);
    // random_test<35>(1024-1);
    // random_test<36>(1024-1);
    // random_test<37>(1024-1);
    // random_test<38>(1024-1);
    // random_test<39>(1024-1);

    // random_test<40>(1024-1);
    // random_test<41>(1024-1);
    // random_test<42>(1024-1);
    // random_test<43>(1024-1);
    // random_test<44>(1024-1);
    // random_test<45>(1024-1);
    // random_test<46>(1024-1);
    // random_test<47>(1024-1);
    // random_test<48>(1024-1);
    // random_test<49>(1024-1);

    // random_test<50>(1024-1);
    // random_test<51>(1024-1);
    // random_test<52>(1024-1);
    // random_test<53>(1024-1);
    // random_test<54>(1024-1);
//    random_test<55>(1024-1); SEGMENTATION FAULT
}


#endif // __SAME_BEHAVIOR_TEST_H__
