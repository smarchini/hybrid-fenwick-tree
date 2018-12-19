#ifndef __LINUXHUGETLB_HPP__
#define __LINUXHUGETLB_HPP__

#include <algorithm>
#include <assert.h>
#include <memory>
#include <sys/mman.h>

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
  static constexpr size_t PAGESIZE = 2048 * 1024;
  static constexpr int PROT = PROT_READ | PROT_WRITE;
  static constexpr int FLAGS = MAP_PRIVATE | MAP_ANONYMOUS;

private:
  size_t Size;
  size_t Space; // multiple of PAGESIZE
  T *Buffer = nullptr;

public:
  DArray<T>() : DArray<T>(0){};

  explicit DArray<T>(size_t length) : Size(length) {
    const size_t minlen = length > 0 ? length : 1;
    Space = ((PAGESIZE - 1) | (minlen * sizeof(T) - 1)) + 1;

#ifdef HFT_TRANSPARENT
    void *result = mmap(nullptr, Space, PROT, FLAGS, -1, 0);
    assert(result != MAP_FAILED);
    madvise(result, Space, MADV_HUGEPAGE);
#else
    void *result = mmap(nullptr, Space, PROT, FLAGS | MAP_HUGETLB, -1, 0);
    assert(result != MAP_FAILED);
#endif

    Buffer = static_cast<T *>(result);
  }

  DArray(DArray<T> &&oth)
      : Size(std::exchange(oth.Size, 0)), Space(std::exchange(oth.Space, 0)),
        Buffer(std::exchange(oth.Buffer, nullptr)) {}

  DArray<T> &operator=(DArray<T> &&oth) {
    swap(*this, oth);
    return *this;
  }

  ~DArray<T>() {
    if (Buffer) {
      int result = munmap(Buffer, Space);
      assert(result == 0);
    }
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
