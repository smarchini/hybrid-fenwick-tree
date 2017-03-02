#include "../include/byte_fenwick_tree.hpp"
#include "../include/broadword.hpp"

constexpr std::uint64_t ByteFenwickTree::MASK[];

/**
 * get_size - Given it's height, compute the required size of an element
 * @height: Height of the element.
 *
 * This function compute (@height+6) / 8 + 1 in order to avoid some magic
 * numbers around the code.
 *
 * 6 is the LEAF_BITSIZE as in CompactFenwickTree and 8 is one Byte in bit.
 * Anything less than or equal k*8 require k Bytes.
 *
 * Returns: The size of an element of a given height.
 */
static inline std::size_t get_size(std::size_t height)
{
    return (height+13) >> 3;
}

ByteFenwickTree::ByteFenwickTree(std::uint64_t sequence[], std::size_t size) :
    size(size),
    levels(find_last_set(size)),
    level_start(new std::size_t[levels+1])
{
    level_start[0] = 0;
    for (size_t i = 1; i <= levels; i++)
        level_start[i] = level_start[i-1] + ((size+1) / (1<<i)) * get_size(i-1);

    tree = new std::uint8_t[level_start[levels]];

    for (std::size_t l = 0; l < levels; l++) {
        for (std::size_t node = 1<<l; node <= size; node += 1 << (l+1)) { // teoricamente potrebbe essere in parallelo (?)
            const std::size_t curr_byte_pos = level_start[l] + get_size(l) * (node >> (l+1)); // volendo lo posso incrementare man mano
            std::uint64_t * const curr_element = reinterpret_cast<std::uint64_t * const>(tree + curr_byte_pos);

            std::size_t sequence_idx = node-1;
            std::uint64_t value = sequence[sequence_idx];
            for (std::size_t j = 0; j < l; j++) {
                sequence_idx >>= 1;
                const std::size_t prev_byte_pos = level_start[j] + get_size(j) * sequence_idx;
                const uint64_t * const prev_element = reinterpret_cast<const std::uint64_t * const>(tree + prev_byte_pos);

                value += *prev_element & MASK[get_size(j)-1];
            }

            *curr_element &= ~MASK[get_size(l)-1];
            *curr_element |= value & MASK[get_size(l)-1];
        }
    }
}

ByteFenwickTree::~ByteFenwickTree()
{
    delete[] tree;
    delete[] level_start;
}

std::uint64_t ByteFenwickTree::get(std::size_t idx) const
{
    std::uint64_t sum = 0;
    idx++;

    while (idx != 0) {
        const std::size_t height = find_first_set(idx) - 1;
        const std::size_t level_idx = idx >> (1 + height);

        const std::size_t byte_pos = level_start[height] + get_size(height) * level_idx;
        const std::uint64_t * const compact_element = reinterpret_cast<const std::uint64_t * const>(tree + byte_pos);

        sum += *compact_element & MASK[get_size(height)-1];
        idx = drop_first_set(idx);
    }

    return sum;
}

void ByteFenwickTree::set(std::size_t idx, std::uint64_t inc)
{
    idx++;

    do {
        const std::size_t height = find_first_set(idx) - 1;
        const std::size_t level_idx = idx >> (1 + height);
        const std::size_t byte_pos = level_start[height] + get_size(height) * level_idx;
        std::uint64_t * const compact_element = reinterpret_cast<std::uint64_t * const>(tree + byte_pos);
        *compact_element += inc;

        idx += mask_first_set(idx);
    } while (idx <= size);
}

std::uint64_t ByteFenwickTree::find(std::uint64_t val) const
{
    std::size_t node = 0, idx = 0;

    for (std::uint64_t height = levels - 1; height != -1ULL; height--) {
        const std::size_t byte_pos = level_start[height] + get_size(height) * idx;
        const std::uint64_t * const compact_element = reinterpret_cast<const std::uint64_t * const>(tree + byte_pos);
        const std::uint64_t value = *compact_element & MASK[get_size(height)-1];

        idx <<= 1;

        if (val >= value) {
            idx++;
            val -= value;
            node += 1 << height;
        }
    }

    return node - 1;
}
