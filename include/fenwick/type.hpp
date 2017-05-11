#ifndef __FENWICK_TYPE_H__
#define __FENWICK_TYPE_H__

#include "../common.hpp"
#include "fenwick_tree.hpp"

namespace dyn {

    /**
     * class TypeFenwickTree
     * @tree: Fenwick Tree data.
     * @size: Number of elements in the tree.
     * @level: Lookup table, it store every starting level index.
     *
     * Each node is the smallest datatype capable of holding its data.
     * Interleaved version of LTypeFenwickTree.
     */
    template<size_t LEAF_MAXVAL>
    class TypeFenwickTree : public FenwickTree
    {
    public:
        static constexpr size_t LEAF_BITSIZE = log2(LEAF_MAXVAL);
        static_assert(LEAF_BITSIZE >= 1, "A leaf should be at least 1 bit long");
        static_assert(LEAF_BITSIZE <= 55, "A leaf should be at most 55 bit long");

    protected:
        const size_t _size;
        DArray<uint8_t> tree;

    public:
        TypeFenwickTree(uint64_t sequence[], size_t size) :
            _size(size),
            tree(get_bytepos(size))
        {
            for (size_t i = 1; i <= size; i++) {
                const size_t bytepos = get_bytepos(i-1);

                switch (LEAF_BITSIZE + lsb(i)) {
                case 17 ... 64: *reinterpret_cast<auint64_t*>(&tree[bytepos]) = sequence[i-1]; break;
                case  9 ... 16: *reinterpret_cast<auint16_t*>(&tree[bytepos]) = sequence[i-1]; break;
                default:        *reinterpret_cast< auint8_t*>(&tree[bytepos]) = sequence[i-1];
                }
            }

            for (size_t m = 2; m <= size; m <<= 1) {
                for (size_t idx = m; idx <= size; idx += m) {
                    const size_t left = get_bytepos(idx-1);
                    const size_t right = get_bytepos(idx - m/2 - 1);

                    switch (LEAF_BITSIZE + lsb(idx)) {
                    case 17 ... 64:
                        switch (LEAF_BITSIZE + lsb(idx - m/2)) {
                        case 17 ... 64: *reinterpret_cast<auint64_t*>(&tree[left]) += *reinterpret_cast<auint64_t*>(&tree[right]); break;
                        case  9 ... 16: *reinterpret_cast<auint64_t*>(&tree[left]) += *reinterpret_cast<auint16_t*>(&tree[right]); break;
                        default:        *reinterpret_cast<auint64_t*>(&tree[left]) += *reinterpret_cast< auint8_t*>(&tree[right]);
                        }
                        break;
                    case  9 ... 16:
                        switch (LEAF_BITSIZE + lsb(idx - m/2)) {
                        case  9 ... 16: *reinterpret_cast<auint16_t*>(&tree[left]) += *reinterpret_cast<auint16_t*>(&tree[right]); break;
                        default:        *reinterpret_cast<auint16_t*>(&tree[left]) += *reinterpret_cast< auint8_t*>(&tree[right]);
                        }
                        break;
                    default:
                        *reinterpret_cast< auint8_t*>(&tree[left]) += *reinterpret_cast< auint8_t*>(&tree[right]);
                    }
                }
            }
        }

        virtual uint64_t get(size_t idx) const
        {
            uint64_t sum = 0;

            // NOTA: nelle versioni interleaved il for decrescete con drop_first_set è più veloce
            // nelle versioni level-ordered il for crescente con mask_last_set è più veloce.
            // TODO: rinominare mask_last_set e altra roba in broadword?
            for (idx = idx+1; idx != 0; idx = drop_first_set(idx)) {
                const size_t bytepos = get_bytepos(idx-1);

                switch (LEAF_BITSIZE + lsb(idx)) {
                case 17 ... 64: sum += *reinterpret_cast<auint64_t*>(&tree[bytepos]); break;
                case  9 ... 16: sum += *reinterpret_cast<auint16_t*>(&tree[bytepos]); break;
                default:        sum += *reinterpret_cast< auint8_t*>(&tree[bytepos]);
                }
            }

            return sum;
        }

        virtual void set(size_t idx, int64_t inc)
        {
            for (idx = idx+1; idx <= size(); idx += mask_first_set(idx)) {
                const size_t bytepos = get_bytepos(idx-1);

                switch (LEAF_BITSIZE + lsb(idx)) {
                case 17 ... 64: *reinterpret_cast<auint64_t*>(&tree[bytepos]) += inc; break;
                case  9 ... 16: *reinterpret_cast<auint16_t*>(&tree[bytepos]) += inc; break;
                default:        *reinterpret_cast< auint8_t*>(&tree[bytepos]) += inc;
                }
            }
        }

        virtual size_t find(uint64_t val) const
        {
            size_t node = 0;

            for (size_t m = mask_last_set(size()); m != 0; m >>= 1) {
                if (node+m-1 >= size()) continue;

                const size_t bytepos = get_bytepos(node+m-1);
                const int bitlen = LEAF_BITSIZE + lsb(node+m);

                uint64_t value;
                switch (bitlen) {
                case 17 ... 64: value = *reinterpret_cast<auint64_t*>(&tree[bytepos]); break;
                case  9 ... 16: value = *reinterpret_cast<auint16_t*>(&tree[bytepos]); break;
                default:        value = *reinterpret_cast< auint8_t*>(&tree[bytepos]);
                }

                if (val >= value) {
                    node += m;
                    val -= value;
                }
            }

            return node - 1;
        }

        virtual size_t find_complement(uint64_t val) const
        {
            size_t node = 0;

            for (size_t m = mask_last_set(size()); m != 0; m >>= 1) {
                if (node+m-1 >= size()) continue;

                const size_t bytepos = get_bytepos(node+m-1);
                const int height = lsb(node+m);

                uint64_t value = LEAF_MAXVAL << height;
                switch (LEAF_BITSIZE + height) {
                case 17 ... 64: value -= *reinterpret_cast<auint64_t*>(&tree[bytepos]); break;
                case  9 ... 16: value -= *reinterpret_cast<auint16_t*>(&tree[bytepos]); break;
                default:        value -= *reinterpret_cast< auint8_t*>(&tree[bytepos]);
                }

                if (val >= value) {
                    node += m;
                    val -= value;
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
            return sizeof(TypeFenwickTree<LEAF_BITSIZE>)*8
                + tree.bit_count() - sizeof(tree);
        }

    private:
        inline size_t get_bytepos(size_t n) const
        {
                return n
                    + (n >> (LEAF_BITSIZE <=  8 ? ( 8 - LEAF_BITSIZE + 1) : 0))
                    + (n >> (LEAF_BITSIZE <= 16 ? (16 - LEAF_BITSIZE + 1) : 0)) * 6;
        }

    };

}

#endif // __FENWICK_TYPE_H__
