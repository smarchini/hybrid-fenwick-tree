#ifndef __FENWICK_LNAIVE_HPP__
#define __FENWICK_LNAIVE_HPP__

#include "fenwick_tree.hpp"

namespace hft {
    namespace fenwick {

        /**
         * class FixedL - no compression and level-ordered node layout.
         * @sequence: sequence of integers.
         * @size: number of elements.
         * @LEAF_MAXVAL: maximum value that @sequence can store.
         *
         */
        template<size_t LEAF_MAXVAL>
        class FixedL : public FenwickTree
        {
        public:
            static constexpr size_t LEAF_BITSIZE = log2(LEAF_MAXVAL);
            static_assert(LEAF_BITSIZE >= 1 && LEAF_BITSIZE <= 64, "Leaves can't be stored in a 64-bit word");

        protected:
            DArray<uint64_t> tree;
            DArray<size_t> level;

        public:
            FixedL(uint64_t sequence[], size_t size):
                level(lambda(size) + 2),
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

            virtual uint64_t prefix(size_t idx) const
            {
                uint64_t sum = 0;

                while (idx != 0) {
                    const int height = rho(idx);
                    const size_t level_idx = idx >> (1 + height);
                    sum += tree[level[height] + level_idx];

                    idx = clear_rho(idx);
                }

                return sum;
            }

            virtual void add(size_t idx, int64_t inc)
            {
                while (idx <= size()) {
                    const int height = rho(idx);
                    const size_t level_idx = idx >> (1 + height);
                    tree[level[height] + level_idx] += inc;

                    idx += mask_rho(idx);
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

                return min(node, size());
            }

            using FenwickTree::compfind;
            virtual size_t compfind(uint64_t *val) const
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

                return min(node, size());
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
}

#endif // __FENWICK_LNAIVE_HPP__
