#ifndef __COMPACT_FENWICK_TREE_H__
#define __COMPACT_FENWICK_TREE_H__

#include "fenwick_tree.hpp"

/**
 * class CompactFenwickTree
 * @tree: Byte indexted Fenwick Tree data.
 * @size: Number of elements in the tree.
 * @levels: Number of levels of the tree.
 * @level_start: Lookup table, it store the starting bit-position of each level.
 *
 * Each node use exactly the number of bits it needs. It's supposed to store
 * increments up to 64, so each leaf needs 7 bit (LEAF_BITSIZE).
 *
 * The data is stored in a bottom-up level-order manner. Let @n be the number of
 * elements in the Fenwick Tree:
 * - from bit 0 to bit 7*n/2: leaves
 * - from bit 7*n/2 to bit 7*n/2+7*n/4: height 1 internal nodes
 * and so on until the end of the tree.
 */
class CompactFenwickTree : public FenwickTree
{
public:
    static constexpr std::size_t LEAF_BITSIZE = 7;

protected:
    const size_t size;
    DArray<std::uint8_t> tree;
    DArray<std::size_t> level;

public:
    /**
     * CompactFenwickTree - Build a FenwickTree given a sequence of increments.
     * @sequence: Sequence of increments.
     * @size: Number of elements stored by the sequence.
     *
     * Running time: O(length)
     */
    CompactFenwickTree(std::uint64_t sequence[], std::size_t size);

    virtual std::uint64_t get(std::size_t idx) const;

    virtual void set(std::size_t idx, std::int64_t inc);

    virtual std::size_t find(std::uint64_t val, bool complement=false) const;

    virtual std::size_t bit_count() const;
};

#endif // __COMPACT_FENWICK_TREE_H__
