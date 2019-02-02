#ifndef __FENWICK_LBYTE_HPP__
#define __FENWICK_LBYTE_HPP__

#include "fenwick_tree.hpp"

namespace hft::fenwick {

/**
 * class ByteL - byte compression and level ordered node layout.
 * @sequence: sequence of integers.
 * @size: number of elements.
 * @BOUND: maximum value that @sequence can store.
 *
 */
template <size_t BOUND> class ByteL : public FenwickTree {
public:
  static constexpr size_t BOUNDSIZE = log2(BOUND);
  static_assert(BOUNDSIZE >= 1 && BOUNDSIZE <= 64,
                "Leaves can't be stored in a 64-bit word");

protected:
  const size_t Size, Levels;
  DArray<uint8_t> Tree;
  unique_ptr<size_t[]> Level;

public:
  ByteL(uint64_t sequence[], size_t size)
      : ByteL(sequence, size, PageKind::Default) {}

  ByteL(uint64_t sequence[], size_t size, PageKind page)
      : Size(size), Levels(lambda(size + 1) + 2),
        Level(make_unique<size_t[]>(Levels)) {
    Level[0] = 0;
    for (size_t i = 1; i < Levels; i++)
      Level[i] = ((size + (1 << (i - 1))) / (1 << i)) * heightsize(i - 1) +
                 Level[i - 1];

    Tree = DArray<uint8_t>(Level[Levels - 1] + 8, page); // +8 for safety

    for (size_t l = 0; l < Levels - 1; l++) {
      for (size_t node = 1 << l; node <= Size; node += 1 << (l + 1)) {
        size_t highpos = Level[l] + heightsize(l) * (node >> (l + 1));
        auint64_t &high = reinterpret_cast<auint64_t &>(Tree[highpos]);

        size_t sequence_idx = node - 1;
        uint64_t value = sequence[sequence_idx];
        for (size_t j = 0; j < l; j++) {
          sequence_idx >>= 1;
          size_t lowpos = Level[j] + heightsize(j) * sequence_idx;
          auint64_t low = *reinterpret_cast<auint64_t *>(&Tree[lowpos]);

          value += low & BYTE_MASK[heightsize(j)];
        }

        high &= ~BYTE_MASK[heightsize(l)];
        high |= value & BYTE_MASK[heightsize(l)];
      }
    }
  }

  virtual uint64_t prefix(size_t idx) const {
    uint64_t sum = 0;

    while (idx != 0) {
      int height = rho(idx);
      size_t isize = heightsize(height);

      auint64_t &element = reinterpret_cast<auint64_t &>(
          Tree[Level[height] + (idx >> (1 + height)) * isize]);

      sum += element & BYTE_MASK[isize];
      idx = clear_rho(idx);
    }

    return sum;
  }

  virtual void add(size_t idx, int64_t inc) {
    while (idx <= Size) {
      int height = rho(idx);

      auint64_t &element = reinterpret_cast<auint64_t &>(
          Tree[Level[height] + (idx >> (1 + height)) * heightsize(height)]);

      element += inc;
      idx += mask_rho(idx);
    }
  }

  using FenwickTree::find;
  virtual size_t find(uint64_t *val) const {
    size_t node = 0, idx = 0;

    for (size_t height = Levels - 2; height != SIZE_MAX; height--) {
      size_t isize = heightsize(height);
      size_t pos = Level[height] + idx * heightsize(height);

      idx <<= 1;

      if (pos >= Level[height + 1])
        continue;

      uint64_t element = *reinterpret_cast<auint64_t *>(&Tree[pos]);
      uint64_t value = element & BYTE_MASK[isize];

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
      size_t isize = heightsize(height);
      size_t pos = Level[height] + idx * heightsize(height);

      idx <<= 1;

      if (pos >= Level[height + 1])
        continue;

      uint64_t element = *reinterpret_cast<auint64_t *>(&Tree[pos]);
      uint64_t value = (BOUND << height) - (element & BYTE_MASK[isize]);

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
    return sizeof(ByteL<BOUNDSIZE>) * 8 +
           Tree.bitCount() - sizeof(Tree) +
           Levels * sizeof(size_t) * 8;
  }

private:
  static inline size_t heightsize(size_t height) {
    return ((height + BOUNDSIZE - 1) >> 3) + 1;
  }
};

} // namespace hft::fenwick

#endif // __FENWICK_LBYTE_HPP__
