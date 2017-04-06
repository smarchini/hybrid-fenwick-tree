#ifndef __BYTE_FENWICK_TREE_H__
#define __BYTE_FENWICK_TREE_H__

#include "fenwick_tree.hpp"

/**
 * class ByteFenwickTree
 * @tree: Byte indexted Fenwick Tree data.
 * @size: Number of elements in the tree.
 * @levels: Number of levels of the tree.
 * @level_start: Lookup table, it store the starting bit-position of each level.
 *
 * Each node use the minimum number of bytes it needs. It's supposed to store
 * increments up to 63, so a single byte per element is enough up to level 3.
 * Then, every 8 levels you add a byte per element.
 *
 * The data is stored in a bottom-up level-order manner.
 */
class ByteFenwickTree : public FenwickTree
{
public:
    static constexpr std::size_t LEAF_BITSIZE = 7;

    // TODO: provare a sostituire con compact_bitmask (calcolata a runtime).
    // l'array è il modo più efficiente su i7 4770.
    static constexpr std::uint64_t MASK[] = { 0x0ULL,
                                              0xFFULL,
                                              0xFFFFULL,
                                              0xFFFFFFULL,
                                              0xFFFFFFFFULL,
                                              0xFFFFFFFFFFULL,
                                              0xFFFFFFFFFFFFULL,
                                              0xFFFFFFFFFFFFFFULL,
                                              0xFFFFFFFFFFFFFFFFULL };
protected:
    std::unique_ptr<std::uint8_t[]> tree;
    const std::size_t size;
    const std::size_t levels;
    std::unique_ptr<std::size_t[]> level_start;

public:
    /**
     * ByteFenwickTree - Build a FenwickTree given a sequence of increments.
     * @sequence: Sequence of increments.
     * @size: Number of elements stored by the sequence.
     *
     * Running time: O(length)
     */
    ByteFenwickTree(std::uint64_t sequence[], std::size_t size);

    virtual std::uint64_t get(std::size_t idx) const;

    virtual void set(std::size_t idx, std::int64_t inc);

    virtual std::size_t find(std::uint64_t val, bool complement=false) const;

    virtual std::size_t bit_count() const;
};

#endif // __BYTE_FENWICK_TREE_H__
