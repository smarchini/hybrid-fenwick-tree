#ifndef __FENWICK_BYTE_H__
#define __FENWICK_BYTE_H__

#include "../common.hpp"
#include "fenwick_tree.hpp"

namespace dyn {

   /**
    * class ByteF
    * @tree: Fenwick Tree data.
    * @size: Number of elements in the tree.
    * @level: Lookup table, it store the starting bit-position of each level.
    *
    * Each node have the smallest byte-size capable of holding its data. It's the
    * interleaved version of ByteF.
    */
    template<size_t LEAF_MAXVAL>
    class ByteF : public FenwickTree
    {
    public:
        static constexpr size_t LEAF_BITSIZE = log2(LEAF_MAXVAL);
        static_assert(LEAF_BITSIZE >= 1, "A leaf should be at least 1 bit long");
        static_assert(LEAF_BITSIZE <= 56, "A leaf should be at most 56 bit long");

    protected:
        const size_t _size;
        DArray<uint8_t> tree;

    public:
        /**
         * ByteF - Build a FenwickTree given a sequence of increments.
         * @sequence: Sequence of increments.
         * @size: Number of elements stored by the sequence.
         *
         * Running time: O(length)
         */
        ByteF(uint64_t sequence[], size_t size) :
            _size(size),
            tree(get_bytepos(size))
        {
            for (size_t i = 1; i <= size; i++) {
                auint64_t * const element = reinterpret_cast<auint64_t*>(&tree[get_bytepos(i-1)]);

                const size_t bytesize = get_bytesize(i);
                *element &= ~BYTE_MASK[bytesize];
                *element |= sequence[i-1] & BYTE_MASK[bytesize];
            }

            for (size_t m = 2; m <= size; m <<= 1) {
                for (size_t idx = m; idx <= size; idx += m) {
                    auint64_t * const left_element = reinterpret_cast<auint64_t*>(&tree[get_bytepos(idx-1)]);
                    auint64_t * const right_element = reinterpret_cast<auint64_t*>(&tree[get_bytepos(idx - m/2 - 1)]);

                    uint64_t value = *right_element & BYTE_MASK[get_bytesize(idx - m/2)];
                    *left_element += value;
                }
            }
        }

        virtual uint64_t prefix(size_t idx) const
        {
            uint64_t sum = 0;

            for (idx = idx+1; idx != 0; idx = drop_first_set(idx)) {
                const auint64_t * const element = reinterpret_cast<auint64_t*>(&tree[get_bytepos(idx-1)]);
                sum += *element & BYTE_MASK[get_bytesize(idx)];
            }

            return sum;
        }

        virtual void set(size_t idx, int64_t inc)
        {
            for (idx = idx+1; idx <= size(); idx += mask_first_set(idx))
                *reinterpret_cast<auint64_t*>(&tree[get_bytepos(idx-1)]) += inc;
        }

        using FenwickTree::find;
        virtual size_t find(uint64_t *val) const
        {
            size_t node = 0;

            for (size_t m = mask_last_set(size()); m != 0; m >>= 1) {
                if (node+m-1 >= size()) continue;

                uint64_t value = *reinterpret_cast<auint64_t*>(&tree[get_bytepos(node+m-1)]) & BYTE_MASK[get_bytesize(node+m)];

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

                uint64_t value = (LEAF_MAXVAL << lsb(node+m))
                    - (*reinterpret_cast<auint64_t*>(&tree[get_bytepos(node+m-1)]) & BYTE_MASK[get_bytesize(node+m)]);

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
            return sizeof(ByteF<LEAF_BITSIZE>)*8
                + tree.bit_count() - sizeof(tree);
        }

    private:
        static inline size_t get_bytesize(size_t n)
        {
            return (lsb(n) + LEAF_BITSIZE - 1) / 8 + 1;
        }

        static inline size_t get_bytepos(size_t idx)
        {
            return idx
                + (idx >> (LEAF_BITSIZE <=  8 ? ( 8 - LEAF_BITSIZE + 1) : 0))
                + (idx >> (LEAF_BITSIZE <= 16 ? (16 - LEAF_BITSIZE + 1) : 0)) * 6;
        }

    };

}

#endif // __FENWICK_BYTE_H__
