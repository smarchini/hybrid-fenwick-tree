#ifndef __FENWICK_NAIVE_HPP__
#define __FENWICK_NAIVE_HPP__

#include "fenwick_tree.hpp"

namespace hft::fenwick {

/**
 * class FixedF - no compression and classical node layout.
 * @sequence: sequence of integers.
 * @size: number of elements.
 * @BOUND: maximum value that @sequence can store.
 *
 */
template <size_t BOUND> class FixedF : public FenwickTree {
public:
  static constexpr size_t BOUNDSIZE = log2(BOUND);
  static_assert(BOUNDSIZE >= 1 && BOUNDSIZE <= 64,
                "Leaves can't be stored in a 64-bit word");

protected:
  DArray<uint64_t> Tree;

public:
  FixedF(uint64_t sequence[], size_t size) : Tree(size) {
    std::copy_n(sequence, size, Tree.get());

    for (size_t m = 2; m <= size; m <<= 1) {
      for (size_t idx = m; idx <= size; idx += m)
        Tree[idx - 1] += Tree[idx - m / 2 - 1];
    }
  }

  virtual uint64_t prefix(size_t idx) const {
    uint64_t sum = 0;

    while (idx != 0) {
      sum += Tree[idx - 1];
      idx = clear_rho(idx);
    }

    return sum;
  }

  virtual void add(size_t idx, int64_t inc) {
    size_t treeSize = Tree.size();
    while (idx <= treeSize) {
      Tree[idx - 1] += inc;
      idx += mask_rho(idx);
    }
  }

  using FenwickTree::find;
  virtual size_t find(uint64_t *val) const {
    size_t treeSize = Tree.size();
    size_t node = 0;

    for (size_t m = mask_lambda(treeSize); m != 0; m >>= 1) {
      if (node + m - 1 >= treeSize)
        continue;

      uint64_t value = Tree[node + m - 1];

      if (*val >= value) {
        node += m;
        *val -= value;
      }
    }

    return node;
  }

  using FenwickTree::compFind;
  virtual size_t compFind(uint64_t *val) const {
    size_t node = 0, treeSize = Tree.size();

    for (size_t m = mask_lambda(treeSize); m != 0; m >>= 1) {
      if (node + m - 1 >= treeSize)
        continue;

      uint64_t value = (BOUND << rho(node + m)) - Tree[node + m - 1];

      if (*val >= value) {
        node += m;
        *val -= value;
      }
    }

    return node;
  }

  virtual size_t size() const { return Tree.size(); }

  virtual size_t bitCount() const {
    return sizeof(FixedF<BOUNDSIZE>) * 8 + Tree.bitCount() - sizeof(Tree);
  }
};

} // namespace hft::fenwick

#endif // __FENWICK_NAIVE_HPP__
