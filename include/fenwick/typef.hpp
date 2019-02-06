#ifndef __FENWICK_TYPE_HPP__
#define __FENWICK_TYPE_HPP__

#include "fenwick_tree.hpp"

namespace hft::fenwick {

/**
 * class TypeF - closer type compression and classical node layout.
 * @sequence: sequence of integers.
 * @size: number of elements.
 * @BOUND: maximum value that @sequence can store.
 *
 */
template <size_t BOUND> class TypeF : public FenwickTree {
public:
  static constexpr size_t BOUNDSIZE = log2(BOUND);
  static_assert(BOUNDSIZE >= 1 && BOUNDSIZE <= 64,
                "Leaves can't be stored in a 64-bit word");

protected:
  const size_t Size;
  DArray<uint8_t> Tree;

public:
  TypeF(uint64_t sequence[], size_t size)
      : Size(size), Tree(get_bytepos(size)) {
    for (size_t i = 1; i <= Size; i++) {
      size_t bytepos = get_bytepos(i - 1);

      switch (BOUNDSIZE + rho(i)) {
      case 17 ... 64:
        reinterpret_cast<auint64_t &>(Tree[bytepos]) = sequence[i - 1];
        break;
      case 9 ... 16:
        reinterpret_cast<auint16_t &>(Tree[bytepos]) = sequence[i - 1];
        break;
      default:
        reinterpret_cast<auint8_t &>(Tree[bytepos]) = sequence[i - 1];
      }
    }

    for (size_t m = 2; m <= Size; m <<= 1) {
      for (size_t idx = m; idx <= Size; idx += m) {
        size_t left = get_bytepos(idx - 1);
        size_t right = get_bytepos(idx - m / 2 - 1);

        switch (BOUNDSIZE + rho(idx)) {
        case 17 ... 64:
          switch (BOUNDSIZE + rho(idx - m / 2)) {
          case 17 ... 64:
            reinterpret_cast<auint64_t &>(Tree[left]) +=
                *reinterpret_cast<auint64_t *>(&Tree[right]);
            break;
          case 9 ... 16:
            reinterpret_cast<auint64_t &>(Tree[left]) +=
                *reinterpret_cast<auint16_t *>(&Tree[right]);
            break;
          default:
            reinterpret_cast<auint64_t &>(Tree[left]) +=
                *reinterpret_cast<auint8_t *>(&Tree[right]);
          }
          break;
        case 9 ... 16:
          switch (BOUNDSIZE + rho(idx - m / 2)) {
          case 9 ... 16:
            reinterpret_cast<auint16_t &>(Tree[left]) +=
                *reinterpret_cast<auint16_t *>(&Tree[right]);
            break;
          default:
            reinterpret_cast<auint16_t &>(Tree[left]) +=
                *reinterpret_cast<auint8_t *>(&Tree[right]);
          }
          break;
        default:
          reinterpret_cast<auint8_t &>(Tree[left]) +=
              *reinterpret_cast<auint8_t *>(&Tree[right]);
        }
      }
    }
  }

  virtual uint64_t prefix(size_t idx) const {
    uint64_t sum = 0;

    while (idx != 0) {
      size_t bytepos = get_bytepos(idx - 1);

      switch (BOUNDSIZE + rho(idx)) {
      case 17 ... 64:
        sum += *reinterpret_cast<auint64_t *>(&Tree[bytepos]);
        break;
      case 9 ... 16:
        sum += *reinterpret_cast<auint16_t *>(&Tree[bytepos]);
        break;
      default:
        sum += *reinterpret_cast<auint8_t *>(&Tree[bytepos]);
      }

      idx = clear_rho(idx);
    }

    return sum;
  }

  virtual void add(size_t idx, int64_t inc) {
    while (idx <= Size) {
      size_t bytepos = get_bytepos(idx - 1);

      switch (BOUNDSIZE + rho(idx)) {
      case 17 ... 64:
        *reinterpret_cast<auint64_t *>(&Tree[bytepos]) += inc;
        break;
      case 9 ... 16:
        *reinterpret_cast<auint16_t *>(&Tree[bytepos]) += inc;
        break;
      default:
        *reinterpret_cast<auint8_t *>(&Tree[bytepos]) += inc;
      }

      idx += mask_rho(idx);
    }
  }

  using FenwickTree::find;
  virtual size_t find(uint64_t *val) const {
    size_t node = 0;

    for (size_t m = mask_lambda(Size); m != 0; m >>= 1) {
      if (node + m - 1 >= Size)
        continue;

      size_t bytepos = get_bytepos(node + m - 1);
      int bitlen = BOUNDSIZE + rho(node + m);

      uint64_t value;
      switch (bitlen) {
      case 17 ... 64:
        value = *reinterpret_cast<auint64_t *>(&Tree[bytepos]);
        break;
      case 9 ... 16:
        value = *reinterpret_cast<auint16_t *>(&Tree[bytepos]);
        break;
      default:
        value = *reinterpret_cast<auint8_t *>(&Tree[bytepos]);
      }

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

      size_t bytepos = get_bytepos(node + m - 1);
      int height = rho(node + m);

      uint64_t value = BOUND << height;
      switch (BOUNDSIZE + height) {
      case 17 ... 64:
        value -= *reinterpret_cast<auint64_t *>(&Tree[bytepos]);
        break;
      case 9 ... 16:
        value -= *reinterpret_cast<auint16_t *>(&Tree[bytepos]);
        break;
      default:
        value -= *reinterpret_cast<auint8_t *>(&Tree[bytepos]);
      }

      if (*val >= value) {
        node += m;
        *val -= value;
      }
    }

    return node;
  }

  virtual size_t size() const { return Size; }

  virtual size_t bitCount() const {
    return sizeof(TypeF<BOUNDSIZE>) * 8 + Tree.bitCount() - sizeof(Tree);
  }

private:
  inline size_t get_bytepos(size_t n) const {
    return n + (n >> (BOUNDSIZE <= 8 ? (8 - BOUNDSIZE + 1) : 0)) +
           (n >> (BOUNDSIZE <= 16 ? (16 - BOUNDSIZE + 1) : 0)) * 6;
  }
};

} // namespace hft::fenwick

#endif // __FENWICK_TYPE_HPP__
