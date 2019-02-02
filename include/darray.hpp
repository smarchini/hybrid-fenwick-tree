#ifndef __DARRAY_HPP__
#define __DARRAY_HPP__

#include "common.hpp"
#include <assert.h>
#include <sys/mman.h>

namespace hft {

/**
 * class DArray - Dinamically-allocated fixed-sized array
 *
 * This class is a wrapper of mmap. It supports hugepages [1], transparent
 * hugepags [2]. It guarantees to return page-aligned pointers.
 *
 * [1] https://www.kernel.org/doc/html/latest/admin-guide/mm/hugetlbpage.html
 * [2] https://www.kernel.org/doc/html/latest/admin-guide/mm/transhuge.html
 *
 */
template <typename T> class DArray {
public:
  static constexpr int PROT = PROT_READ | PROT_WRITE;
  static constexpr int FLAGS = MAP_PRIVATE | MAP_ANONYMOUS;

private:
  size_t Size = 0;  // length of the array
  size_t Space = 0; // allocated space in memory
  T *Buffer = nullptr;

public:
  DArray<T>() = default;

  explicit DArray<T>(size_t length) : DArray<T>(length, PageKind::Default) {}

  explicit DArray<T>(size_t length, PageKind page)
      : Size(length),
        Space(((pagesize(page) - 1) | (Size * sizeof(T) - 1)) + 1) {
    if (Size) {
      void *mem = page == PageKind::Huge
                      ? mmap(nullptr, Space, PROT, FLAGS | MAP_HUGETLB, -1, 0)
                      : mmap(nullptr, Space, PROT, FLAGS, -1, 0);
      assert(mem != MAP_FAILED && "mmap failed");

      if (page == PageKind::Transparent) {
        int adv = madvise(mem, Space, MADV_HUGEPAGE);
        assert(adv == 0 && "madvise failed");
      }

      Buffer = static_cast<T *>(mem);
    }
  }

  DArray(DArray<T> &&oth)
      : Size(std::exchange(oth.Size, 0)), Space(std::exchange(oth.Space, 0)),
        Buffer(std::exchange(oth.Buffer, nullptr)) {}

  DArray<T> &operator=(DArray<T> &&oth) {
    swap(*this, oth);
    return *this;
  }

  ~DArray<T>() {
    if (Buffer != nullptr) {
      int result = munmap(Buffer, Space);
      assert(result == 0 && "mmunmap failed");
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

#endif // __DARRAY_HPP__
