#ifndef __TEST_UTILS_HPP__
#define __TEST_UTILS_HPP__

#include <gtest/gtest.h>

#include <bitset>
#include <random>

#include "../include/common.hpp"

#include "../include/fenwick/fenwick_tree.hpp"
#include "../include/fenwick/fixedf.hpp"
#include "../include/fenwick/fixedl.hpp"
#include "../include/fenwick/typef.hpp"
#include "../include/fenwick/typel.hpp"
#include "../include/fenwick/bytef.hpp"
#include "../include/fenwick/bytel.hpp"
#include "../include/fenwick/bitf.hpp"
#include "../include/fenwick/bitl.hpp"
#include "../include/fenwick/hybrid.hpp"

#include "../include/rankselect/rank_select.hpp"
#include "../include/rankselect/word.hpp"
#include "../include/rankselect/stride.hpp"


// Exposed classes
// -----------------------------------------------------------------------------
class FixedF_Test : public hft::fenwick::FixedF<64> {
public:
    FixedF_Test(std::uint64_t sequence[], std::size_t length) :
        hft::fenwick::FixedF<64>(sequence, length) {}
    using hft::fenwick::FixedF<64>::Tree;
};

class TypeL_Test : public hft::fenwick::TypeL<64> {
public:
    TypeL_Test(std::uint64_t sequence[], std::size_t length) :
        hft::fenwick::TypeL<64>(sequence, length) {}
    using hft::fenwick::TypeL<64>::Tree8;
    using hft::fenwick::TypeL<64>::Tree16;
    using hft::fenwick::TypeL<64>::Tree64;
};

class TypeF_Test : public hft::fenwick::TypeF<64> {
public:
    TypeF_Test(std::uint64_t sequence[], std::size_t length) :
        hft::fenwick::TypeF<64>(sequence, length) {}
    using hft::fenwick::TypeF<64>::Tree;
};

class ByteL_Test : public hft::fenwick::ByteL<64> {
public:
    ByteL_Test(std::uint64_t sequence[], std::size_t length) :
        hft::fenwick::ByteL<64>(sequence, length) {}
    using hft::fenwick::ByteL<64>::Tree;
};

class ByteF_Test : public hft::fenwick::ByteF<64> {
public:
    ByteF_Test(std::uint64_t sequence[], std::size_t length) :
        hft::fenwick::ByteF<64>(sequence, length) {}
    using hft::fenwick::ByteF<64>::Tree;
};

class BitL_Test : public hft::fenwick::BitL<64> {
public:
    BitL_Test(std::uint64_t sequence[], std::size_t length) :
        hft::fenwick::BitL<64>(sequence, length) {}
    using hft::fenwick::BitL<64>::Tree;
};

class BitF_Test : public hft::fenwick::BitF<64> {
public:
    BitF_Test(std::uint64_t sequence[], std::size_t length) :
        hft::fenwick::BitF<64>(sequence, length) {}
    using hft::fenwick::BitF<64>::Tree;
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

#endif // __TEST_UTILS_HPP__
