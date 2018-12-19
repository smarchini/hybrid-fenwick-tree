#ifndef __UNIQUEPTR_HPP__
#define __UNIQUEPTR_HPP__

#include <algorithm>
#include <memory>

namespace hft {

/**
 * class DArray - Dinamically-allocated fixed-sized array
 *
 */
template <typename T> class DArray {
private:
  size_t Size = 0;
  std::unique_ptr<T[]> Buffer;

public:
  DArray<T>() : DArray<T>(0){};
  explicit DArray(size_t size)
      : Size(size), Buffer(std::make_unique<T[]>(size)) {}

  DArray(DArray<T> &&oth)
      : Size(std::exchange(oth.Size, 0)), Buffer(std::move(oth.Buffer)) {}

  DArray &operator=(DArray oth) {
    swap(*this, oth);
    return *this;
  }

  ~DArray() = default;

  friend void swap(DArray &first, DArray &second) noexcept {
    std::swap(first.Size, second.Size);
    std::swap(first.Buffer, second.Buffer);
  }

  inline T *get() const { return Buffer.get(); }

  inline T &operator[](size_t i) const { return Buffer[i]; };

  inline size_t size() const { return Size; }

  size_t bitCount() const {
    return sizeof(DArray<T>) * 8 + Size * sizeof(T) * 8;
  }
};

} // namespace hft

#endif // __UNIQUEPTR_HPP__
