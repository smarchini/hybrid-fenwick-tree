#ifndef __TEST_FENWICK_HYBRID_HPP__
#define __TEST_FENWICK_HYBRID_HPP__

#include "../utils.hpp"

template <typename T>
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

    hft::fenwick::FixedF<64> naive(increments, size);
    T mixed(increments, size);

    // prefix
    for (size_t i = 0; i < size; i++)
        EXPECT_EQ(naive.prefix(i), mixed.prefix(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate argument: " << typeid(T).name();

    // find
    for (std::uint64_t i = 0; i < size; i++)
        EXPECT_EQ(naive.find(i), mixed.find(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate argument: " << typeid(T).name();

    // add
    for (size_t i = 0; i < size; i++) {
        naive.add(i, add_updates[i]);
        mixed.add(i, add_updates[i]);
    }

    // prefix
    for (size_t i = 0; i < size; i++)
        EXPECT_EQ(naive.prefix(i), mixed.prefix(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate argument: " << typeid(T).name();

    // find complement
    for (std::uint64_t i = 0; i < size; i++)
        EXPECT_EQ(naive.compfind(i), mixed.compfind(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate argument: " << typeid(T).name();

    delete[] increments;
    delete[] add_updates;
}


TEST(fenwick_mixed, naive)
{
    using namespace hft::fenwick;

    for (std::size_t i = 1; i < 10; i++)
        fenwick_random_test<Hybrid<FixedF, FixedF, 64, 11>>(i);
    for (std::size_t i = 1000; i < 1010; i++)
        fenwick_random_test<Hybrid<FixedF, FixedF, 64, 11>>(i);
    for (std::size_t i = 100000; i < 100010; i++)
        fenwick_random_test<Hybrid<FixedF, FixedF, 64, 11>>(i);
}

TEST(fenwick_mixed, type)
{
    using namespace hft::fenwick;

    for (std::size_t i = 1; i < 10; i++)
        fenwick_random_test<Hybrid<TypeL, TypeF, 64, 11>>(i);
    for (std::size_t i = 1000; i < 1010; i++)
        fenwick_random_test<Hybrid<TypeL, TypeF, 64, 11>>(i);
    for (std::size_t i = 100000; i < 100010; i++)
        fenwick_random_test<Hybrid<TypeL, TypeF, 64, 11>>(i);

    for (std::size_t i = 1; i < 10; i++)
        fenwick_random_test<Hybrid<TypeF, TypeL, 64, 11>>(i);
    for (std::size_t i = 1000; i < 1010; i++)
        fenwick_random_test<Hybrid<TypeF, TypeL, 64, 11>>(i);
    for (std::size_t i = 100000; i < 100010; i++)
        fenwick_random_test<Hybrid<TypeF, TypeL, 64, 11>>(i);

}

TEST(fenwick_mixed, byte)
{
    using namespace hft::fenwick;

    for (std::size_t i = 1; i < 10; i++)
        fenwick_random_test<Hybrid<ByteL, ByteF, 64, 11>>(i);
    for (std::size_t i = 1000; i < 1010; i++)
        fenwick_random_test<Hybrid<ByteL, ByteF, 64, 11>>(i);
    for (std::size_t i = 100000; i < 100010; i++)
        fenwick_random_test<Hybrid<ByteL, ByteF, 64, 11>>(i);

    for (std::size_t i = 1; i < 10; i++)
        fenwick_random_test<Hybrid<ByteF, ByteL, 64, 11>>(i);
    for (std::size_t i = 1000; i < 1010; i++)
        fenwick_random_test<Hybrid<ByteF, ByteL, 64, 11>>(i);
    for (std::size_t i = 100000; i < 100010; i++)
        fenwick_random_test<Hybrid<ByteF, ByteL, 64, 11>>(i);
}

TEST(fenwick_mixed, bit)
{
    using namespace hft::fenwick;

    for (std::size_t i = 1; i < 10; i++)
        fenwick_random_test<Hybrid<BitL, BitF, 64, 11>>(i);
    for (std::size_t i = 1000; i < 1010; i++)
        fenwick_random_test<Hybrid<BitL, BitF, 64, 11>>(i);
    for (std::size_t i = 100000; i < 100010; i++)
        fenwick_random_test<Hybrid<BitL, BitF, 64, 11>>(i);

    for (std::size_t i = 1; i < 10; i++)
        fenwick_random_test<Hybrid<BitF, BitL, 64, 11>>(i);
    for (std::size_t i = 1000; i < 1010; i++)
        fenwick_random_test<Hybrid<BitF, BitL, 64, 11>>(i);
    for (std::size_t i = 100000; i < 100010; i++)
        fenwick_random_test<Hybrid<BitF, BitL, 64, 11>>(i);
}


#endif // __TEST_FENWICK_HYBRID_HPP__
