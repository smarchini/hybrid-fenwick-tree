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
  static constexpr size_t STARTING_OFFSET = 1;
  static constexpr size_t END_PADDING = 56;
  static_assert(BOUNDSIZE >= 1 && BOUNDSIZE <= 55,
                "Some nodes will span on multiple words");

protected:
  const size_t Size;
  DArray<uint8_t> Tree;

public:
  BitF(uint64_t sequence[], size_t size)
      : Size(size), Tree((bitpos(size) + END_PADDING + 7) >> 3) {

    for (size_t i = 1; i <= size; i++) {
      size_t pos = bitpos(i);
      auint64_t &element = reinterpret_cast<auint64_t &>(Tree[pos >> 3]);

      int isize = BOUNDSIZE + rho(i);
      size_t shift = pos & 0b111;
      uint64_t mask = ((1ULL << isize) - 1) << shift;

      element &= ~mask;
      element |= mask & (sequence[i - 1] << shift);
    }

    for (size_t m = 2; m <= size; m <<= 1) {
      for (size_t idx = m; idx <= size; idx += m) {
        size_t pos = bitpos(idx);
        auint64_t &element = reinterpret_cast<auint64_t &>(Tree[pos >> 3]);

        size_t subpos = bitpos(idx - m / 2);
        uint64_t subelem = *reinterpret_cast<auint64_t *>(&Tree[subpos >> 3]);
        uint64_t value = bitextract(subelem, subpos & 0b111, BOUNDSIZE + rho(idx - m / 2));

        element += value << (pos & 0b111);
      }
    }
  }

  virtual uint64_t prefix(size_t idx) const {
    uint64_t sum = 0;

    while (idx != 0) {
      sum += get_partial_frequency(idx);
      idx = clear_rho(idx);
    }

    return sum;
  }

  virtual void add(size_t idx, int64_t inc) {
    while (idx <= Size) {
      size_t pos = bitpos(idx);
      auint64_t &element = reinterpret_cast<auint64_t &>(Tree[pos >> 3]);

      element += inc << (pos & 0b111);
      idx += mask_rho(idx);
    }
  }

  using FenwickTree::find;
  virtual size_t find(uint64_t *val) const {
    size_t node = 0;

    for (size_t m = mask_lambda(Size); m != 0; m >>= 1) {
      if (node + m > Size) continue;

      uint64_t value = get_partial_frequency(node + m);

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
      if (node + m > Size) continue;

      uint64_t value = get_partial_frequency(node + m);

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
  inline size_t bitpos(size_t j) const {
    j--;
    return (BOUNDSIZE + 1) * j - popcount(j) + STARTING_OFFSET;
  }

  inline uint64_t get_partial_frequency(size_t j) const {
      const uint64_t prod = (BOUNDSIZE + 1) * j;
      const uint64_t mask = (UINT64_C(1) << BOUNDSIZE + rho(j)) - 1;
      if (prod % 64 == 0) {
         const uint64_t word = *(reinterpret_cast<auint64_t *>(&Tree[prod >> 3]) - 1);
         return (word >> popcount(j) - 1) & mask;
      }
      else {
         const size_t pos = prod + STARTING_OFFSET - (BOUNDSIZE + 1) - popcount(j - 1);
         const uint64_t word = *reinterpret_cast<auint64_t *>(&Tree[pos >> 3]);
         return word >> (pos & 7) & mask;
      }
  }
};

} // namespace hft::fenwick

#endif // __FENWICK_BIT_HPP__
