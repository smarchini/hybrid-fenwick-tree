#ifndef __FENWICK_NAIVE_H__
#define __FENWICK_NAIVE_H__

#include "../common.hpp"
#include "fenwick_tree.hpp"

namespace dyn {

   /**
    * class NaiveFenwickTree - Naive implementation of a Fenwick Tree.
    * @tree: Fenwick Tree data.
    * @size: Number of elements.
    *
    * Typical implementation of a FenwickTree data structure as described by the
    * original paper.
    */
    template<size_t LEAF_BITSIZE>
    class NaiveFenwickTree : public FenwickTree
    {
        static_assert(LEAF_BITSIZE >= 1, "A leaf should be at least 1 bit long");
        static_assert(LEAF_BITSIZE <= 64, "A leaf should be at most 64 bit long");

    protected:
        DArray<uint64_t> tree;

    public:
        /**
         * NaiveFenwickTree - Build a FenwickTree given a sequence of increments.
         * @sequence: Sequence of increments to compress.
         * @length: Number of elements stored by the sequence.
         *
         * Running time: O(@length)
         */
        NaiveFenwickTree(uint64_t sequence[], size_t size):
            tree(size)
        {
            std::copy_n(sequence, size, tree.get());

            for (size_t m = 2; m <= size; m <<= 1) {
                for (size_t idx = m; idx <= size; idx += m)
                    tree[idx - 1] += tree[idx - m/2 - 1];
            }
        }

        virtual uint64_t get(size_t idx) const
        {
            uint64_t sum = 0;

            for (idx = idx+1; idx != 0; idx = drop_first_set(idx))
                sum += tree[idx - 1];

            return sum;
        }

        virtual void set(size_t idx, int64_t inc)
        {
            const size_t size = tree.size();
            for (idx = idx+1; idx <= size; idx += mask_first_set(idx))
                tree[idx-1] += inc;
        }

        virtual size_t find(uint64_t val) const
        {
            size_t node = 0;

            for (size_t m = mask_last_set(tree.size()); m != 0; m >>= 1) {
                if (node+m-1 >= tree.size()) continue;

                uint64_t value = tree[node+m-1];

                if(val >= value) {
                    node += m;
                    val -= value;
                }
            }

            return node - 1;
        }

        virtual size_t find_complement(uint64_t val) const
        {
            size_t node = 0;

            for (size_t m = mask_last_set(tree.size()); m != 0; m >>= 1) {
                if (node+m-1 >= tree.size()) continue;

                uint64_t value = (1ULL << (LEAF_BITSIZE + lsb(m) - 1)) - tree[node+m-1];

                if(val >= value) {
                    node += m;
                    val -= value;
                }
            }

            return node - 1;
        }

        virtual size_t bit_count() const
        {
            return sizeof(NaiveFenwickTree<LEAF_BITSIZE>)*8
                + tree.bit_count() - sizeof(tree);
        }
    };

}

#endif // __FENWICK_NAIVE_H__
