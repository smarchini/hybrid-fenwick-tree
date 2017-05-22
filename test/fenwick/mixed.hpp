#ifndef __TEST_FENWICK_MIXED_H__
#define __TEST_FENWICK_MIXED_H__

#include "../utils.hpp"

template <typename T>
void fenwick_random_test(std::size_t size)
{
    static std::mt19937 mte;
    std::uniform_int_distribution<std::uint64_t> dist(0, 64);

    std::uint64_t *increments = new std::uint64_t[size];
    std::int64_t *set_updates = new std::int64_t[size];

    for (std::size_t i = 0; i < size; i++)
        increments[i] = dist(mte);

    for (std::size_t i = 0; i < size; i++) {
        std::uniform_int_distribution<std::int64_t> sdist(0, 64 - increments[i]);
        set_updates[i] = sdist(mte);
    }

    dyn::NaiveFenwickTree<64> naive(increments, size);
    T mixed(increments, size);

    // get
    for (size_t i = 0; i < size; i++)
        EXPECT_EQ(naive.get(i), mixed.get(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate argument: " << typeid(T).name();

    // find
    for (std::uint64_t i = 0; i < size; i++)
        EXPECT_EQ(naive.find(i), mixed.find(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate argument: " << typeid(T).name();

    // set
    for (size_t i = 0; i < size; i++) {
        naive.set(i, set_updates[i]);
        mixed.set(i, set_updates[i]);
    }

    // get
    for (size_t i = 0; i < size; i++)
        EXPECT_EQ(naive.get(i), mixed.get(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate argument: " << typeid(T).name();

    // find complement
    for (std::uint64_t i = 0; i < size; i++)
        EXPECT_EQ(naive.find_complement(i), mixed.find_complement(i)) << "At index: " << i << "\nsize: " << size << "\ntemplate argument: " << typeid(T).name();

    delete[] increments;
    delete[] set_updates;
}


TEST(fenwick_mixed, naive)
{
    using namespace dyn;

    for (std::size_t i = 1; i < 10; i++)
        fenwick_random_test<MixedFenwickTree<NaiveFenwickTree, NaiveFenwickTree, 64, 11>>(i);
    for (std::size_t i = 1000; i < 1010; i++)
        fenwick_random_test<MixedFenwickTree<NaiveFenwickTree, NaiveFenwickTree, 64, 11>>(i);
    for (std::size_t i = 100000; i < 100010; i++)
        fenwick_random_test<MixedFenwickTree<NaiveFenwickTree, NaiveFenwickTree, 64, 11>>(i);
}

TEST(fenwick_mixed, type)
{
    using namespace dyn;

    for (std::size_t i = 1; i < 10; i++)
        fenwick_random_test<MixedFenwickTree<LTypeFenwickTree, TypeFenwickTree, 64, 11>>(i);
    for (std::size_t i = 1000; i < 1010; i++)
        fenwick_random_test<MixedFenwickTree<LTypeFenwickTree, TypeFenwickTree, 64, 11>>(i);
    for (std::size_t i = 100000; i < 100010; i++)
        fenwick_random_test<MixedFenwickTree<LTypeFenwickTree, TypeFenwickTree, 64, 11>>(i);

    for (std::size_t i = 1; i < 10; i++)
        fenwick_random_test<MixedFenwickTree<TypeFenwickTree, LTypeFenwickTree, 64, 11>>(i);
    for (std::size_t i = 1000; i < 1010; i++)
        fenwick_random_test<MixedFenwickTree<TypeFenwickTree, LTypeFenwickTree, 64, 11>>(i);
    for (std::size_t i = 100000; i < 100010; i++)
        fenwick_random_test<MixedFenwickTree<TypeFenwickTree, LTypeFenwickTree, 64, 11>>(i);

}

TEST(fenwick_mixed, byte)
{
    using namespace dyn;

    for (std::size_t i = 1; i < 10; i++)
        fenwick_random_test<MixedFenwickTree<LByteFenwickTree, ByteFenwickTree, 64, 11>>(i);
    for (std::size_t i = 1000; i < 1010; i++)
        fenwick_random_test<MixedFenwickTree<LByteFenwickTree, ByteFenwickTree, 64, 11>>(i);
    for (std::size_t i = 100000; i < 100010; i++)
        fenwick_random_test<MixedFenwickTree<LByteFenwickTree, ByteFenwickTree, 64, 11>>(i);

    for (std::size_t i = 1; i < 10; i++)
        fenwick_random_test<MixedFenwickTree<ByteFenwickTree, LByteFenwickTree, 64, 11>>(i);
    for (std::size_t i = 1000; i < 1010; i++)
        fenwick_random_test<MixedFenwickTree<ByteFenwickTree, LByteFenwickTree, 64, 11>>(i);
    for (std::size_t i = 100000; i < 100010; i++)
        fenwick_random_test<MixedFenwickTree<ByteFenwickTree, LByteFenwickTree, 64, 11>>(i);
}

TEST(fenwick_mixed, bit)
{
    using namespace dyn;

    for (std::size_t i = 1; i < 10; i++)
        fenwick_random_test<MixedFenwickTree<LBitFenwickTree, BitFenwickTree, 64, 11>>(i);
    for (std::size_t i = 1000; i < 1010; i++)
        fenwick_random_test<MixedFenwickTree<LBitFenwickTree, BitFenwickTree, 64, 11>>(i);
    for (std::size_t i = 100000; i < 100010; i++)
        fenwick_random_test<MixedFenwickTree<LBitFenwickTree, BitFenwickTree, 64, 11>>(i);

    for (std::size_t i = 1; i < 10; i++)
        fenwick_random_test<MixedFenwickTree<BitFenwickTree, LBitFenwickTree, 64, 11>>(i);
    for (std::size_t i = 1000; i < 1010; i++)
        fenwick_random_test<MixedFenwickTree<BitFenwickTree, LBitFenwickTree, 64, 11>>(i);
    for (std::size_t i = 100000; i < 100010; i++)
        fenwick_random_test<MixedFenwickTree<BitFenwickTree, LBitFenwickTree, 64, 11>>(i);
}


#endif // __TEST_FENWICK_MIXED_H__
