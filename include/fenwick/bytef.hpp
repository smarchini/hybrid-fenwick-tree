#ifndef __FENWICK_BYTE_HPP__
#define __FENWICK_BYTE_HPP__

#include "fenwick_tree.hpp"

namespace hft::fenwick {

/**
 * class ByteF - byte compression and classical node layout.
 * @sequence: sequence of integers.
 * @size: number of elements.
 * @BOUND: maximum value that @sequence can store.
 *
 */
template <size_t BOUND> class ByteF : public FenwickTree {
public:
  static constexpr size_t BOUNDSIZE = log2(BOUND);
  static_assert(BOUNDSIZE >= 1 && BOUNDSIZE <= 64,
                "Leaves can't be stored in a 64-bit word");

protected:
  const size_t Size;
  DArray<uint8_t> Tree;

  std::array<uint64_t, 4096> addrprefix, addradd, addrfind;

public:
  ByteF(uint64_t sequence[], size_t size)
      : Size(size), Tree(bytepos(size) + 8) {
    for (size_t i = 1; i <= size; i++) {
      auint64_t &element = reinterpret_cast<auint64_t &>(Tree[bytepos(i - 1)]);

      size_t isize = bytesize(i);
      element &= ~BYTE_MASK[isize];
      element |= sequence[i - 1] & BYTE_MASK[isize];
    }

    for (size_t m = 2; m <= size; m <<= 1) {
      for (size_t idx = m; idx <= size; idx += m) {
        auint64_t &left = reinterpret_cast<auint64_t &>(Tree[bytepos(idx - 1)]);
        auint64_t right = *reinterpret_cast<auint64_t *>(&Tree[bytepos(idx - m / 2 - 1)]);

        uint64_t value = right & BYTE_MASK[bytesize(idx - m / 2)];
        left += value;
      }
    }
  }

  virtual uint64_t prefix(size_t idx) {
    uint64_t sum = 0;

    while (idx != 0) {
      uint64_t element = *reinterpret_cast<auint64_t *>(&Tree[bytepos(idx - 1)]);
      addrprefix[(uint64_t)(&Tree[bytepos(idx - 1)]) % 4096]++;

      sum += element & BYTE_MASK[bytesize(idx)];
      idx = clear_rho(idx);
    }

    return sum;
  }

  virtual void add(size_t idx, int64_t inc) {
    while (idx <= Size) {
      auint64_t &element = reinterpret_cast<auint64_t &>(Tree[bytepos(idx - 1)]);
      addradd[(uint64_t)&element % 4096]++;

      element += inc;
      idx += mask_rho(idx);
    }
  }

  using FenwickTree::find;
  virtual size_t find(uint64_t *val) {
    size_t node = 0;

    for (size_t m = mask_lambda(Size); m != 0; m >>= 1) {
      if (node + m - 1 >= Size)
        continue;

      uint64_t value =
          *reinterpret_cast<auint64_t *>(&Tree[bytepos(node + m - 1)]) &
          BYTE_MASK[bytesize(node + m)];
      addrfind[(uint64_t)(&Tree[bytepos(node + m - 1)]) % 4096]++;

      if (*val >= value) {
        node += m;
        *val -= value;
      }
    }

    return node;
  }

  using FenwickTree::compFind;
  virtual size_t compFind(uint64_t *val) {
    size_t node = 0;

    for (size_t m = mask_lambda(Size); m != 0; m >>= 1) {
      if (node + m - 1 >= Size)
        continue;

      uint64_t value =
          (BOUND << rho(node + m)) -
          (*reinterpret_cast<auint64_t *>(&Tree[bytepos(node + m - 1)]) &
           BYTE_MASK[bytesize(node + m)]);

      if (*val >= value) {
        node += m;
        *val -= value;
      }
    }

    return node;
  }

  virtual size_t size() { return Size; }

  virtual size_t bitCount() {
    return sizeof(ByteF<BOUNDSIZE>) * 8 + Tree.bitCount() - sizeof(Tree);
  }

  ~ByteF() {
    std::ofstream fprefix(std::string("address_ByteF_prefix_") + STRINGIFY(MAGIC) + ".txt", std::ofstream::app);
    std::ofstream fadd(std::string("address_ByteF_add_") + STRINGIFY(MAGIC) + ".txt", std::ofstream::app);
    std::ofstream ffind(std::string("address_ByteF_find_") + STRINGIFY(MAGIC) + ".txt", std::ofstream::app);

    for (auto i: addrprefix) fprefix << i << "\n";
    for (auto i: addradd) fadd << i << "\n";
    for (auto i: addrfind) ffind << i << "\n";
  }

private:
  static inline size_t bytesize(size_t idx) {
    return ((rho(idx) + BOUNDSIZE - 1) >> 3) + 1;
  }

  static inline size_t bytepos(size_t idx) {
    constexpr size_t NEXTBYTE = ((BOUNDSIZE - 1) | (8 - 1)) + 1;

    constexpr size_t SMALL = ((BOUNDSIZE - 1) >> 3) + 1;
    constexpr size_t MEDIUM = NEXTBYTE - BOUNDSIZE + 1;
    constexpr size_t LARGE = MEDIUM + 8;

    constexpr size_t MULTIPLIER = 8 - SMALL - 1;

    return idx * SMALL + (idx >> MEDIUM) + (idx >> LARGE) * MULTIPLIER;
  }
};

} // namespace hft::fenwick

#endif // __FENWICK_BYTE_HPP__
