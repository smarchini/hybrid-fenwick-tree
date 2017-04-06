#ifndef __SIMPLE_FENWICK_TREE_H__
#define __SIMPLE_FENWICK_TREE_H__

#include "fenwick_tree.hpp"

/**
 * class SimpleFenwickTree - Naive implementation of a Fenwick Tree
 * @tree: Fenwick Tree data
 * @size: Number of elements
 *
 * Typical implementation of a FenwickTree data structure as described by the
 * original paper.
 */
class SimpleFenwickTree : public FenwickTree
{
public:
    static constexpr std::size_t LEAF_BITSIZE = 7;

protected:
    std::unique_ptr<std::uint64_t[]> tree;
    const std::size_t size;

public:
    /**
     * SimpleFenwickTree - Build a FenwickTree given a sequence of increments.
     * @sequence: Sequence of increments to compress.
     * @length: Number of elements stored by the sequence.
     *
     * Running time: O(@length)
     */
    SimpleFenwickTree(std::uint64_t sequence[], std::size_t length);

    virtual std::uint64_t get(std::size_t idx) const;

    virtual void set(std::size_t idx, std::int64_t inc);

    virtual std::size_t find(std::uint64_t val, bool complement=false) const;

    virtual std::size_t bit_count() const;
};

#endif // __SIMPLE_FENWICK_TREE_H__
