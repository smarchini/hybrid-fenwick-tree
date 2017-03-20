#include "../include/typed_fenwick_tree.hpp"
#include "../include/broadword.hpp"


TypedFenwickTree::TypedFenwickTree(std::uint64_t sequence[], std::size_t size) :
    size(size),
    levels(find_last_set(size)),
    level_start(new std::size_t[levels+1])
{
    std::size_t type_ends[4] = {0};

    level_start[0] = 0;
    for (size_t i = 1, j = 0; i <= levels; i++) {
        type_ends[j] = level_start[i-1] + ((size+1) / (1<<i));
        level_start[i] = (i-1 == 8-LEAF_BITSIZE || i-1 == 16-LEAF_BITSIZE || i-1 == 32-LEAF_BITSIZE) ? 0 : type_ends[j];

        if (i-1 == 8-LEAF_BITSIZE || i-1 == 16-LEAF_BITSIZE || i-1 == 32-LEAF_BITSIZE) j++;
    }

    switch(levels+6) {
    case 33 ... 64: tree64 = new std::uint64_t[type_ends[3]];
    case 17 ... 32: tree32 = new std::uint32_t[type_ends[2]];
    case 9 ... 16:  tree16 = new std::uint16_t[type_ends[1]];
    default:         tree8 = new std::uint8_t[type_ends[0]];
    }

    fill_tree<std::uint8_t, LEAF_BITSIZE, 8>(tree8, sequence);
    fill_tree<std::uint16_t, 9, 16>(tree16, sequence);
    fill_tree<std::uint32_t, 17, 32>(tree32, sequence);
    fill_tree<std::uint64_t, 33, 64>(tree64, sequence);
}

TypedFenwickTree::~TypedFenwickTree()
{
    if (tree8) delete[] tree8;
    if (tree16) delete[] tree16;
    if (tree32) delete[] tree32;
    if (tree64) delete[] tree64;
    if (level_start) delete[] level_start;
}

std::uint64_t TypedFenwickTree::get(std::size_t idx) const
{
    std::uint64_t sum = 0ULL;

    idx++;
    std::size_t index = 0ULL;

    do {
        index += mask_last_set(idx ^ index);
        const std::size_t height = find_first_set(index) - 1;
        const std::size_t level_idx = index >> (1 + height);
        const std::size_t tree_idx = level_start[height] + level_idx;

        switch(height+6) {
        case 6 ... 8:   sum +=  tree8[tree_idx]; break;
        case 9 ... 16:  sum += tree16[tree_idx]; break;
        case 17 ... 32: sum += tree32[tree_idx]; break;
        case 33 ... 64: sum += tree64[tree_idx];
        }
    } while (idx ^ index);

    return sum;
}

void TypedFenwickTree::set(std::size_t idx, std::uint64_t inc)
{
    for (idx = idx+1; idx <= size; idx += mask_first_set(idx)) {
        const std::size_t height = find_first_set(idx) - 1;
        const std::size_t level_idx = idx >> (1 + height);
        const std::size_t tree_idx = level_start[height] + level_idx;

        switch(height+6) {
        case 6 ... 8:    tree8[tree_idx] += inc; break;
        case 9 ... 16:  tree16[tree_idx] += inc; break;
        case 17 ... 32: tree32[tree_idx] += inc; break;
        case 33 ... 64: tree64[tree_idx] += inc;
        }
    }
}

std::size_t TypedFenwickTree::find(std::uint64_t val) const
{
    std::size_t node = 0, idx = 0;

    for (std::uint64_t height = levels - 1; height != -1ULL; height--) {
        const std::size_t tree_idx = level_start[height] + idx;

        std::uint64_t value=0;
        switch(height+6) {
        case 6 ... 8:   value =  tree8[tree_idx]; break;
        case 9 ... 16:  value = tree16[tree_idx]; break;
        case 17 ... 32: value = tree32[tree_idx]; break;
        case 33 ... 64: value = tree64[tree_idx];
        }

        idx <<= 1;

        if (val >= value) {
            idx++;
            val -= value;
            node += 1 << height;
        }
    }

    return node - 1;
}
