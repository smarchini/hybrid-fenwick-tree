#ifndef __UNIQUEPTR_HPP__
#define __UNIQUEPTR_HPP__

#include <memory>
#include <algorithm>

namespace hft {

    /**
     * class DArray - Dinamically-allocated fixed-sized array
     *
     */
    template <typename T>
    class DArray
    {
    private:
        size_t _size = 0;
        std::unique_ptr<T[]> buffer;

    public:
        // constructors
        DArray<T>() : DArray<T>(0) { };
        explicit DArray(size_t size):
            _size(size),
            buffer(std::make_unique<T[]>(size))
        { }

        // move constructor
        DArray(DArray<T>&& oth):
            _size(std::exchange(oth._size, 0)),
            buffer(std::move(oth.buffer)) { }

        // move assignment (copy&swap idiom)
        DArray& operator=(DArray oth)
        {
            swap(*this, oth);
            return *this;
        }

        // destructor
        ~DArray() = default;

        // swap
        friend void swap(DArray& first, DArray& second) noexcept
        {
            using std::swap;
            swap(first._size, second._size);
            swap(first.buffer, second.buffer);
        }

        // data access capabilities
        inline T*  get() const { return buffer.get(); }
        inline T& operator[](size_t i) const { return buffer[i]; };
        inline size_t size() const { return _size; }

        size_t bit_count() const
        {
            return sizeof(DArray<T>)*8 + sizeof(T)*_size*8;
        }
    };

}

#endif // __UNIQUEPTR_HPP__
