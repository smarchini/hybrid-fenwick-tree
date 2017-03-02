#include "../include/simple_fenwick_tree.hpp"
#include "../include/broadword.hpp"
#include <algorithm>


SimpleFenwickTree::SimpleFenwickTree(std::uint64_t sequence[], std::size_t size) :
    tree(new std::uint64_t[size]),
    size(size)
{
    std::copy_n(sequence, size, tree);

    std::size_t m = 2;

    while (m <= size) {
        for (std::size_t idx = m; idx <= size; idx += m)
            tree[idx - 1] += tree[idx - m/2 - 1];

        m <<= 1;
    }
}


SimpleFenwickTree::~SimpleFenwickTree()
{
    delete[] tree;
}


std::uint64_t SimpleFenwickTree::get(std::size_t idx) const
{
    int sum = 0;
    idx++;

    while (idx != 0) {
        sum += tree[idx - 1];
        idx = drop_first_set(idx);
    }

    return sum;
}


void SimpleFenwickTree::set(std::size_t idx, std::uint64_t inc)
{
    idx++;

    do {
        tree[idx-1] += inc;
        idx += mask_first_set(idx);
    } while (idx <= size);
}


std::uint64_t SimpleFenwickTree::find(std::uint64_t val) const
{
    std::size_t node = 0;
    std::size_t m = mask_last_set(size);

    while (m != 0) {
        if (val >= tree[node+m-1]) {
            node += m;
            val -= tree[node-1];
        }
        m >>= 1;
    }

    return node - 1;
}
