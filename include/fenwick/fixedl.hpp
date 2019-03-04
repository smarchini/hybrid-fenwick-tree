#ifndef __FENWICK_LNAIVE_HPP__
#define __FENWICK_LNAIVE_HPP__

#include "fenwick_tree.hpp"

namespace hft::fenwick {

/**
 * class FixedL - no compression and level-ordered node layout.
 * @sequence: sequence of integers.
 * @size: number of elements.
 * @BOUND: maximum value that @sequence can store.
 *
 */
template <size_t BOUND> class FixedL : public FenwickTree {
public:
  static constexpr size_t BOUNDSIZE = ceil_log2_plus1(BOUND);
  static_assert(BOUNDSIZE >= 1 && BOUNDSIZE <= 64,
                "Leaves can't be stored in a 64-bit word");
  static constexpr size_t LVL_PADDING = 8; // multiple of 8 for word alignment

protected:
  const size_t Size, Levels;
  DArray<uint64_t> Tree;
  unique_ptr<size_t[]> Level;

public:
  FixedL(uint64_t sequence[], size_t size)
      : Size(size), Levels(lambda(size + 1) + 2),
        Level(make_unique<size_t[]>(Levels)) {
    Level[0] = 0;
    for (size_t i = 1; i < Levels; i++)
      Level[i] = ((size + (1 << (i - 1))) / (1 << i)) + Level[i - 1] + (LVL_PADDING / 8);

    Tree = DArray<uint64_t>(Level[Levels - 1]);

    for (size_t l = 0; l < Levels - 1; l++) {
      for (size_t node = 1 << l; node <= size; node += 1 << (l + 1)) {
        size_t sequence_idx = node - 1;
        uint64_t value = sequence[sequence_idx];
        for (size_t j = 0; j < l; j++) {
          sequence_idx >>= 1;
          value += Tree[Level[j] + sequence_idx];
        }

        Tree[Level[l] + (node >> (l + 1))] = value;
      }
    }
  }

  virtual uint64_t prefix(size_t idx) const {
    uint64_t sum = 0;

    while (idx != 0) {
      int height = rho(idx);
      size_t level_idx = idx >> (1 + height);
      sum += Tree[Level[height] + level_idx];

      idx = clear_rho(idx);
    }

    return sum;
  }

  virtual void add(size_t idx, int64_t inc) {
    while (idx <= Size) {
      int height = rho(idx);
      size_t level_idx = idx >> (1 + height);
      Tree[Level[height] + level_idx] += inc;

      idx += mask_rho(idx);
    }
  }

  using FenwickTree::find;
  virtual size_t find(uint64_t *val) const {
    size_t node = 0, idx = 0;

    for (size_t height = Levels - 2; height != SIZE_MAX; height--) {
      size_t pos = Level[height] + idx;

      idx <<= 1;

      if (pos >= Level[height + 1] - LVL_PADDING / 8)
        continue;

      uint64_t value = Tree[pos];
      if (*val >= value) {
        idx++;
        *val -= value;
        node += 1 << height;
      }
    }

    return min(node, Size);
  }

  using FenwickTree::compFind;
  virtual size_t compFind(uint64_t *val) const {
    size_t node = 0, idx = 0;

    for (size_t height = Levels - 2; height != SIZE_MAX; height--) {
      size_t pos = Level[height] + idx;

      idx <<= 1;

      if (pos >= Level[height + 1] - LVL_PADDING / 8)
        continue;

      uint64_t value = (BOUND << height) - Tree[pos];
      if (*val >= value) {
        idx++;
        *val -= value;
        node += 1 << height;
      }
    }

    return min(node, Size);
  }

  virtual size_t size() const { return Size; }

  virtual size_t bitCount() const {
    return sizeof(FixedL<BOUNDSIZE>) * 8 +
           Tree.bitCount() - sizeof(Tree) +
           Levels * sizeof(size_t) * 8;
  }
};

} // namespace hft::fenwick

#endif // __FENWICK_LNAIVE_HPP__
