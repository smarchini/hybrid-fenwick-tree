#ifndef __SAME_BEHAVIOR_TEST_H__
#define __SAME_BEHAVIOR_TEST_H__

#include <gtest/gtest.h>
#include <cstdint>
#include "./test_utils.hpp"

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

    SimpleFenwickTree_Test simple(increments, size);
    CompactFenwickTree_Test compact(increments, size);
    ByteFenwickTree_Test byte(increments, size);
    TypedFenwickTree_Test typed(increments, size);
    ShrankFenwickTree_Test shrank(increments, size);

    // get
    for (size_t i = 0; i < size; i++) {
        std::uint64_t simple_get = simple.get(i);

        EXPECT_EQ(simple_get, compact.get(i)) << "at index " << i;
        EXPECT_EQ(simple_get, byte.get(i)) << "at index " << i;
        EXPECT_EQ(simple_get, typed.get(i)) << "at index " << i;
        EXPECT_EQ(simple_get, shrank.get(i)) << "at index " << i;
    }

    // find
    for (std::uint64_t i = 0; i < size; i++) {
        std::uint64_t simple_find = simple.find(i);

        EXPECT_EQ(simple_find, compact.find(i)) << "at index " << i;
        EXPECT_EQ(simple_find, byte.find(i)) << "at index " << i;
        EXPECT_EQ(simple_find, typed.find(i)) << "at index " << i;
        EXPECT_EQ(simple_find, shrank.find(i)) << "at index " << i;
    }

    // set
    for (size_t i = 0; i < size; i++) {
        simple.set(i, set_updates[i]);
        compact.set(i, set_updates[i]);
        byte.set(i, set_updates[i]);
        typed.set(i, set_updates[i]);
        shrank.set(i, set_updates[i]);
    }

    // get
    for (size_t i = 0; i < size; i++) {
        std::uint64_t simple_get = simple.get(i);

        EXPECT_EQ(simple_get, compact.get(i)) << "at index " << i;
        EXPECT_EQ(simple_get, byte.get(i)) << "at index " << i;
        EXPECT_EQ(simple_get, typed.get(i)) << "at index " << i;
        EXPECT_EQ(simple_get, shrank.get(i)) << "at index " << i;
    }

    // bit_count
    EXPECT_EQ(shrank.bit_count(), compact.bit_count());

    delete[] increments;
    delete[] set_updates;
}


TEST(same_behavior, random_small)
{
    random_test(2-1);
    random_test(4-1);
    random_test(8-1);
    random_test(16-1);
    random_test(32-1);
    random_test(64-1);
    random_test(128-1);
    random_test(256-1);
    random_test(512-1);
    random_test(1024-1);
}

TEST(same_behavior, random_big)
{
    random_test(2*1024-1);
    random_test(4*1024-1);
    random_test(8*1024-1);
    random_test(16*1024-1);
    random_test(32*1024-1);
    random_test(64*1024-1);
    random_test(128*1024-1);
    random_test(256*1024-1);
    random_test(512*1024-1);
    random_test(1024*1024-1);
}

TEST(same_behavior, partial_tree)
{
    for (std::size_t i = 1; i < 10000; i++)
        random_test(i);
}

#endif // __SAME_BEHAVIOR_TEST_H__
