#ifndef __TEST_UTILS_H__
#define __TEST_UTILS_H__

#include <cstdint>
#include <bitset>
#include <random>

#include "../include/simple_fenwick_tree.hpp"
#include "../include/compact_fenwick_tree.hpp"
#include "../include/byte_fenwick_tree.hpp"
#include "../include/typed_fenwick_tree.hpp"
#include "../include/shrank_fenwick_tree.hpp"
#include "../include/dynamic_rank_select.hpp"


// Exposed classes
// -----------------------------------------------------------------------------
class SimpleFenwickTree_Test : public SimpleFenwickTree {
public:
    SimpleFenwickTree_Test(std::uint64_t sequence[], std::size_t length) :
        SimpleFenwickTree(sequence, length) {}
    using SimpleFenwickTree::tree;
};

class CompactFenwickTree_Test : public CompactFenwickTree {
public:
    CompactFenwickTree_Test(std::uint64_t sequence[], std::size_t length) :
        CompactFenwickTree(sequence, length) {}
    using CompactFenwickTree::tree;
};

class ByteFenwickTree_Test : public ByteFenwickTree {
public:
    ByteFenwickTree_Test(std::uint64_t sequence[], std::size_t length) :
        ByteFenwickTree(sequence, length) {}
    using ByteFenwickTree::tree;
};

class TypedFenwickTree_Test : public TypedFenwickTree {
public:
    TypedFenwickTree_Test(std::uint64_t sequence[], std::size_t length) :
        TypedFenwickTree(sequence, length) {}
    using TypedFenwickTree::tree8;
    using TypedFenwickTree::tree16;
    using TypedFenwickTree::tree32;
    using TypedFenwickTree::tree64;
};

class ShrankFenwickTree_Test : public ShrankFenwickTree {
public:
    ShrankFenwickTree_Test(std::uint64_t sequence[], std::size_t length) :
        ShrankFenwickTree(sequence, length) {}
    using ShrankFenwickTree::tree;
};

template<typename T>
class DynRankSelect_Test : public DynRankSelect<T> {
public:
    DynRankSelect_Test(std::uint64_t bitvector[], std::size_t length):
        DynRankSelect<T>(bitvector, length) {}
    using DynRankSelect<T>::fenwick;
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

    std::uniform_int_distribution<std::uint64_t> dist(0, 63);

    for (std::size_t i = 0; i < size; i++)
        array[i] = dist(mte);
}

#endif // __TEST_UTILS_H__
