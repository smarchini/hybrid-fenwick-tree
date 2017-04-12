#ifndef __DARRAY_H__
#define __DARRAY_H__

#include <memory>
#include <algorithm>

namespace dyn {

    /**
     * class DArray - Deep-copyable & dinamically-allocated fixed-sized array
     *
     * This class provide deep copy capabilities to std::unique_ptr<T[]>
     */
    template <typename T>
    class DArray
    {
    private:
        size_t _size;
        std::unique_ptr<T[]> buffer;

    public:
        // constructors
        DArray() = default;
        explicit DArray(size_t size):
            _size(size),
            buffer(std::make_unique<T[]>(size))
        {}

        // move constructor
        DArray(DArray&& oth) = default;

        // deep-copy constructor
        DArray(const DArray& oth):
            _size(oth._size),
            buffer(std::make_unique<T[]>(oth._size))
        {
            copy_n(oth.buffer.get(), _size, buffer.get());
        }

        // move & deep-copy assignment (copy&swap idiom)
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
            swap(first._size  , second._size);
            swap(first.buffer, second.buffer);
        }

        // data access capabilities
        inline T* get() const { return buffer.get(); }
        inline T& operator[](size_t i) const { return buffer[i]; };
        inline size_t size() const { return _size; }
    };

}

#endif // __DARRAY_H__
