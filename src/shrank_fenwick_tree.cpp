#include "../include/shrank_fenwick_tree.hpp"
#include "../include/broadword.hpp"


inline std::size_t get_bitpos(std::size_t n)
{
    n--;
    return 7*n - __builtin_popcountll(n);
}

ShrankFenwickTree::ShrankFenwickTree(std::uint64_t sequence[], std::size_t size) :
    size(size),
    levels(find_last_set(size))
{
    tree = new std::uint8_t[get_bitpos(size) / 8 + 1];

    for (std::size_t i = 1; i <= size; i++) {
        const std::size_t bitpos = get_bitpos(i);
        const std::size_t bitsize = 5 + find_first_set(i);
        const std::size_t shift = bitpos & 0b111;
        const std::uint64_t mask = compact_bitmask(bitsize, shift);

        std::uint64_t * const element = reinterpret_cast<std::uint64_t * const>(tree + bitpos / 8);

        *element &= ~mask;
        *element |= mask & (sequence[i-1] << shift);
    }

    for (std::size_t m = 2; m <= size; m <<= 1) {
        for (std::size_t idx = m; idx <= size; idx += m) {
            const std::size_t left_bitpos = get_bitpos(idx);
            const std::size_t left_shift = left_bitpos & 0b111;
            std::uint64_t * const left_element = reinterpret_cast<std::uint64_t * const>(tree + left_bitpos / 8);

            const std::size_t right_bitpos = get_bitpos(idx - m/2);
            const std::size_t right_bitsize = 5 + find_first_set(idx - m/2);
            const std::size_t right_shift = right_bitpos & 0b111;
            const std::uint64_t right_mask = compact_bitmask(right_bitsize, right_shift);
            std::uint64_t * const right_element = reinterpret_cast<std::uint64_t * const>(tree + right_bitpos / 8);

            std::uint64_t value = (right_mask & *right_element) >> right_shift;
            *left_element += value << left_shift;
        }
    }
}


ShrankFenwickTree::~ShrankFenwickTree()
{
    if (tree) delete[] tree;
}


std::uint64_t ShrankFenwickTree::get(std::size_t idx) const
{
    std::uint64_t sum = 0ULL;

    for (idx = idx+1; idx != 0; idx = drop_first_set(idx)) {
        const std::size_t height = find_first_set(idx) - 1;
        const std::size_t bit_pos = get_bitpos(idx);
        const std::uint64_t * const compact_element = reinterpret_cast<const std::uint64_t * const>(tree + bit_pos / 8);

        const std::size_t shift = bit_pos & 0b111;
        const std::uint64_t mask = compact_bitmask(LEAF_BITSIZE+height, shift);

        sum += (*compact_element & mask) >> shift;
    }

    return sum;
}


void ShrankFenwickTree::set(std::size_t idx, std::uint64_t inc)
{
    for (idx = idx+1; idx <= size; idx += mask_first_set(idx)) {
        const std::size_t bit_pos = get_bitpos(idx);
        std::uint64_t * const compact_element = reinterpret_cast<std::uint64_t * const>(tree + bit_pos / 8);

        const std::size_t shift = bit_pos & 0b111;
        *compact_element += inc << shift;
    }
}


std::size_t ShrankFenwickTree::find(std::uint64_t val) const
{
    std::size_t node = 0;

    for (std::size_t m = mask_last_set(size); m != 0; m >>= 1) {
        const std::size_t height = find_first_set(node+m) - 1;
        const std::size_t bit_pos = get_bitpos(node+m);
        const std::uint64_t * const compact_element = reinterpret_cast<const std::uint64_t * const>(tree + bit_pos / 8);

        const std::size_t shift = bit_pos & 0b111;
        const std::uint64_t mask = compact_bitmask(LEAF_BITSIZE+height, 0);
        const std::uint64_t value = (*compact_element >> shift) & mask;

        __builtin_prefetch (tree + get_bitpos(node+(m>>1)) / 8, 0, 0);
        __builtin_prefetch (tree + get_bitpos(node+m+(m>>1)) / 8, 0, 0);

        if (val >= value) {
            node += m;
            val -= value;
        }
    }

    return node - 1;
}
