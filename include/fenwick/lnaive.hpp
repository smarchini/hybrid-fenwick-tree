#ifndef __FENWICK_LNAIVE_H__
#define __FENWICK_LNAIVE_H__

#include "../common.hpp"
#include "fenwick_tree.hpp"

namespace dyn {

   /**
    * class FixedL - Level-order version of FixedF
    * @tree: Fenwick Tree data.
    * @size: Number of elements.
    *
    */
    template<size_t LEAF_MAXVAL>
    class FixedL : public FenwickTree
    {
    public:
        static constexpr size_t LEAF_BITSIZE = log2(LEAF_MAXVAL);
        static_assert(LEAF_BITSIZE >= 1, "A leaf should be at least 1 bit long");
        static_assert(LEAF_BITSIZE <= 64, "A leaf should be at most 64 bit long");

    protected:
        DArray<uint64_t> tree;
        DArray<size_t> level;

    public:
        /**
         * FixedL - Build a FenwickTree given a sequence of increments.
         * @sequence: Sequence of increments to compress.
         * @length: Number of elements stored by the sequence.
         *
         * Running time: O(@length)
         */
        FixedL(uint64_t sequence[], size_t size):
            level(msb(size) + 2),
            tree(size)
        {
            level[0] = 0;
            for (size_t i = 1; i < level.size(); i++)
                level[i] = ((size + (1<<(i-1))) / (1<<i)) + level[i-1];

            const size_t levels = level.size() - 1;
            for (size_t l = 0; l < levels; l++) {
                for (size_t node = 1<<l; node <= size; node += 1 << (l+1)) {
                    size_t sequence_idx = node-1;
                    uint64_t value = sequence[sequence_idx];
                    for (size_t j = 0; j < l; j++) {
                        sequence_idx >>= 1;
                        value += tree[level[j] + sequence_idx];
                    }

                    tree[level[l] + (node >> (l+1))] = value;
                }
            }
        }

        virtual uint64_t get(size_t idx) const
        {
            uint64_t sum = 0;
            size_t index = 0;

            for (idx++; idx != index;) {
                index += mask_last_set(idx ^ index);

                const int height = lsb(index);
                const size_t level_idx = index >> (1 + height);
                sum += tree[level[height] + level_idx];
            }

            return sum;
        }

        virtual void set(size_t idx, int64_t inc)
        {
            for (idx = idx+1; idx <= size(); idx += mask_first_set(idx)) {
                const int height = lsb(idx);
                const size_t level_idx = idx >> (1 + height);
                tree[level[height] + level_idx] += inc;
            }
        }

        using FenwickTree::find;
        virtual size_t find(uint64_t *val) const
        {
            size_t node = 0, idx = 0;

            for (size_t height = level.size() - 2; height != SIZE_MAX; height--) {
                const size_t pos = level[height] + idx;

                idx <<= 1;

                if (pos >= level[height+1]) continue;

                uint64_t value = tree[pos];
                if (*val >= value) {
                    idx++;
                    *val -= value;
                    node += 1 << height;
                }
            }

            return node <= size() ? node-1 : size()-1;
        }

        using FenwickTree::find_complement;
        virtual size_t find_complement(uint64_t *val) const
        {
            size_t node = 0, idx = 0;

            for (size_t height = level.size() - 2; height != SIZE_MAX; height--) {
                const size_t pos = level[height] + idx;

                idx <<= 1;

                if (pos >= level[height+1]) continue;

                uint64_t value = (LEAF_MAXVAL << height) - tree[pos];
                if(*val >= value) {
                    idx++;
                    *val -= value;
                    node += 1 << height;
                }
            }

            return node <= size() ? node-1 : size()-1;
        }

        virtual size_t size() const
        {
            return tree.size();
        }

        virtual size_t bit_count() const
        {
            return sizeof(FixedL<LEAF_BITSIZE>)*8
                + tree.bit_count() - sizeof(tree);
        }
    };

}

#endif // __FENWICK_LNAIVE_H__
