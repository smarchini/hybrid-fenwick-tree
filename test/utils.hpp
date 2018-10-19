#ifndef __TEST_UTILS_H__
#define __TEST_UTILS_H__

#include <gtest/gtest.h>

#include <bitset>
#include <random>

#include "../include/common.hpp"

#include "../include/fenwick/fenwick_tree.hpp"
#include "../include/fenwick/naive.hpp"
#include "../include/fenwick/lnaive.hpp"
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
class FixedF_Test : public dyn::FixedF<64> {
public:
    FixedF_Test(std::uint64_t sequence[], std::size_t length) :
        dyn::FixedF<64>(sequence, length) {}
    using dyn::FixedF<64>::tree;
};

class TypeL_Test : public dyn::TypeL<64> {
public:
    TypeL_Test(std::uint64_t sequence[], std::size_t length) :
        dyn::TypeL<64>(sequence, length) {}
    using dyn::TypeL<64>::tree8;
    using dyn::TypeL<64>::tree16;
    using dyn::TypeL<64>::tree64;
};

class TypeF_Test : public dyn::TypeF<64> {
public:
    TypeF_Test(std::uint64_t sequence[], std::size_t length) :
        dyn::TypeF<64>(sequence, length) {}
    using dyn::TypeF<64>::tree;
};

class ByteL_Test : public dyn::ByteL<64> {
public:
    ByteL_Test(std::uint64_t sequence[], std::size_t length) :
        dyn::ByteL<64>(sequence, length) {}
    using dyn::ByteL<64>::tree;
};

class ByteF_Test : public dyn::ByteF<64> {
public:
    ByteF_Test(std::uint64_t sequence[], std::size_t length) :
        dyn::ByteF<64>(sequence, length) {}
    using dyn::ByteF<64>::tree;
};

class BitL_Test : public dyn::BitL<64> {
public:
    BitL_Test(std::uint64_t sequence[], std::size_t length) :
        dyn::BitL<64>(sequence, length) {}
    using dyn::BitL<64>::tree;
};

class BitF_Test : public dyn::BitF<64> {
public:
    BitF_Test(std::uint64_t sequence[], std::size_t length) :
        dyn::BitF<64>(sequence, length) {}
    using dyn::BitF<64>::tree;
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
