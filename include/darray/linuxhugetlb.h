#ifndef __LINUXHUGETLB_HPP__
#define __LINUXHUGETLB_HPP__

#include <algorithm>
#include <assert.h>
#include <memory>
#include <sys/mman.h>
#include <iostream>

#include "../common.hpp"

namespace hft {

/**
 * class DArray - Dinamically-allocated fixed-sized array
 *
 * This class is a wrapper for mmap to take advantage of hugepages [1].
 *
 * [1] https://www.kernel.org/doc/html/latest/admin-guide/mm/hugetlbpage.html
 *
 */
template <typename T> class DArray {
public:
  static constexpr int PROT = PROT_READ | PROT_WRITE;
  static constexpr int FLAGS = MAP_PRIVATE | MAP_ANONYMOUS;

private:
  size_t Size;  // length of the array
  size_t Space; // allocated space in memory
  T *Buffer = nullptr;

public:
  DArray<T>() : DArray<T>(0){};

  explicit DArray<T>(size_t length) : Size(length) {
    size_t minlen = length > 0 ? length : 1;

#ifdef HFT_TRANSPARENT
    Space = ((4096 - 1) | (minlen * sizeof(T) - 1)) + 1;
    void *memory = mmap(nullptr, Space, PROT, FLAGS, -1, 0);
    assert(memory != MAP_FAILED && "mmap failed");
    int advised = madvise(memory, Space, MADV_HUGEPAGE);
    assert(advised == 0 && "madvise failed");
#else
    Space = ((2097152 - 1) | (minlen * sizeof(T) - 1)) + 1;
    void *memory = mmap(nullptr, Space, PROT, FLAGS | MAP_HUGETLB, -1, 0);
    assert(memory != MAP_FAILED && "mmap failed");
#endif

    Buffer = static_cast<T *>(memory);
  }

  DArray(DArray<T> &&oth)
      : Size(std::exchange(oth.Size, 0)), Space(std::exchange(oth.Space, 0)),
        Buffer(std::exchange(oth.Buffer, nullptr)) {}

  DArray<T> &operator=(DArray<T> &&oth) {
    swap(*this, oth);
    return *this;
  }

  ~DArray<T>() {
    int result = munmap(Buffer, Space);
    assert(result == 0 && "mmunmap failed");
  }

  friend void swap(DArray<T> &first, DArray<T> &second) noexcept {
    std::swap(first.Size, second.Size);
    std::swap(first.Space, second.Space);
    std::swap(first.Buffer, second.Buffer);
  }

  inline T *get() const { return Buffer; }

  inline T &operator[](size_t i) const { return Buffer[i]; };

  inline size_t size() const { return Size; }

  size_t bitCount() const { return sizeof(DArray<T>) * 8 + Space * 8; }
};

} // namespace hft

#endif // __LINUXHUGETLB_HPP__
