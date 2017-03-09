#include "../include/simple_fenwick_tree.hpp"
#include "../include/broadword.hpp"
#include <algorithm>


SimpleFenwickTree::SimpleFenwickTree(std::uint64_t sequence[], std::size_t size) :
    tree(new std::uint64_t[size]),
    size(size)
{
    std::copy_n(sequence, size, tree);

    for (std::size_t m = 2; m <= size; m <<= 1) {
        for (std::size_t idx = m; idx <= size; idx += m)
            tree[idx - 1] += tree[idx - m/2 - 1];
    }
}


SimpleFenwickTree::~SimpleFenwickTree()
{
    if (tree) delete[] tree;
}


std::uint64_t SimpleFenwickTree::get(std::size_t idx) const
{
    std::uint64_t sum = 0;

    for (idx = idx+1; idx != 0; idx = drop_first_set(idx))
        sum += tree[idx - 1];

    return sum;
}


void SimpleFenwickTree::set(std::size_t idx, std::uint64_t inc)
{
    for (idx = idx+1; idx <= size; idx += mask_first_set(idx))
        tree[idx-1] += inc;
}


std::size_t SimpleFenwickTree::find(std::uint64_t val) const
{
    std::size_t node = 0;

    for (std::size_t m = mask_last_set(size); m != 0; m >>= 1) {
        if (val >= tree[node+m-1]) {
            node += m;
            val -= tree[node-1];
        }
    }

    return node - 1;
}
