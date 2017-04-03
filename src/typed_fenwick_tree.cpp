#include "../include/typed_fenwick_tree.hpp"
#include "../include/broadword.hpp"

using std::size_t; using std::uint64_t; using std::uint32_t; using std::uint16_t; using std::uint8_t;


TypedFenwickTree::TypedFenwickTree(uint64_t sequence[], size_t size) :
    size(size),
    levels(find_last_set(size)),
    level_start(std::make_unique<size_t[]>(levels+1))
{
    level_start[0] = 0;
    for (size_t i = 1, j = 0; i <= levels; i++) {
        type_ends[j] = (size + (1<<(i-1))) / (1<<i) + level_start[i-1];
        level_start[i] = (i-1 == 8-LEAF_BITSIZE || i-1 == 16-LEAF_BITSIZE || i-1 == 32-LEAF_BITSIZE) ? 0 : type_ends[j];

        if (i-1 == 8-LEAF_BITSIZE || i-1 == 16-LEAF_BITSIZE || i-1 == 32-LEAF_BITSIZE) j++;
    }

    switch(levels+LEAF_BITSIZE) {
    case 33 ... 64: tree64 = std::make_unique<uint64_t[]>(type_ends[3]);
    case 17 ... 32: tree32 = std::make_unique<uint32_t[]>(type_ends[2]);
    case 9 ... 16:  tree16 = std::make_unique<uint16_t[]>(type_ends[1]);
    default:         tree8 = std::make_unique<uint8_t[]>(type_ends[0]);
    }

    fill_tree<uint8_t, LEAF_BITSIZE, 8>(tree8.get(), sequence);
    fill_tree<uint16_t, 9, 16>(tree16.get(), sequence);
    fill_tree<uint32_t, 17, 32>(tree32.get(), sequence);
    fill_tree<uint64_t, 33, 64>(tree64.get(), sequence);
}

uint64_t TypedFenwickTree::get(size_t idx) const
{
    uint64_t sum = 0ULL;

    idx++;
    size_t index = 0ULL;

    do {
        index += mask_last_set(idx ^ index);
        const size_t height = find_first_set(index) - 1;
        const size_t level_idx = index >> (1 + height);
        const size_t tree_idx = level_start[height] + level_idx;

        switch(height+LEAF_BITSIZE) {
        case 33 ... 64: sum += tree64[tree_idx]; break;
        case 17 ... 32: sum += tree32[tree_idx]; break;
        case  9 ... 16: sum += tree16[tree_idx]; break;
        default:        sum +=  tree8[tree_idx]; break;
        }
    } while (idx ^ index);

    return sum;
}

void TypedFenwickTree::set(size_t idx, uint64_t inc)
{
    for (idx = idx+1; idx <= size; idx += mask_first_set(idx)) {
        const size_t height = find_first_set(idx) - 1;
        const size_t level_idx = idx >> (1 + height);
        const size_t tree_idx = level_start[height] + level_idx;

        switch(height+LEAF_BITSIZE) {
        case 33 ... 64: tree64[tree_idx] += inc; break;
        case 17 ... 32: tree32[tree_idx] += inc; break;
        case  9 ... 16: tree16[tree_idx] += inc; break;
        default       :  tree8[tree_idx] += inc; break;
        }
    }
}

size_t TypedFenwickTree::find(uint64_t val, bool complement) const
{
    size_t node = 0, idx = 0;

    for (uint64_t height = levels - 1; height != -1ULL; height--) {
        const size_t tree_idx = level_start[height] + idx;

        uint64_t value = 0;
        switch(height+LEAF_BITSIZE) {
        case 33 ... 64:
            if (tree_idx >= type_ends[3]) value = -1ULL;
            else value = tree64[tree_idx];
            break;
        case 17 ... 32:
            if (tree_idx >= type_ends[2]) value = -1ULL;
            else value = tree32[tree_idx];
            break;
        case 9 ... 16:
            if (tree_idx >= type_ends[1]) value = -1ULL;
            else value = tree16[tree_idx];
            break;
        default:
            if (tree_idx >= type_ends[0]) value = -1ULL;
            else value =  tree8[tree_idx];
            break;
        }

        if (complement)
            value = (1ULL << (LEAF_BITSIZE + height - 1)) - value;

        idx <<= 1;

        if (val >= value) {
            idx++;
            val -= value;
            node += 1 << height;
        }
    }

    return node <= size ? node-1 : size-1;
}

size_t TypedFenwickTree::bit_count() const
{
    return type_ends[0] * 8
        + type_ends[1] * 16
        + type_ends[2] * 32
        + type_ends[3] * 64;
}
