#ifndef __FENWICK_LNAIVE_HPP__
#define __FENWICK_LNAIVE_HPP__

#include "fenwick_tree.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

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
  static constexpr size_t BOUNDSIZE = log2(BOUND);
  static_assert(BOUNDSIZE >= 1 && BOUNDSIZE <= 64,
                "Leaves can't be stored in a 64-bit word");

protected:
  const size_t Levels;
  DArray<uint64_t> Tree;
  unique_ptr<size_t[]> Level;

  std::array<uint64_t, 4096> addrprefix, addradd, addrfind;

public:
  FixedL(uint64_t sequence[], size_t size)
      : Levels(lambda(size + 1) + 2), Tree(size),
        Level(make_unique<size_t[]>(Levels)) {
    Level[0] = 0;
    for (size_t i = 1; i < Levels; i++)
      Level[i] = ((size + (1 << (i - 1))) / (1 << i)) + Level[i - 1];

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

  virtual uint64_t prefix(size_t idx) {
    uint64_t sum = 0;

    while (idx != 0) {
      int height = rho(idx);
      size_t level_idx = idx >> (1 + height);
      sum += Tree[Level[height] + level_idx];
      addrprefix[(uint64_t)(&Tree[Level[height] + level_idx]) % 4096]++;

      idx = clear_rho(idx);
    }

    return sum;
  }

  virtual void add(size_t idx, int64_t inc) {
    size_t tree_size = Tree.size();

    while (idx <= tree_size) {
      int height = rho(idx);
      size_t level_idx = idx >> (1 + height);
      Tree[Level[height] + level_idx] += inc;
      addradd[(uint64_t)(&Tree[Level[height] + level_idx]) % 4096]++;

      idx += mask_rho(idx);
    }
  }

  using FenwickTree::find;
  virtual size_t find(uint64_t *val) {
    size_t node = 0, idx = 0;

    for (size_t height = Levels - 2; height != SIZE_MAX; height--) {
      size_t pos = Level[height] + idx;

      idx <<= 1;

      if (pos >= Level[height + 1])
        continue;

      uint64_t value = Tree[pos];
      addrfind[(uint64_t)(&Tree[pos]) % 4096]++;

      if (*val >= value) {
        idx++;
        *val -= value;
        node += 1 << height;
      }
    }

    return min(node, size());
  }

  using FenwickTree::compFind;
  virtual size_t compFind(uint64_t *val) {
    size_t node = 0, idx = 0;

    for (size_t height = Levels - 2; height != SIZE_MAX; height--) {
      size_t pos = Level[height] + idx;

      idx <<= 1;

      if (pos >= Level[height + 1])
        continue;

      uint64_t value = (BOUND << height) - Tree[pos];
      if (*val >= value) {
        idx++;
        *val -= value;
        node += 1 << height;
      }
    }

    return min(node, size());
  }

  virtual size_t size() { return Tree.size(); }

  virtual size_t bitCount() {
    return sizeof(FixedL<BOUNDSIZE>) * 8 +
           Tree.bitCount() - sizeof(Tree) +
           Levels * sizeof(size_t) * 8;
  }

  ~FixedL() {
    using namespace std;
    ofstream fprefix(std::string("address_FixedF_prefix_") + STRINGIFY(MAGIC) + ".txt", std::ofstream::app);
    ofstream fadd(std::string("address_FixedF_add_") + STRINGIFY(MAGIC) + ".txt", std::ofstream::app);
    ofstream ffind(std::string("address_FixedF_find_") + STRINGIFY(MAGIC) + ".txt", std::ofstream::app);

    if (any_of(begin(addrprefix), end(addrprefix), [](int i) { return i != 0; }))
        for (auto i: addrprefix) fprefix << i << "\n";

    if (any_of(begin(addradd), end(addradd), [](int i) { return i != 0; }))
        for (auto i: addradd) fadd << i << "\n";

    if (any_of(begin(addrfind), end(addrfind), [](int i) { return i != 0; }))
        for (auto i: addrfind) ffind << i << "\n";
  }
};

} // namespace hft::fenwick

#endif // __FENWICK_LNAIVE_HPP__
