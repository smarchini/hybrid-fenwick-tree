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
  static constexpr size_t BOUNDSIZE = ceil_log2_plus1(BOUND);
  static constexpr size_t STARTING_OFFSET = 1;
  static constexpr size_t END_PADDING = 56;
  static_assert(BOUNDSIZE >= 1 && BOUNDSIZE <= 55,
                "Some nodes will span on multiple words");

protected:
  const size_t Size;
  DArray<uint8_t> Tree;

public:
  BitF(uint64_t sequence[], size_t size)
      : Size(size), Tree((first_bit_after(size) + END_PADDING + 7) >> 3) {

    for (size_t idx = 1; idx <= size; idx++)
      add_to_partial_frequency(idx, sequence[idx - 1]);

    for (size_t m = 2; m <= size; m <<= 1)
      for (size_t idx = m; idx <= size; idx += m) 
	add_to_partial_frequency(idx, get_partial_frequency(idx - m/2));
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
      add_to_partial_frequency(idx, inc);
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

      const int height = rho(node + m);
      uint64_t value = (BOUND << height) - get_partial_frequency(node + m);

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
  inline size_t first_bit_after(size_t j) const {
    return (BOUNDSIZE + 1) * j - popcount(j) + STARTING_OFFSET + (j / (64*1024)) * 64;
  }

  inline uint64_t get_partial_frequency(size_t j) const {
      const uint64_t mask = (UINT64_C(1) << (BOUNDSIZE + rho(j))) - 1;
      j--;
      const uint64_t prod = (BOUNDSIZE + 1) * j;
      const size_t pos = prod + STARTING_OFFSET - popcount(j) + (j / (64*1024)) * 64;

      return (prod + (BOUNDSIZE + 1)) % 64 == 0 ?
         (*(reinterpret_cast<auint64_t *>(&Tree[0]) + pos / 64) >> (pos % 64)) & mask :
         (*(reinterpret_cast<auint64_t *>(&Tree[pos / 8])) >> (pos % 8)) & mask;
  }

  inline uint64_t add_to_partial_frequency(size_t j, uint64_t value) const {
      j--;
      const size_t prod = (BOUNDSIZE + 1) * j;
      const size_t pos = prod + STARTING_OFFSET - popcount(j) + (j / (64*1024)) * 64;

      return (prod + (BOUNDSIZE + 1)) % 64 == 0 ?
         *(reinterpret_cast<auint64_t *>(&Tree[0]) + pos / 64) += value << (pos % 64) :
         *reinterpret_cast<auint64_t *>(&Tree[pos / 8]) += value << (pos % 8);
  }
};

} // namespace _hft::fenwick

#endif // __FENWICK_BIT_HPP__
