#ifndef __FENWICK_NAIVE_HPP__
#define __FENWICK_NAIVE_HPP__

#include "../common.hpp"
#include "fenwick_tree.hpp"

namespace hft {
    namespace fenwick {

        /**
         * class FixedF - no compression and classical node layout.
         * @sequence: sequence of integers.
         * @size: number of elements.
         * @LEAF_MAXVAL: maximum value that @sequence can store.
         *
         */
        template<size_t LEAF_MAXVAL>
        class FixedF : public FenwickTree
        {
        public:
            static constexpr size_t LEAF_BITSIZE = log2(LEAF_MAXVAL);
            static_assert(LEAF_BITSIZE >= 1 && LEAF_BITSIZE <= 64, "Leaves can't be stored in a 64-bit word");

        protected:
            DArray<uint64_t> tree;

        public:
            FixedF(uint64_t sequence[], size_t size):
                tree(size)
            {
                std::copy_n(sequence, size, tree.get());

                for (size_t m = 2; m <= size; m <<= 1) {
                    for (size_t idx = m; idx <= size; idx += m)
                        tree[idx - 1] += tree[idx - m/2 - 1];
                }
            }

            virtual uint64_t prefix(size_t idx) const
            {
                uint64_t sum = 0;

                while (idx != 0) {
                    sum += tree[idx - 1];
                    idx = clear_rho(idx);
                }

                return sum;
            }

            virtual void add(size_t idx, int64_t inc)
            {
                const size_t size = tree.size();
                while (idx <= size) {
                    tree[idx-1] += inc;
                    idx += mask_rho(idx);
                }
            }

            using FenwickTree::find;
            virtual size_t find(uint64_t *val) const
            {
                size_t node = 0;

                for (size_t m = mask_lambda(tree.size()); m != 0; m >>= 1) {
                    if (node+m-1 >= tree.size()) continue;

                    uint64_t value = tree[node+m-1];

                    if(*val >= value) {
                        node += m;
                        *val -= value;
                    }
                }

                return node;
            }

            using FenwickTree::compfind;
            virtual size_t compfind(uint64_t *val) const
            {
                size_t node = 0;

                for (size_t m = mask_lambda(tree.size()); m != 0; m >>= 1) {
                    if (node+m-1 >= tree.size()) continue;

                    uint64_t value = (LEAF_MAXVAL << rho(node+m)) - tree[node+m-1];

                    if(*val >= value) {
                        node += m;
                        *val -= value;
                    }
                }

                return node;
            }

            virtual size_t size() const
            {
                return tree.size();
            }

            virtual size_t bit_count() const
            {
                return sizeof(FixedF<LEAF_BITSIZE>)*8
                    + tree.bit_count() - sizeof(tree);
            }
        };

    }
}

#endif // __FENWICK_NAIVE_HPP__
