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
  static_assert(BOUNDSIZE >= 1 && BOUNDSIZE <= 64,
                "Leaves can't be stored in a 64-bit word");

protected:
  const size_t Size;
  DArray<uint8_t> Tree;

  std::array<uint64_t, 4096> addrprefix, addradd, addrfind;

public:
  BitF(uint64_t sequence[], size_t size)
      : Size(size), Tree((bitpos(size) >> 3) + 8) // +8 for safety
  {
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

  virtual uint64_t prefix(size_t idx) {
    uint64_t sum = 0;

    while (idx != 0) {
      size_t pos = bitpos(idx - 1);
      uint64_t element = *reinterpret_cast<auint64_t *>(&Tree[pos >> 3]);
      addrprefix[(uint64_t)&element % 4096]++;

      sum += bitextract(element, pos & 0b111, BOUNDSIZE + rho(idx));
      idx = clear_rho(idx);
    }

    return sum;
  }

  virtual void add(size_t idx, int64_t inc) {
    while (idx <= Size) {
      size_t pos = bitpos(idx - 1);
      auint64_t &element = reinterpret_cast<auint64_t &>(Tree[pos >> 3]);
      addradd[(uint64_t)&element % 4096]++;

      element += inc << (pos & 0b111);
      idx += mask_rho(idx);
    }
  }

  using FenwickTree::find;
  virtual size_t find(uint64_t *val) {
    size_t node = 0;

    for (size_t m = mask_lambda(Size); m != 0; m >>= 1) {
      if (node + m - 1 >= Size)
        continue;

      size_t pos = bitpos(node + m - 1);
      uint64_t element = *reinterpret_cast<auint64_t *>(&Tree[pos >> 3]);
      addrfind[(uint64_t)&element % 4096]++;

      uint64_t value =
          bitextract(element, pos & 0b111, BOUNDSIZE + rho(node + m));

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

      int height = rho(node + m);
      size_t pos = bitpos(node + m - 1);
      uint64_t element = *reinterpret_cast<auint64_t *>(&Tree[pos >> 3]);

      uint64_t value = (BOUND << height) -
                       bitextract(element, pos & 0b111, BOUNDSIZE + height);

      if (*val >= value) {
        node += m;
        *val -= value;
      }
    }

    return node;
  }

  virtual size_t size() { return Size; }

  virtual size_t bitCount() {
    return sizeof(BitF<BOUNDSIZE>) * 8 +
           Tree.bitCount() - sizeof(Tree);
  }

  ~BitF() {
    using namespace std;

    if (any_of(begin(addrprefix), end(addrprefix), [](int i) { return i != 0; })) {
      ofstream fprefix(std::string("address_BitF_prefix_") + STRINGIFY(MAGIC) + ".txt");
      for (auto i: addrprefix) fprefix << i << "\n";
    }

    if (any_of(begin(addradd), end(addradd), [](int i) { return i != 0; })) {
      ofstream fadd(std::string("address_BitF_add_") + STRINGIFY(MAGIC) + ".txt");
      for (auto i: addradd) fadd << i << "\n";
    }

    if (any_of(begin(addrfind), end(addrfind), [](int i) { return i != 0; })) {
      ofstream ffind(std::string("address_BitF_find_") + STRINGIFY(MAGIC) + ".txt");
      for (auto i: addrfind) ffind << i << "\n";
    }
  }

private:
  inline size_t bitpos(size_t n) const {
    return (BOUNDSIZE + 1) * n - popcount(n);
  }
};

} // namespace hft::fenwick

#endif // __FENWICK_BIT_HPP__
