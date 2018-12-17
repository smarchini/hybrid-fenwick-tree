#ifndef __FENWICK_BIT_HPP__
#define __FENWICK_BIT_HPP__

#include "fenwick_tree.hpp"
#include <iostream>

namespace hft {
    namespace fenwick {

        /**
         * class BitF - bit compression and classical node layout.
         * @sequence: sequence of integers.
         * @size: number of elements.
         * @LEAF_MAXVAL: maximum value that @sequence can store.
         *
         */
        template<size_t LEAF_MAXVAL>
        class BitF : public FenwickTree
        {
        public:
            static constexpr size_t LEAF_BITSIZE = log2(LEAF_MAXVAL);
            static_assert(LEAF_BITSIZE >= 1 && LEAF_BITSIZE <= 64, "Leaves can't be stored in a 64-bit word");

        protected:
            const size_t _size;
            DArray<uint8_t> tree;

        public:
            BitF(uint64_t sequence[], size_t size) :
                _size(size),
                tree((get_bitpos(size) >> 3) + 8) // +8 to prevent segfault the last element
            {
                for (size_t i = 1; i <= size; i++) {
                    const size_t bitpos = get_bitpos(i-1);
                    auint64_t &element = reinterpret_cast<auint64_t&>(tree[bitpos >> 3]);

                    const int bitsize = LEAF_BITSIZE + rho(i);
                    const size_t shift = bitpos & 0b111;
                    const uint64_t mask = ((1ULL << bitsize) - 1) << shift;

                    element &= ~mask;
                    element |= mask & (sequence[i-1] << shift);
                }

                for (size_t m = 2; m <= size; m <<= 1) {
                    for (size_t idx = m; idx <= size; idx += m) {
                        const size_t elem_bitpos = get_bitpos(idx-1);
                        auint64_t &element = reinterpret_cast<auint64_t&>(tree[elem_bitpos >> 3]);

                        const size_t subtree_bitpos = get_bitpos(idx - m/2 - 1);
                        const auint64_t subtree = *reinterpret_cast<auint64_t*>(&tree[subtree_bitpos >> 3]);
                        const uint64_t value = bitextract(subtree, subtree_bitpos & 0b111, LEAF_BITSIZE + rho(idx - m/2));

                        element += value << (elem_bitpos & 0b111);
                    }
                }
            }

            virtual uint64_t prefix(size_t idx) const
            {
                uint64_t sum = 0;

                while (idx != 0) {
                    const size_t bit_pos = get_bitpos(idx-1);
                    const uint64_t element = *reinterpret_cast<auint64_t*>(&tree[bit_pos >> 3]);

                    sum += bitextract(element, bit_pos & 0b111, LEAF_BITSIZE + rho(idx));
                    idx = clear_rho(idx);
                }

                return sum;
            }

            virtual void add(size_t idx, int64_t inc)
            {
                while (idx <= size()) {
                    const size_t bit_pos = get_bitpos(idx-1);
                    auint64_t &element = reinterpret_cast<auint64_t&>(tree[bit_pos >> 3]);

                    element += inc << (bit_pos & 0b111);

                    idx += mask_rho(idx);
                }
            }

            using FenwickTree::find;
            virtual size_t find(uint64_t *val) const
            {
                size_t node = 0;

                for (size_t m = mask_lambda(size()); m != 0; m >>= 1) {
                    if (node+m-1 >= size()) continue;

                    const size_t bit_pos = get_bitpos(node+m-1);
                    const uint64_t element = *reinterpret_cast<auint64_t*>(&tree[bit_pos >> 3]);

                    uint64_t value = bitextract(element, bit_pos & 0b111, LEAF_BITSIZE + rho(node+m));

                    if (*val >= value) {
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

                for (size_t m = mask_lambda(size()); m != 0; m >>= 1) {
                    if (node+m-1 >= size()) continue;

                    const int height = rho(node+m);
                    const size_t bit_pos = get_bitpos(node+m-1);
                    const uint64_t element = *reinterpret_cast<auint64_t*>(&tree[bit_pos >> 3]);

                    uint64_t value = (LEAF_MAXVAL << height)
                        - bitextract(element, bit_pos & 0b111, LEAF_BITSIZE + height);

                    if (*val >= value) {
                        node += m;
                        *val -= value;
                    }
                }

                return node;
            }

            virtual size_t size() const
            {
                return _size;
            }

            virtual size_t bit_count() const
            {
                return sizeof(BitF<LEAF_BITSIZE>) * 8
                    + tree.bit_count() - sizeof(tree);
            }

        private:
            inline size_t get_bitpos(size_t n) const
            {
                return (LEAF_BITSIZE+1)*n - popcount(n);
            }

        };

    }
}

#endif // __FENWICK_BIT_HPP__
