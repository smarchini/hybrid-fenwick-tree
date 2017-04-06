#include "../include/shrank_fenwick_tree.hpp"
#include "../include/broadword.hpp"

using std::size_t; using std::uint64_t; using std::uint32_t; using std::uint16_t; using std::uint8_t;

/**
 * auint64_t - Aliased uint64_t
 *
 * Strict aliasing rule: it is illegal (with some exceptions [1]) to access the
 * same memory location using pointers of different types. Accessing such an
 * object invokes undefined behavior.
 *
 * GCC __may_alias__ attribute prevents the compiler to makes optimizations who
 * relies on strict aliasing rule. With this type is now valid to access aliased
 * data.
 *
 * [1] Type aliasing http://en.cppreference.com/w/cpp/language/reinterpret_cast
 */
using auint64_t = std::uint64_t __attribute__((__may_alias__));

inline size_t get_bitpos(size_t n)
{
    return (ShrankFenwickTree::LEAF_BITSIZE+1)*n - popcount(n);
}

ShrankFenwickTree::ShrankFenwickTree(uint64_t sequence[], size_t size) :
    size(size),
    levels(find_last_set(size))
{
    tree = std::make_unique<uint8_t[]>(get_bitpos(size-1) / 8 + 1 + 4); // +4 to prevent segfault on the last element

    for (size_t i = 1; i <= size; i++) {
        const size_t bitpos = get_bitpos(i-1);
        auint64_t * const element = reinterpret_cast<auint64_t*>(&tree[bitpos/8]);

        const size_t bitsize = LEAF_BITSIZE + find_first_set(i) - 1;
        const size_t shift = bitpos & 0b111;
        const uint64_t mask = compact_bitmask(bitsize, shift);

        *element &= ~mask;
        *element |= mask & (sequence[i-1] << shift);
    }

    for (size_t m = 2; m <= size; m <<= 1) {
        for (size_t idx = m; idx <= size; idx += m) {
            const size_t left_bitpos = get_bitpos(idx-1);
            auint64_t * const left_element = reinterpret_cast<auint64_t*>(&tree[left_bitpos/8]);
            const size_t left_shift = left_bitpos & 0b111;

            const size_t right_bitpos = get_bitpos(idx - m/2 - 1);
            auint64_t * const right_element = reinterpret_cast<auint64_t*>(&tree[right_bitpos/8]);
            const size_t right_shift = right_bitpos & 0b111;

            const size_t right_bitsize = LEAF_BITSIZE + find_first_set(idx - m/2) - 1;
            const uint64_t right_mask = compact_bitmask(right_bitsize, right_shift);

            uint64_t value = (right_mask & *right_element) >> right_shift;
            *left_element += value << left_shift;
        }
    }
}

uint64_t ShrankFenwickTree::get(size_t idx) const
{
    uint64_t sum = 0ULL;

    for (idx = idx+1; idx != 0; idx = drop_first_set(idx)) {
        const size_t bit_pos = get_bitpos(idx-1);
        const auint64_t * const compact_element = reinterpret_cast<auint64_t*>(&tree[bit_pos/8]);

        const size_t height = find_first_set(idx) - 1;
        const size_t shift = bit_pos & 0b111;
        const uint64_t mask = compact_bitmask(LEAF_BITSIZE+height, shift);

        sum += (*compact_element & mask) >> shift;
    }

    return sum;
}

void ShrankFenwickTree::set(size_t idx, int64_t inc)
{
    for (idx = idx+1; idx <= size; idx += mask_first_set(idx)) {
        const size_t bit_pos = get_bitpos(idx-1);
        auint64_t * const compact_element = reinterpret_cast<auint64_t*>(&tree[bit_pos/8]);

        const size_t shift = bit_pos & 0b111;
        *compact_element += inc << shift;
    }
}

size_t ShrankFenwickTree::find(uint64_t val, bool complement) const
{
    size_t node = 0;
    const size_t bit_max = bit_count();

    for (size_t m = mask_last_set(size); m != 0; m >>= 1) {
        const size_t bit_pos = get_bitpos(node+m-1);
        const auint64_t * const compact_element = reinterpret_cast<auint64_t*>(&tree[bit_pos/8]);

        const size_t height = find_first_set(node+m) - 1;
        const size_t shift = bit_pos & 0b111;
        const uint64_t mask = compact_bitmask(LEAF_BITSIZE+height, 0);

        uint64_t value = 0;
        if (bit_pos >= bit_max) value = -1ULL;
        else value = (*compact_element >> shift) & mask;

        if (complement)
            value = (1ULL << (LEAF_BITSIZE + height - 1)) - value;

        if (val >= value) {
            node += m;
            val -= value;
        }
    }

    return node - 1;
}

size_t ShrankFenwickTree::bit_count() const
{
    return get_bitpos(size);
}
