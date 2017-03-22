#include "../include/byte_fenwick_tree.hpp"
#include "../include/broadword.hpp"

constexpr std::uint64_t ByteFenwickTree::MASK[];
using std::size_t; using std::uint64_t; using std::uint32_t; using std::uint16_t; using std::uint8_t;

/**
 * get_size - Given it's height, compute the required size of an element
 * @height: Height of the element.
 *
 * This function compute (@height+6-1) / 8 + 1 in order to avoid some magic
 * numbers around the code.
 *
 * 6 is the LEAF_BITSIZE as in CompactFenwickTree and 8 is one Byte in bit.
 * Anything less than or equal k*8 require k Bytes.
 *
 * Returns: The size of an element of a given height.
 */
static inline size_t get_size(size_t height)
{
    // TODO: "Ma non dovrebbe essere relativamente semplice estendere la formula
    // con la popcount che permette di fare l'interleaving nel caso Shrank per
    // fare interleaving nel caso byte?" (mail Vigna)

    return (height+13) >> 3;
}

ByteFenwickTree::ByteFenwickTree(uint64_t sequence[], size_t size) :
    size(size),
    levels(find_last_set(size)),
    level_start(new size_t[levels+1])
{
    level_start[0] = 0;
    for (size_t i = 1; i <= levels; i++) {
        // Compute: sum_{k=0}^{i} [(size + 2^{k-i}) / 2^k] * get_size(k)
        level_start[i] = ((size + (1<<(i-1))) / (1<<i)) * get_size(i-1) + level_start[i-1];
    }

    tree = new uint8_t[level_start[levels] + 3]; // +3 to prevent segfault on the last element

    for (size_t l = 0; l < levels; l++) {
        for (size_t node = 1<<l; node <= size; node += 1 << (l+1)) {
            const size_t curr_byte_pos = level_start[l] + get_size(l) * (node >> (l+1));
            uint64_t * const curr_element = reinterpret_cast<uint64_t * const>(tree + curr_byte_pos);

            size_t sequence_idx = node-1;
            uint64_t value = sequence[sequence_idx];
            for (size_t j = 0; j < l; j++) {
                sequence_idx >>= 1;
                const size_t prev_byte_pos = level_start[j] + get_size(j) * sequence_idx;
                const uint64_t * const prev_element = reinterpret_cast<const uint64_t * const>(tree + prev_byte_pos);

                value += *prev_element & MASK[get_size(j)];
            }

            *curr_element &= ~MASK[get_size(l)];
            *curr_element |= value & MASK[get_size(l)];
        }
    }
}

ByteFenwickTree::~ByteFenwickTree()
{
    if (tree) delete[] tree;
    if (level_start) delete[] level_start;
}

uint64_t ByteFenwickTree::get(size_t idx) const
{
    uint64_t sum = 0;

    idx++;
    size_t index = 0ULL;

    do {
        index += mask_last_set(idx ^ index);
        const size_t height = find_first_set(index) - 1;
        const size_t level_idx = index >> (1 + height);
        const size_t elem_size = get_size(height);
        const size_t byte_pos = level_start[height] + elem_size * level_idx;
        const uint64_t * const compact_element = reinterpret_cast<const uint64_t * const>(tree + byte_pos);

        sum += *compact_element & MASK[elem_size];
    } while (idx ^ index);

    return sum;
}

void ByteFenwickTree::set(size_t idx, uint64_t inc)
{
    for (idx = idx+1; idx <= size; idx += mask_first_set(idx)) {
        const size_t height = find_first_set(idx) - 1;
        const size_t level_idx = idx >> (1 + height);
        const size_t byte_pos = level_start[height] + get_size(height) * level_idx;
        uint64_t * const compact_element = reinterpret_cast<uint64_t * const>(tree + byte_pos);

        *compact_element += inc;
    }
}

size_t ByteFenwickTree::find(uint64_t val) const
{
    size_t node = 0, idx = 0;

    for (uint64_t height = levels - 1; height != -1ULL; height--) {
        const size_t elem_size = get_size(height);
        const size_t byte_pos = level_start[height] + elem_size * idx;
        const uint64_t * const compact_element = reinterpret_cast<const uint64_t * const>(tree + byte_pos);
        __builtin_prefetch(compact_element, 0, 0);

        uint64_t value = 0;
        if (byte_pos >= level_start[levels]) value = -1ULL;
        else value = *compact_element & MASK[elem_size];

        idx <<= 1;

        if(val >= value) {
            idx++;
            val -= value;
            node += 1 << height;
        }
    }

    return node <= size ? node-1 : size-1;
}

size_t ByteFenwickTree::bit_count() const
{
    return level_start[levels] * 8;
}
