#include "../include/simple_fenwick_tree.hpp"
#include "../include/broadword.hpp"
#include <algorithm>

using std::size_t; using std::uint64_t; using std::uint32_t; using std::uint16_t; using std::uint8_t;


SimpleFenwickTree::SimpleFenwickTree(uint64_t sequence[], size_t size) :
    tree(new uint64_t[size]),
    size(size)
{
    std::copy_n(sequence, size, tree);

    for (size_t m = 2; m <= size; m <<= 1) {
        for (size_t idx = m; idx <= size; idx += m)
            tree[idx - 1] += tree[idx - m/2 - 1];
    }
}


SimpleFenwickTree::~SimpleFenwickTree()
{
    if (tree) delete[] tree;
}


uint64_t SimpleFenwickTree::get(size_t idx) const
{
    uint64_t sum = 0;

    for (idx = idx+1; idx != 0; idx = drop_first_set(idx))
        sum += tree[idx - 1];

    return sum;
}


void SimpleFenwickTree::set(size_t idx, uint64_t inc)
{
    for (idx = idx+1; idx <= size; idx += mask_first_set(idx))
        tree[idx-1] += inc;
}


size_t SimpleFenwickTree::find(uint64_t val) const
{
    size_t node = 0;

    for (size_t m = mask_last_set(size); m != 0; m >>= 1) {
        if(node+m-1 < size && val >= tree[node+m-1]) {
            node += m;
            val -= tree[node-1];
        }
    }

    return node - 1;
}

size_t SimpleFenwickTree::bit_count() const
{
    return 64 * size;
}
