#include "../include/shrank_fenwick_tree.hpp"
#include "../include/broadword.hpp"

using std::size_t; using std::uint64_t; using std::uint32_t; using std::uint16_t; using std::uint8_t;


inline size_t get_bitpos(size_t n)
{
    return 7*n - __builtin_popcountll(n);
}


ShrankFenwickTree::ShrankFenwickTree(uint64_t sequence[], size_t size) :
    size(size),
    levels(find_last_set(size))
{
    tree = new uint8_t[get_bitpos(size-1) / 8 + 1 + 4]; // +4 to prevent segfault on the last element

    for (size_t i = 1; i <= size; i++) {
        const size_t bitpos = get_bitpos(i-1);
        uint64_t * const element = reinterpret_cast<uint64_t * const>(tree + bitpos / 8);
        //__builtin_prefetch (element, 0, 1);

        const size_t bitsize = 5 + find_first_set(i);
        const size_t shift = bitpos & 0b111;
        const uint64_t mask = compact_bitmask(bitsize, shift);

        *element &= ~mask;
        *element |= mask & (sequence[i-1] << shift);
    }

    for (size_t m = 2; m <= size; m <<= 1) {
        for (size_t idx = m; idx <= size; idx += m) {
            const size_t left_bitpos = get_bitpos(idx-1);
            uint64_t * const left_element = reinterpret_cast<uint64_t * const>(tree + left_bitpos / 8);
            //__builtin_prefetch (left_element, 0, 1);
            const size_t left_shift = left_bitpos & 0b111;

            const size_t right_bitpos = get_bitpos(idx - m/2 - 1);
            uint64_t * const right_element = reinterpret_cast<uint64_t * const>(tree + right_bitpos / 8);
            //__builtin_prefetch (left_element, 0, 0);
            const size_t right_shift = right_bitpos & 0b111;

            const size_t right_bitsize = 5 + find_first_set(idx - m/2);
            const uint64_t right_mask = compact_bitmask(right_bitsize, right_shift);

            uint64_t value = (right_mask & *right_element) >> right_shift;
            *left_element += value << left_shift;
        }
    }
}


ShrankFenwickTree::~ShrankFenwickTree()
{
    if (tree) delete[] tree;
}


uint64_t ShrankFenwickTree::get(size_t idx) const
{
    uint64_t sum = 0ULL;

    for (idx = idx+1; idx != 0; idx = drop_first_set(idx)) {
        const size_t bit_pos = get_bitpos(idx-1);
        const uint64_t * const compact_element = reinterpret_cast<const uint64_t * const>(tree + bit_pos / 8);
        //__builtin_prefetch (compact_element, 0, 1);

        const size_t height = find_first_set(idx) - 1;
        const size_t shift = bit_pos & 0b111;
        const uint64_t mask = compact_bitmask(LEAF_BITSIZE+height, shift);

        sum += (*compact_element & mask) >> shift;
    }

    return sum;
}


void ShrankFenwickTree::set(size_t idx, uint64_t inc)
{
    for (idx = idx+1; idx <= size; idx += mask_first_set(idx)) {
        const size_t bit_pos = get_bitpos(idx-1);
        uint64_t * const compact_element = reinterpret_cast<uint64_t * const>(tree + bit_pos / 8);

        // TODO: quando si risolve il problema della find, provare a usare un
        // do-while e fare accessi forward. attualmente il do-while è più lento.
        __builtin_prefetch (compact_element, 1, 1);

        const size_t shift = bit_pos & 0b111;
        *compact_element += inc << shift;
    }
}


size_t ShrankFenwickTree::find(uint64_t val) const
{
    size_t node = 0;
    const size_t bit_max = bit_count();

    for (size_t m = mask_last_set(size); m != 0; m >>= 1) {
        const size_t bit_pos = get_bitpos(node+m-1);

        // TODO: provare ad usare le parentesi quadre: non ho bisogno di
        // ricordarmi del pointer. non dovrebbe cambiare niente, mi aspetto che
        // il compilatore ottimizzi tutto.
        const uint64_t * const compact_element = reinterpret_cast<const uint64_t * const>(tree + bit_pos / 8);

        // TODO: meglio non prefetchare quello che è già in chache, trovare un
        // modo per risolvere. verificare se è davvero questo il problema.
        __builtin_prefetch (compact_element, 0, 1);

        const size_t height = find_first_set(node+m) - 1;
        const size_t shift = bit_pos & 0b111;
        const uint64_t mask = compact_bitmask(LEAF_BITSIZE+height, 0);

        uint64_t value = 0;
        if (bit_pos >= bit_max) value = -1ULL;
        else value = (*compact_element >> shift) & mask;

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
