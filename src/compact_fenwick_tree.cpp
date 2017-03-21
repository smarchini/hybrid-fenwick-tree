#include "../include/compact_fenwick_tree.hpp"
#include "../include/broadword.hpp"

using std::size_t; using std::uint64_t; using std::uint32_t; using std::uint16_t; using std::uint8_t;

CompactFenwickTree::CompactFenwickTree(uint64_t sequence[], size_t size) :
    size(size),
    levels(find_last_set(size)),
    level_start(new size_t[levels+1])
{
    level_start[0] = 0;
    for (size_t i = 1; i <= levels; i++) {
        // Compute: \sum_{i=1}^{k} \frac{(5+i)*2^n}{2^k}
        level_start[i] = (-i + 7*(1<<i) - 7) * (1 << (levels-i));
    }

    tree = new uint8_t[(level_start[levels]-1) / 8 + 1];

    for (size_t l = 0; l < levels; l++) {
        for (size_t node = 1<<l; node <= size; node += 1 << (l+1)) {
            const size_t curr_bit_pos = level_start[l] + (LEAF_BITSIZE+l) * (node >> (l+1));
            const size_t curr_shift = curr_bit_pos & 0b111;
            const uint64_t curr_mask = compact_bitmask(LEAF_BITSIZE+l, curr_shift);
            uint64_t * const curr_element = reinterpret_cast<uint64_t * const>(tree + curr_bit_pos / 8);

            size_t sequence_idx = node-1;
            uint64_t value = sequence[sequence_idx];
            for (size_t j = 0; j < l; j++) {
                sequence_idx >>= 1;

                const size_t prev_bit_pos = level_start[j] + (LEAF_BITSIZE+j) * sequence_idx;
                const size_t prev_shift = prev_bit_pos & 0b111;
                const uint64_t prev_mask = compact_bitmask(LEAF_BITSIZE+j, prev_shift);
                const uint64_t * const prev_element = reinterpret_cast<const uint64_t * const>(tree + prev_bit_pos / 8);

                value += (*prev_element & prev_mask) >> prev_shift;
            }

            *curr_element &= ~curr_mask;
            *curr_element |= curr_mask & (value << curr_shift);
        }
    }
}

CompactFenwickTree::~CompactFenwickTree()
{
    if (tree) delete[] tree;
    if (level_start) delete[] level_start;
}


uint64_t CompactFenwickTree::get(size_t idx) const
{
    uint64_t sum = 0ULL;

    idx++;
    size_t index = 0ULL;

    do {
        index += mask_last_set(idx ^ index);
        const size_t height = find_first_set(index) - 1;
        const size_t level_idx = index >> (1 + height);

        const size_t bit_pos = level_start[height] + (LEAF_BITSIZE+height) * level_idx;
        const size_t shift = bit_pos & 0b111;
        const uint64_t mask = compact_bitmask(LEAF_BITSIZE+height, shift);
        const uint64_t * const compact_element = reinterpret_cast<const uint64_t * const>(tree + bit_pos / 8);

        sum += (*compact_element & mask) >> shift;
    } while (idx ^ index);

    return sum;
}


void CompactFenwickTree::set(size_t idx, uint64_t inc)
{
    for (idx = idx+1; idx <= size; idx += mask_first_set(idx)) {
        const size_t height = find_first_set(idx) - 1;
        const size_t level_idx = idx >> (1 + height);
        const size_t bit_pos = level_start[height] + (LEAF_BITSIZE+height) * level_idx;
        const size_t shift = bit_pos & 0b111;
        uint64_t * const compact_element = reinterpret_cast<uint64_t * const>(tree + bit_pos / 8);
        *compact_element += inc << shift;
    }
}


size_t CompactFenwickTree::find(uint64_t val) const
{
    size_t node = 0, idx = 0;

    for (uint64_t height = levels - 1; height != -1ULL; height--) {
        const size_t bit_pos = level_start[height] + (LEAF_BITSIZE+height) * idx;
        const uint64_t * const compact_element = reinterpret_cast<const uint64_t * const>(tree + bit_pos / 8);
	__builtin_prefetch(compact_element, 0, 1);

        const size_t shift = bit_pos & 0b111;
        const uint64_t mask = compact_bitmask(LEAF_BITSIZE+height, 0);
        const uint64_t value = (*compact_element >> shift) & mask;

        idx <<= 1;

        if (val >= value) {
            idx++;
            val -= value;
            node += 1 << height;
        }
    }

    return node - 1;
}

size_t CompactFenwickTree::bit_count() const
{
    return level_start[levels];
}
