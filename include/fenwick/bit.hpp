#ifndef __FENWICK_BIT_H__
#define __FENWICK_BIT_H__

#include "../common.hpp"
#include "fenwick_tree.hpp"

namespace dyn {

   /** TODO: commenti
    * class
    */
    template<size_t LEAF_MAXVAL>
    class BitFenwickTree : public FenwickTree
    {
    public:
        static constexpr size_t LEAF_BITSIZE = log2(LEAF_MAXVAL);
        static_assert(LEAF_BITSIZE >= 1, "A leaf should be at least 1 bit long");
        static_assert(LEAF_BITSIZE <= 55, "A leaf should be at most 55 bit long");

    protected:
        const size_t _size;
        DArray<uint8_t> tree;

    public:
        /**
         * BitFenwickTree - Build a FenwickTree given a sequence of increments.
         * @sequence: Sequence of increments.
         * @size: Number of elements stored by the sequence.
         *
         * Running time: O(length)
         */
        BitFenwickTree(uint64_t sequence[], size_t size) :
            _size(size),
            tree(get_bitpos(size-1)/8 + (lsb(size)+LEAF_BITSIZE)/8 + 1 + 4)
        {
            for (size_t i = 1; i <= size; i++) {
                const size_t bitpos = get_bitpos(i-1);
                auint64_t * const element = reinterpret_cast<auint64_t*>(&tree[bitpos/8]);

                const int bitsize = LEAF_BITSIZE + lsb(i);
                const size_t shift = bitpos & 0b111;
                const uint64_t mask = compact_bitmask(bitsize, shift);

                *element &= ~mask;
                *element |= mask & (sequence[i-1] << shift);
            }

            for (size_t m = 2; m <= size; m <<= 1) {
                for (size_t idx = m; idx <= size; idx += m) {
                    const size_t left_bitpos = get_bitpos(idx-1);
                    auint64_t * const left_element = reinterpret_cast<auint64_t*>(&tree[left_bitpos/8]);
                    const size_t left_shift = left_bitpos & 0b111;

                    const size_t right_bitpos = get_bitpos(idx - m/2 - 1);
                    auint64_t * const right_element = reinterpret_cast<auint64_t*>(&tree[right_bitpos/8]);
                    const size_t right_shift = right_bitpos & 0b111;

                    const int right_bitsize = LEAF_BITSIZE + lsb(idx - m/2);
                    const uint64_t right_mask = compact_bitmask(right_bitsize, right_shift);

                    uint64_t value = (right_mask & *right_element) >> right_shift;
                    *left_element += value << left_shift;
                }
            }
        }

        virtual uint64_t get(size_t idx) const
        {
            uint64_t sum = 0;

            for (idx = idx+1; idx != 0; idx = drop_first_set(idx)) {
                const size_t bit_pos = get_bitpos(idx-1);
                const auint64_t * const compact_element = reinterpret_cast<auint64_t*>(&tree[bit_pos/8]);

                const int height = lsb(idx);
                const size_t shift = bit_pos & 0b111;
                const uint64_t mask = compact_bitmask(LEAF_BITSIZE+height, shift);

                sum += (*compact_element & mask) >> shift;
            }

            return sum;
        }

        virtual void set(size_t idx, int64_t inc)
        {
            for (idx = idx+1; idx <= size(); idx += mask_first_set(idx)) {
                const size_t bit_pos = get_bitpos(idx-1);
                auint64_t * const compact_element = reinterpret_cast<auint64_t*>(&tree[bit_pos/8]);

                const size_t shift = bit_pos & 0b111;
                *compact_element += inc << shift;
            }
        }

        using FenwickTree::find;
        virtual size_t find(uint64_t *val) const
        {
            size_t node = 0;

            for (size_t m = mask_last_set(size()); m != 0; m >>= 1) {
                if (node+m-1 >= size()) continue;

                const size_t bit_pos = get_bitpos(node+m-1);
                const int height = lsb(node+m);
                const size_t shift = bit_pos & 0b111;
                const uint64_t mask = compact_bitmask(LEAF_BITSIZE+height, 0);

                uint64_t value = (*reinterpret_cast<auint64_t*>(&tree[bit_pos/8]) >> shift) & mask;

                if (*val >= value) {
                    node += m;
                    *val -= value;
                }
            }

            return node - 1;
        }

        using FenwickTree::find_complement;
        virtual size_t find_complement(uint64_t *val) const
        {
            size_t node = 0;

            for (size_t m = mask_last_set(size()); m != 0; m >>= 1) {
                if (node+m-1 >= size()) continue;

                const size_t bit_pos = get_bitpos(node+m-1);
                const int height = lsb(node+m);
                const size_t shift = bit_pos & 0b111;
                const uint64_t mask = compact_bitmask(LEAF_BITSIZE+height, 0);

                uint64_t value = (LEAF_MAXVAL << height)
                    - ((*reinterpret_cast<auint64_t*>(&tree[bit_pos/8]) >> shift) & mask);

                if (*val >= value) {
                    node += m;
                    *val -= value;
                }
            }

            return node - 1;
        }

        virtual size_t size() const
        {
            return _size;
        }

        virtual size_t bit_count() const
        {
            return sizeof(BitFenwickTree<LEAF_BITSIZE>)*8
                + tree.bit_count() - sizeof(tree);
        }

    private:
        inline size_t get_bitpos(size_t n) const
        {
            return (LEAF_BITSIZE+1)*n - popcount(n);
        }

    };

}

#endif // __FENWICK_BIT_H__
