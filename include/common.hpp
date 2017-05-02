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
}

#endif // __COMMON_H__
