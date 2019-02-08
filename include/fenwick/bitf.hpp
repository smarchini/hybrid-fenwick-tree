#ifndef __FENWICK_BIT_HPP__
#define __FENWICK_BIT_HPP__

#include "fenwick_tree.hpp"

namespace hft::fenwick {

/**
 * class BitF - bit compression and classical node layout.
 * @sequence: sequence of integers.
 * @size: number of elements.
 * @BOUND: maximum value that @sequence can store.
 *
 */
template <size_t BOUND> class BitF : public FenwickTree {
public:
  static constexpr size_t BOUNDSIZE = log2(BOUND);
  static constexpr size_t STARTING_OFFSET = 65;
  static_assert(BOUNDSIZE >= 1 && BOUNDSIZE <= 55,
                "Some nodes will span on multiple words");

protected:
  DArray<uint8_t> Tree;
  auint64_t &Size; // firsts 64 bits of Tree

public:
  BitF(uint64_t sequence[], size_t size)
      : Tree((bitpos(size) >> 3) + 8), // +8 for safety
        Size(reinterpret_cast<auint64_t &>(Tree[0])) {
    Size = size;

    for (size_t i = 1; i <= size; i++) {
      size_t pos = bitpos(i - 1);
      auint64_t &element = reinterpret_cast<auint64_t &>(Tree[pos >> 3]);

      int isize = BOUNDSIZE + rho(i);
      size_t shift = pos & 0b111;
      uint64_t mask = ((1ULL << isize) - 1) << shift;

      element &= ~mask;
      element |= mask & (sequence[i - 1] << shift);
    }

    for (size_t m = 2; m <= size; m <<= 1) {
      for (size_t idx = m; idx <= size; idx += m) {
        size_t pos = bitpos(idx - 1);
        auint64_t &element = reinterpret_cast<auint64_t &>(Tree[pos >> 3]);

        size_t subpos = bitpos(idx - m / 2 - 1);
        uint64_t subelem = *reinterpret_cast<auint64_t *>(&Tree[subpos >> 3]);
        uint64_t value = bitextract(subelem, subpos & 0b111, BOUNDSIZE + rho(idx - m / 2));

        element += value << (pos & 0b111);
      }
    }
  }

  virtual uint64_t prefix(size_t idx) const {
    uint64_t sum = 0;

    while (idx != 0) {
      size_t end = endbitpos(idx);
      uint64_t element = *reinterpret_cast<auint64_t *>(&Tree[(end - 56) >> 3]);
      size_t length = BOUNDSIZE + rho(idx);

      sum += element << (7 - (end & 0b111)) >> (64 - length);
      idx = clear_rho(idx);
    }

    return sum;
  }

  virtual void add(size_t idx, int64_t inc) {
    while (idx <= Size) {
      size_t end = endbitpos(idx);
      auint64_t &element = reinterpret_cast<auint64_t &>(Tree[(end - 56) >> 3]);

      element += inc << (64 - BOUNDSIZE - rho(idx) - 7 + (end & 0b111));
      idx += mask_rho(idx);
    }
  }

  using FenwickTree::find;
  virtual size_t find(uint64_t *val) const {
    size_t node = 0;

    for (size_t m = mask_lambda(Size); m != 0; m >>= 1) {
      if (node + m - 1 >= Size)
        continue;

      size_t end = endbitpos(node + m);
      size_t offset = 7 - (end & 0b111);
      uint64_t element = *reinterpret_cast<auint64_t *>(&Tree[(end - 56) >> 3]);

      uint64_t value = (element << offset) >> (64 - BOUNDSIZE - rho(node + m));

      if (*val >= value) {
        node += m;
        *val -= value;
      }
    }

    return node;
  }

  using FenwickTree::compFind;
  virtual size_t compFind(uint64_t *val) const {
    size_t node = 0;

    for (size_t m = mask_lambda(Size); m != 0; m >>= 1) {
      if (node + m - 1 >= Size)
        continue;

      int height = rho(node + m);
      size_t end = endbitpos(node + m);
      size_t offset = 7 - (end & 0b111);
      uint64_t element = *reinterpret_cast<auint64_t *>(&Tree[(end - 56) >> 3]);

      uint64_t value = (BOUND << height) -
                       ((element << offset) >> (64 - BOUNDSIZE - height));

      if (*val >= value) {
        node += m;
        *val -= value;
      }
    }

    return node;
  }

  virtual size_t size() const { return Size; }

  virtual size_t bitCount() const {
    return sizeof(BitF<BOUNDSIZE>) * 8 +
           Tree.bitCount() - sizeof(Tree);
  }

private:
  inline size_t bitpos(size_t n) const {
    return (BOUNDSIZE + 1) * n - popcount(n) + STARTING_OFFSET;
  }

  inline size_t endbitpos(size_t n) const {
    return (BOUNDSIZE + 1) * n + 64 - popcount(n);
  }
};

} // namespace hft::fenwick

#endif // __FENWICK_BIT_HPP__
