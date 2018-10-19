#ifndef __COMMON_H__
#define __COMMON_H__

#include <cstdint>

#include "broadword.hpp"
#include "darray.hpp"

namespace dyn {
    using std::size_t; using std::uint64_t; using std::uint32_t; using std::uint16_t; using std::uint8_t;

    /**
     * auint64_t - Aliased uint64_t
     *
     * Strict aliasing rule: it's illegal to access the same memory location
     * with data of different types. If you have two pointers T* and a U*, the
     * compiler can assume they are not pointingthe same data. Accessing such a
     * data invokes undefined behavior.
     *
     * GCC __may_alias__ attribute is basically the opposite of the C keywoard
     * 'restrict', it prevents the compiler to makes such assumptions. With this
     * type is now valid to access aliased data.
     */
    using auint64_t = std::uint64_t __attribute__((__may_alias__));
    using auint32_t = std::uint32_t __attribute__((__may_alias__));
    using auint16_t = std::uint16_t __attribute__((__may_alias__));
    using  auint8_t =  std::uint8_t __attribute__((__may_alias__));

    /**
     * Bitmask array used in ByteL and ByteF
     */
    static constexpr uint64_t BYTE_MASK[] = { 0x0ULL,
                                              0xFFULL,
                                              0xFFFFULL,
                                              0xFFFFFFULL,
                                              0xFFFFFFFFULL,
                                              0xFFFFFFFFFFULL,
                                              0xFFFFFFFFFFFFULL,
                                              0xFFFFFFFFFFFFFFULL,
                                              0xFFFFFFFFFFFFFFFFULL };

    /**
     * log2() - Compile time log2 roundup
     */
    constexpr size_t log2(size_t n)
    {
        return ((n<2) ? 1 : 1+log2(n/2));
    }
}

#endif // __COMMON_H__
