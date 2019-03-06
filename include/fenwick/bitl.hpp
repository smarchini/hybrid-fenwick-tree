#ifndef __FENWICK_LBIT_HPP__
#define __FENWICK_LBIT_HPP__

#include "fenwick_tree.hpp"

namespace hft::fenwick {

/**
 * class BitL - bit compression and level ordered node layout.
 * @sequence: sequence of integers.
 * @size: number of elements.
 * @BOUND: maximum value that @sequence can store.
 *
 */
template <size_t BOUND> class BitL : public FenwickTree {
public:
  static constexpr size_t BOUNDSIZE = ceil_log2_plus1(BOUND);
  static_assert(BOUNDSIZE >= 1 && BOUNDSIZE <= 57,
                "Some nodes will span on multiple words");

protected:
  const size_t Size, Levels;
  DArray<uint8_t> Tree;
  unique_ptr<size_t[]> Level;

public:
  BitL(uint64_t sequence[], size_t size)
      : Size(size), Levels(lambda(size) + 2),
        Level(make_unique<size_t[]>(Levels)) {
    Level[0] = 0;
    for (size_t i = 1; i < Levels; i++)
      Level[i] = ((size + (1ULL << (i - 1))) / (1ULL << i)) * (BOUNDSIZE - 1 + i) +
          Level[i - 1];

    Tree = DArray<uint8_t>((Level[Levels - 1] >> 3) + 8); // +8 for safety

    for (size_t l = 0; l < Levels - 1; l++) {
      for (size_t node = 1ULL << l; node <= size; node += 1ULL << (l + 1)) {
        size_t highpos = Level[l] + (BOUNDSIZE + l) * (node >> (l + 1));
        size_t highshift = highpos & 0b111;
        uint64_t highmask = compact_bitmask(BOUNDSIZE + l, highshift);
        auint64_t &high = reinterpret_cast<auint64_t &>(Tree[highpos >> 3]);

        size_t sequence_idx = node - 1;
        uint64_t value = sequence[sequence_idx];

        for (size_t j = 0; j < l; j++) {
          sequence_idx >>= 1;
          size_t lowpos = Level[j] + (BOUNDSIZE + j) * sequence_idx;
          size_t lowshift = lowpos & 0b111;
          uint64_t lowmask = compact_bitmask(BOUNDSIZE + j, lowshift);
          uint64_t low = *reinterpret_cast<auint64_t *>(&Tree[lowpos >> 3]);

          value += (low & lowmask) >> lowshift;
        }

        high &= ~highmask;
        high |= (value << highshift) & highmask;
      }
    }
  }

  virtual uint64_t prefix(size_t idx) const {
    uint64_t sum = 0;

    while (idx != 0) {
      int height = rho(idx);
      size_t pos = Level[height] + (idx >> (1 + height)) * (BOUNDSIZE + height);
      auint64_t element = *reinterpret_cast<auint64_t *>(&Tree[pos >> 3]);

      sum += bitextract(element, pos & 0b111, BOUNDSIZE + height);
      idx = clear_rho(idx);
    }

    return sum;
  }

  virtual void add(size_t idx, int64_t inc) {
    while (idx <= Size) {
      int height = rho(idx);
      size_t pos = Level[height] + (idx >> (1 + height)) * (BOUNDSIZE + height);
      auint64_t &element = reinterpret_cast<auint64_t &>(Tree[pos >> 3]);

      element += inc << (pos & 0b111);
      idx += mask_rho(idx);
    }
  }

  using FenwickTree::find;
  virtual size_t find(uint64_t *val) const {
    size_t node = 0, idx = 0;

    for (size_t height = Levels - 2; height != SIZE_MAX; height--) {
      size_t pos = Level[height] + idx * (BOUNDSIZE + height);

      idx <<= 1;

      if (pos >= Level[height + 1])
        continue;

      uint64_t element = *reinterpret_cast<auint64_t *>(&Tree[pos >> 3]);
      uint64_t value = bitextract(element, pos & 0b111, BOUNDSIZE + height);

      if (*val >= value) {
        idx++;
        *val -= value;
        node += 1ULL << height;
      }
    }

    return min(node, Size);
  }

  using FenwickTree::compFind;
  virtual size_t compFind(uint64_t *val) const {
    size_t node = 0, idx = 0;

    for (size_t height = Levels - 2; height != SIZE_MAX; height--) {
      size_t pos = Level[height] + idx * (BOUNDSIZE + height);

      idx <<= 1;

      if (pos >= Level[height + 1])
        continue;

      uint64_t element = *reinterpret_cast<auint64_t *>(&Tree[pos >> 3]);
      uint64_t value = (BOUND << height) -
                       bitextract(element, pos & 0b111, BOUNDSIZE + height);

      if (*val >= value) {
        idx++;
        *val -= value;
        node += 1ULL << height;
      }
    }

    return min(node, Size);
  }

  virtual size_t size() const { return Size; }

  virtual size_t bitCount() const {
    return sizeof(BitL<BOUNDSIZE>) * 8 +
           Tree.bitCount() - sizeof(Tree) +
           Levels * sizeof(size_t) * 8;
  }
};

} // namespace hft::fenwick

#endif // __FENWICK_LBIT_HPP__
