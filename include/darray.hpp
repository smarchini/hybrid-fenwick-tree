#ifndef __DARRAY_HPP__
#define __DARRAY_HPP__

#include <memory>
#include <algorithm>
#include <sys/mman.h>
#include <assert.h>

#include "common.hpp"
#include <iostream>

// TODO: echo 0 | sudo tee /proc/sys/vm/nr_hugepages
// TODO: watch -n 0.1 cat /sys/devices/system/node/node*/meminfo
namespace hft {

    /**
     * class DArray - Dinamically-allocated fixed-sized array
     *
     * This class is a wrapper for mmap to take advantage of hugepages [1].
     *
     * [1] https://www.kernel.org/doc/Documentation/vm/hugetlbpage.txt
     *
     */
    template <typename T>
    class DArray
    {
    public:
        static constexpr size_t PAGESIZE = 2048*1024; // 2MB hugepages
        static constexpr int PROTECTION = PROT_READ | PROT_WRITE;
        static constexpr int FLAGS = MAP_HUGETLB | MAP_PRIVATE | MAP_ANONYMOUS;

    private:
        size_t _size;
        size_t space; // it should to be a multiple of PAGESIZE
        T* buffer = nullptr;

    public:
        // constructors
        DArray<T>() : DArray<T>(0) { };
        explicit DArray<T>(size_t size):
            _size(size),
            space(mround(PAGESIZE, (size > 0 ? size : 1) * sizeof(T)))
        {
            void *result = mmap(nullptr, space, PROTECTION, FLAGS, -1, 0);
            assert(result != MAP_FAILED);
            madvise(result, space, MADV_HUGEPAGE); // TODO: provare con MADV_RANDOM
            buffer = static_cast<T*>(result);
        }

        // move constructor
        DArray(DArray&& oth):
            _size(std::move(oth._size)),
            space(std::move(oth.space)),
            buffer(std::move(oth.buffer)) { }

        // move assignment (copy&swap idiom)
        DArray<T>& operator=(DArray<T> oth)
        {
            swap(*this, oth);
            return *this;
        }

        // destructor
        ~DArray<T>()
        {
            int result = munmap(buffer, space);
            assert(result == 0);
        }

        // swap
        friend void swap(DArray<T>& first, DArray<T>& second) noexcept
        {
            using std::swap;
            swap(first._size, second._size);
            swap(first.space, second.space);
            swap(first.buffer, second.buffer);
        }

        // data access capabilities
        inline T*  get() const { return buffer; }
        inline T& operator[](size_t i) const {
            std::cout << "(space = " << space << ") buffer[" << i << "] = " << std::flush;
            std::cout << (int)buffer[i] << std::endl;
            return buffer[i];
        };
        inline size_t size() const { return _size; }

        size_t bit_count() const
        {
            return sizeof(DArray<T>)*8 + sizeof(T)*_size*8;
        }
    };

}

#endif // __DARRAY_HPP__
