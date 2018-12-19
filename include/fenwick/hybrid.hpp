#ifndef __FENWICK_HYBRID_HPP__
#define __FENWICK_HYBRID_HPP__

#include "fenwick_tree.hpp"
#include <vector>

namespace hft::fenwick {

template <template <size_t> class TOP, template <size_t> class BOTTOM,
          size_t BOUND, size_t CUTPOINT>
class Hybrid : public FenwickTree {
private:
  static constexpr size_t BOTTOMSIZE = (1ULL << CUTPOINT) - 1;
  static constexpr size_t TOPBOUND = BOUND * (BOTTOMSIZE + 1);

protected:
  const size_t Size;
  std::vector<BOTTOM<BOUND>> BottomForest;
  TOP<TOPBOUND> TopFenwick;

public:
  Hybrid(uint64_t sequence[], size_t size)
      : Size(size), TopFenwick(buildTop(sequence, size)) {
    for (size_t i = 0; i <= TopFenwick.size(); i++) {
      size_t next = (BOTTOMSIZE + 1) * i;
      size_t bsize = BOTTOMSIZE + next <= size ? BOTTOMSIZE : size & BOTTOMSIZE;

      BottomForest.push_back(BOTTOM<BOUND>(sequence + next, bsize));
    }

    for (size_t i = 0; i < TopFenwick.size(); i++)
      TopFenwick.add(i + 1, BottomForest[i].prefix(BottomForest[i].size()));
  }

  virtual uint64_t prefix(size_t idx) const {
    size_t top = idx >> CUTPOINT;
    size_t bottom = idx & BOTTOMSIZE;

    return TopFenwick.prefix(top) + BottomForest[top].prefix(bottom);
  }

  virtual void add(size_t idx, int64_t inc) {
    size_t top = idx >> CUTPOINT;
    size_t bottom = idx & BOTTOMSIZE;

    if (bottom == 0) {
      TopFenwick.add(top, inc);
    } else {
      TopFenwick.add(top + 1, inc);
      BottomForest[top].add(bottom, inc);
    }
  }

  using FenwickTree::find;
  virtual size_t find(uint64_t *val) const {
    size_t top = TopFenwick.size() != 0 ? TopFenwick.find(val) : 0;
    size_t bottom = top < BottomForest.size() ? BottomForest[top].find(val) : 0;

    return (top << CUTPOINT) + bottom;
  }

  using FenwickTree::compFind;
  virtual size_t compFind(uint64_t *val) const {
    size_t top = TopFenwick.size() != 0 ? TopFenwick.compFind(val) : 0;
    size_t bottom = top < BottomForest.size() ? BottomForest[top].compFind(val) : 0;

    return (top << CUTPOINT) + bottom;
  }

  virtual size_t size() const { return Size; }

  virtual size_t bitCount() const {
    size_t bottomSize = 0;
    for (auto &t : BottomForest)
      bottomSize += t.bitCount();

    return bottomSize + sizeof(Hybrid<BOTTOM, TOP, BOUND, CUTPOINT>) +
           TopFenwick.bitCount();
  }

private:
  static TOP<TOPBOUND> buildTop(const uint64_t sequence[], size_t size) {
    size_t topsize = size >> CUTPOINT;
    std::unique_ptr<uint64_t[]> subseq = std::make_unique<uint64_t[]>(topsize);

    for (size_t i = 1; i <= topsize; i++)
      subseq[i - 1] = sequence[i * (BOTTOMSIZE + 1) - 1];

    return TOP<TOPBOUND>(subseq.get(), topsize);
  }
};

} // namespace hft::fenwick

#endif // __FENWICK_HYBRID_HPP__
