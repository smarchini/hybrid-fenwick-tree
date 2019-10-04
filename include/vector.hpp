#ifndef __VECTOR_HPP__
#define __VECTOR_HPP__

#include "common.hpp"
#include <assert.h>
#include <iostream>
#include <sys/mman.h>

namespace hft {

enum PageType { TRANSHUGE, SMALLPAGE, HUGETLBPAGE };

template <typename T, PageType PT = TRANSHUGE> class Vector {

public:
  static constexpr int PROT = PROT_READ | PROT_WRITE;
  static constexpr int FLAGS = MAP_PRIVATE | MAP_ANONYMOUS | (PT == HUGETLBPAGE ? MAP_HUGETLB : 0);

private:
  size_t Capacity = 0, Size = 0;
  T *Data = nullptr;

public:
  Vector<T, PT>() = default;

  explicit Vector<T, PT>(size_t size) : Size(size), Capacity(size) {
    size_t bytes = page_aligned(Size);

    if (bytes) {
      void *mem = mmap(nullptr, bytes, PROT, FLAGS, -1, 0);
      assert(mem != MAP_FAILED && "mmap failed");

      if (PT == TRANSHUGE) {
        int adv = madvise(mem, bytes, MADV_HUGEPAGE);
        assert(adv == 0 && "madvise failed");
      }

      Data = static_cast<T *>(mem);
    }
  }

  ~Vector<T, PT>() {
    if (Data) {
      int result = munmap(Data, page_aligned(Size));
      assert(result == 0 && "mmunmap failed");
    }
  }

  void reserve(size_t space) {
    if (space > Capacity)
      remap(space);
  }

  void shrink(size_t space) {
    if (space < Capacity)
      remap(space);
  }

  // TODO: perfect forwarding or something?
  void pushBack(T elem) {
    if (++Size > Capacity)
      reserve(Capacity * 2);

    Data[Size] = elem;
  }

  T popBack() { return Data[Size--]; }

  inline T *get() const { return Data; }

  inline T &operator[](size_t i) const { return Data[i]; };

  inline size_t size() const { return Size; }

  inline size_t capacity() const { return Capacity; }

  size_t bitCount() const { return sizeof(Vector<T, PT>) * 8 + page_aligned(Capacity) * 8; }

private:
  static size_t page_aligned(size_t size) {
    if (PT == HUGETLBPAGE)
      return ((2 * 1024 * 1024 - 1) | (size * sizeof(T) - 1)) + 1;
    else
      return ((4 * 1024 - 1) | (size * sizeof(T) - 1)) + 1;
  }

  size_t remap(size_t space) {
    void *mem = mremap(Data, page_aligned(Capacity), space * sizeof(T), FLAGS, -1, 0);
    assert(mem != MAP_FAILED && "mremap failed");
    Capacity = space;
  }

  friend std::ostream &operator<<(std::ostream &os, const Vector<T, PT> &vector) {
    const uint64_t nsize = hton(static_cast<uint64_t>(vector.Size));
    os.write((char *)&nsize, sizeof(uint64_t));

    for (size_t i = 0; i < vector.Size; ++i) {
      const T value = hton(vector[i]);
      os.write((char *)&value, sizeof(T));
    }

    return os;
  }

  friend std::istream &operator>>(std::istream &is, Vector<T, PT> &vector) {
    uint64_t nsize;
    is.read((char *)&nsize, sizeof(uint64_t));

    vector = Vector<T, PT>(ntoh(nsize));

    for (size_t i = 0; i < vector.size(); ++i) {
      is.read((char *)&vector[i], sizeof(T));
      vector[i] = ntoh(vector[i]);
    }

    return is;
  }
};

} // namespace hft

#endif // __VECTOR_HPP__
