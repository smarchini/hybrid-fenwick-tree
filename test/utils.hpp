#ifndef __TEST_UTILS_H__
#define __TEST_UTILS_H__

#include <gtest/gtest.h>

#include <bitset>
#include <random>

#include "../include/common.hpp"

#include "../include/fenwick/fenwick_tree.hpp"
#include "../include/fenwick/simple.hpp"
#include "../include/fenwick/typed.hpp"
#include "../include/fenwick/ityped.hpp"
#include "../include/fenwick/byte.hpp"
#include "../include/fenwick/compact.hpp"
#include "../include/fenwick/shrank.hpp"

#include "../include/rankselect/rank_select.hpp"
#include "../include/rankselect/word.hpp"
#include "../include/rankselect/line.hpp"


// Exposed classes
// -----------------------------------------------------------------------------
class SimpleFenwickTree_Test : public dyn::SimpleFenwickTree<7> {
public:
    SimpleFenwickTree_Test(std::uint64_t sequence[], std::size_t length) :
        dyn::SimpleFenwickTree<7>(sequence, length) {}
    using dyn::SimpleFenwickTree<7>::tree;
};

class TypedFenwickTree_Test : public dyn::TypedFenwickTree<7> {
public:
    TypedFenwickTree_Test(std::uint64_t sequence[], std::size_t length) :
        dyn::TypedFenwickTree<7>(sequence, length) {}
    using dyn::TypedFenwickTree<7>::tree8;
    using dyn::TypedFenwickTree<7>::tree16;
    using dyn::TypedFenwickTree<7>::tree32;
    using dyn::TypedFenwickTree<7>::tree64;
};

class ITypedFenwickTree_Test : public dyn::ITypedFenwickTree<7> {
public:
    ITypedFenwickTree_Test(std::uint64_t sequence[], std::size_t length) :
        dyn::ITypedFenwickTree<7>(sequence, length) {}
    using dyn::ITypedFenwickTree<7>::tree;
};

class ByteFenwickTree_Test : public dyn::ByteFenwickTree<7> {
public:
    ByteFenwickTree_Test(std::uint64_t sequence[], std::size_t length) :
        dyn::ByteFenwickTree<7>(sequence, length) {}
    using dyn::ByteFenwickTree<7>::tree;
};

class CompactFenwickTree_Test : public dyn::CompactFenwickTree<7> {
public:
    CompactFenwickTree_Test(std::uint64_t sequence[], std::size_t length) :
        dyn::CompactFenwickTree<7>(sequence, length) {}
    using dyn::CompactFenwickTree<7>::tree;
};

class ShrankFenwickTree_Test : public dyn::ShrankFenwickTree<7> {
public:
    ShrankFenwickTree_Test(std::uint64_t sequence[], std::size_t length) :
        dyn::ShrankFenwickTree<7>(sequence, length) {}
    using dyn::ShrankFenwickTree<7>::tree;
};


// Test data
// -----------------------------------------------------------------------------
std::uint64_t inc1[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
std::uint64_t inc2[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
std::uint64_t inc3[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };


// Utility functions
// -----------------------------------------------------------------------------
void increments_to_sequence(std::uint64_t *inc, std::uint64_t *seq, std::size_t size)
{
    std::uint64_t curr = 0;
    for (std::size_t i = 0; i < size; i++) {
        curr += inc[i];
        seq[i] = curr;
    }
}

std::string tree_tostring(std::uint8_t *tree, std::size_t length) {

    std::string bit_tree;
    for (int i = length; i >= 0; i -= 8)
        bit_tree += std::bitset<8>(tree[i/8]).to_string();

    return bit_tree.substr(bit_tree.length() - length);
}

void fill_with_random_values(std::uint64_t array[], std::size_t size)
{
    static std::random_device rd;
    static std::mt19937 mte(rd());

    std::uniform_int_distribution<std::uint64_t> dist(1, 60); // 0, 64

    for (std::size_t i = 0; i < size; i++)
        array[i] = dist(mte);
}

#endif // __TEST_UTILS_H__
