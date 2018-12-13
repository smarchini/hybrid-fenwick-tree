#ifndef __LINUXHUGETLB_HPP__
#define __LINUXHUGETLB_HPP__

#include <memory>
#include <algorithm>
#include <sys/mman.h>
#include <assert.h>
#include <iostream>

#include "../common.hpp"

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
        static constexpr size_t PAGESIZE = 2048*1024;
        static constexpr int PROTECTION = PROT_READ | PROT_WRITE;
        static constexpr int ADVICE = MADV_HUGEPAGE;

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
#ifdef HFT_TRANSPARENT
            std::cout << "transparent space = " << space << std::endl;
            void *result = mmap(nullptr, space, PROTECTION, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
            assert(result != MAP_FAILED);
            madvise(result, space, ADVICE);
#else
            std::cout << "space = " << space << std::endl;
            void *result = mmap(nullptr, space, PROTECTION, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
            assert(result != MAP_FAILED);
#endif
            buffer = static_cast<T*>(result);
        }

        // move constructor
        DArray(DArray<T>&& oth):
            _size(std::exchange(oth._size, 0)),
            space(std::exchange(oth.space, 0)),
            buffer(std::exchange(oth.buffer, nullptr)) { }

        // move assignment (copy&swap idiom)
        DArray<T>& operator=(DArray<T> &&oth)
        {
            swap(*this, oth);
            return *this;
        }

        // destructor
        ~DArray<T>()
        {
            if (buffer) {
                int result = munmap(buffer, space);
                assert(result == 0);
            }
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
        inline T& operator[](size_t i) const { return buffer[i]; };
        inline size_t size() const { return _size; }

        size_t bit_count() const
        {
            return sizeof(DArray<T>)*8 + space * 8;
        }
    };

}

#endif // __LINUXHUGETLB_HPP__
