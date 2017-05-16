#ifndef __TEST_UTILS_H__
#define __TEST_UTILS_H__

#include <gtest/gtest.h>

#include <bitset>
#include <random>

#include "../include/common.hpp"

#include "../include/fenwick/fenwick_tree.hpp"
#include "../include/fenwick/naive.hpp"
#include "../include/fenwick/type.hpp"
#include "../include/fenwick/ltype.hpp"
#include "../include/fenwick/byte.hpp"
#include "../include/fenwick/lbyte.hpp"
#include "../include/fenwick/bit.hpp"
#include "../include/fenwick/lbit.hpp"
#include "../include/fenwick/mixed.hpp"

#include "../include/rankselect/rank_select.hpp"
#include "../include/rankselect/word.hpp"
#include "../include/rankselect/line.hpp"


// Exposed classes
// -----------------------------------------------------------------------------
class NaiveFenwickTree_Test : public dyn::NaiveFenwickTree<64> {
public:
    NaiveFenwickTree_Test(std::uint64_t sequence[], std::size_t length) :
        dyn::NaiveFenwickTree<64>(sequence, length) {}
    using dyn::NaiveFenwickTree<64>::tree;
};

class LTypeFenwickTree_Test : public dyn::LTypeFenwickTree<64> {
public:
    LTypeFenwickTree_Test(std::uint64_t sequence[], std::size_t length) :
        dyn::LTypeFenwickTree<64>(sequence, length) {}
    using dyn::LTypeFenwickTree<64>::tree8;
    using dyn::LTypeFenwickTree<64>::tree16;
    using dyn::LTypeFenwickTree<64>::tree64;
};

class TypeFenwickTree_Test : public dyn::TypeFenwickTree<64> {
public:
    TypeFenwickTree_Test(std::uint64_t sequence[], std::size_t length) :
        dyn::TypeFenwickTree<64>(sequence, length) {}
    using dyn::TypeFenwickTree<64>::tree;
};

class LByteFenwickTree_Test : public dyn::LByteFenwickTree<64> {
public:
    LByteFenwickTree_Test(std::uint64_t sequence[], std::size_t length) :
        dyn::LByteFenwickTree<64>(sequence, length) {}
    using dyn::LByteFenwickTree<64>::tree;
};

class ByteFenwickTree_Test : public dyn::ByteFenwickTree<64> {
public:
    ByteFenwickTree_Test(std::uint64_t sequence[], std::size_t length) :
        dyn::ByteFenwickTree<64>(sequence, length) {}
    using dyn::ByteFenwickTree<64>::tree;
};

class LBitFenwickTree_Test : public dyn::LBitFenwickTree<64> {
public:
    LBitFenwickTree_Test(std::uint64_t sequence[], std::size_t length) :
        dyn::LBitFenwickTree<64>(sequence, length) {}
    using dyn::LBitFenwickTree<64>::tree;
};

class BitFenwickTree_Test : public dyn::BitFenwickTree<64> {
public:
    BitFenwickTree_Test(std::uint64_t sequence[], std::size_t length) :
        dyn::BitFenwickTree<64>(sequence, length) {}
    using dyn::BitFenwickTree<64>::tree;
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

#endif // __TEST_UTILS_H__
