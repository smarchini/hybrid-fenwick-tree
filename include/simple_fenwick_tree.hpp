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
protected:
    std::uint64_t *tree = nullptr;
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

    virtual ~SimpleFenwickTree();

    virtual std::uint64_t get(std::size_t idx) const;

    virtual void set(std::size_t idx, std::uint64_t inc);

    virtual std::uint64_t find(std::uint64_t val) const;
};

#endif // __SIMPLE_FENWICK_TREE_H__
