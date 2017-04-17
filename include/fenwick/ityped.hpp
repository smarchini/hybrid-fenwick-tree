#ifndef __ITYPED_FENWICK_TREE_H__
#define __ITYPED_FENWICK_TREE_H__

#include "../common.hpp"
#include "fenwick_tree.hpp"
#include <iostream>

namespace dyn {

    /**
     * class ITypedFenwickTree
     * @tree: Fenwick Tree data.
     * @size: Number of elements in the tree.
     * @level: Lookup table, it store every starting level index.
     *
     * Each node is the smallest datatype capable of holding its data.
     * Interleaved version of TypedFenwickTree.
     */
    template<size_t LEAF_BITSIZE>
    class ITypedFenwickTree : public FenwickTree
    {
    protected:
        const size_t size;
        DArray<uint8_t> tree;

    public:
        ITypedFenwickTree(uint64_t sequence[], size_t size) :
            size(size),
            tree(get_bytepos(size-1) + (lsb(size-1)+LEAF_BITSIZE)/8 + 1 + 4)
        {
            for (size_t i = 1; i <= size; i++) {
                const size_t bytepos = get_bytepos(i-1);

                switch (LEAF_BITSIZE + lsb(i)) {
                case 33 ... 64: *reinterpret_cast<auint64_t*>(&tree[bytepos]) = sequence[i-1]; break;
                case 17 ... 32: *reinterpret_cast<auint32_t*>(&tree[bytepos]) = sequence[i-1]; break;
                case  9 ... 16: *reinterpret_cast<auint16_t*>(&tree[bytepos]) = sequence[i-1]; break;
                default:        *reinterpret_cast< auint8_t*>(&tree[bytepos]) = sequence[i-1];
                }
            }

            for (size_t m = 2; m <= size; m <<= 1) {
                for (size_t idx = m; idx <= size; idx += m) {
                    const size_t left = get_bytepos(idx-1);
                    const size_t right = get_bytepos(idx - m/2 - 1);

                    switch (LEAF_BITSIZE + lsb(idx)) {
                    case 33 ... 64:
                        switch (LEAF_BITSIZE + lsb(idx - m/2)) {
                        case 33 ... 64: *reinterpret_cast<auint64_t*>(&tree[left]) += *reinterpret_cast<auint64_t*>(&tree[right]); break;
                        case 17 ... 32: *reinterpret_cast<auint64_t*>(&tree[left]) += *reinterpret_cast<auint32_t*>(&tree[right]); break;
                        case  9 ... 16: *reinterpret_cast<auint64_t*>(&tree[left]) += *reinterpret_cast<auint16_t*>(&tree[right]); break;
                        default:        *reinterpret_cast<auint64_t*>(&tree[left]) += *reinterpret_cast< auint8_t*>(&tree[right]);
                        }
                        break;
                    case 17 ... 32:
                        switch (LEAF_BITSIZE + lsb(idx - m/2)) {
                        case 33 ... 64: *reinterpret_cast<auint32_t*>(&tree[left]) += *reinterpret_cast<auint64_t*>(&tree[right]); break;
                        case 17 ... 32: *reinterpret_cast<auint32_t*>(&tree[left]) += *reinterpret_cast<auint32_t*>(&tree[right]); break;
                        case  9 ... 16: *reinterpret_cast<auint32_t*>(&tree[left]) += *reinterpret_cast<auint16_t*>(&tree[right]); break;
                        default:        *reinterpret_cast<auint32_t*>(&tree[left]) += *reinterpret_cast< auint8_t*>(&tree[right]);
                        }
                        break;
                    case  9 ... 16:
                        switch (LEAF_BITSIZE + lsb(idx - m/2)) {
                        case 33 ... 64: *reinterpret_cast<auint16_t*>(&tree[left]) += *reinterpret_cast<auint64_t*>(&tree[right]); break;
                        case 17 ... 32: *reinterpret_cast<auint16_t*>(&tree[left]) += *reinterpret_cast<auint32_t*>(&tree[right]); break;
                        case  9 ... 16: *reinterpret_cast<auint16_t*>(&tree[left]) += *reinterpret_cast<auint16_t*>(&tree[right]); break;
                        default:        *reinterpret_cast<auint16_t*>(&tree[left]) += *reinterpret_cast< auint8_t*>(&tree[right]);
                        }
                        break;
                    default:
                        switch (LEAF_BITSIZE + lsb(idx - m/2)) {
                        case 33 ... 64: *reinterpret_cast<auint8_t*>(&tree[left]) += *reinterpret_cast<auint64_t*>(&tree[right]); break;
                        case 17 ... 32: *reinterpret_cast<auint8_t*>(&tree[left]) += *reinterpret_cast<auint32_t*>(&tree[right]); break;
                        case  9 ... 16: *reinterpret_cast<auint8_t*>(&tree[left]) += *reinterpret_cast<auint16_t*>(&tree[right]); break;
                        default:        *reinterpret_cast<auint8_t*>(&tree[left]) += *reinterpret_cast< auint8_t*>(&tree[right]);
                        }
                    }
                }
            }
        }

        virtual uint64_t get(size_t idx) const
        {
            uint64_t sum = 0ULL;

            // NOTA: nelle versioni interleaved il for decrescete con drop_first_set è più veloce
            // nelle versioni level-ordered il for crescente con mask_last_set è più veloce.
            for (idx = idx+1; idx != 0; idx = drop_first_set(idx)) {
                const size_t bytepos = get_bytepos(idx-1);

                switch (LEAF_BITSIZE + lsb(idx)) {
                case 33 ... 64: sum += *reinterpret_cast<auint64_t*>(&tree[bytepos]); break;
                case 17 ... 32: sum += *reinterpret_cast<auint32_t*>(&tree[bytepos]); break;
                case  9 ... 16: sum += *reinterpret_cast<auint16_t*>(&tree[bytepos]); break;
                default:        sum += *reinterpret_cast< auint8_t*>(&tree[bytepos]);
                }
            }

            return sum;
        }

        virtual void set(size_t idx, int64_t inc)
        {
            for (idx = idx+1; idx <= size; idx += mask_first_set(idx)) {
                const size_t bytepos = get_bytepos(idx-1);

                switch (LEAF_BITSIZE + lsb(idx)) {
                case 33 ... 64: *reinterpret_cast<auint64_t*>(&tree[bytepos]) += inc; break;
                case 17 ... 32: *reinterpret_cast<auint32_t*>(&tree[bytepos]) += inc; break;
                case  9 ... 16: *reinterpret_cast<auint16_t*>(&tree[bytepos]) += inc; break;
                default:        *reinterpret_cast< auint8_t*>(&tree[bytepos]) += inc;
                }
            }
        }

        virtual size_t find(uint64_t val, bool complement=false) const
        {
            size_t node = 0;
            const size_t byte_max = get_bytepos(size-1) + (lsb(size-1)+LEAF_BITSIZE)/8 + 1;

            for (size_t m = mask_last_set(size); m != 0; m >>= 1) {
                const size_t bytepos = get_bytepos(node+m-1);
                const int bitlen = LEAF_BITSIZE + lsb(node+m);

                uint64_t value = 0;
                if (bytepos >= byte_max) value = -1ULL;
                else {
                    switch (bitlen) {
                    case 33 ... 64: value = *reinterpret_cast<auint64_t*>(&tree[bytepos]); break;
                    case 17 ... 32: value = *reinterpret_cast<auint32_t*>(&tree[bytepos]); break;
                    case  9 ... 16: value = *reinterpret_cast<auint16_t*>(&tree[bytepos]); break;
                    default:        value = *reinterpret_cast< auint8_t*>(&tree[bytepos]);
                    }
                }

                if (complement)
                    value = (1ULL << (bitlen-1)) - value;

                if (val >= value) {
                    node += m;
                    val -= value;
                }
            }

            return node - 1;
        }


        virtual size_t bit_count() const
        {
            return sizeof(ITypedFenwickTree<LEAF_BITSIZE>)*8 + get_bytepos(size)*8;
        }

    private:
        inline size_t get_bytepos(size_t n) const
        {
            // C++17: constexpr if statement http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0292r2.html
            // TODO: controllare se gli shift > lg2(n) sono effettivamente leciti o è undefined behavior
            if (LEAF_BITSIZE <= 8) {
                return n
                    + (n >> ( 8 - LEAF_BITSIZE + 1))
                    + (n >> (16 - LEAF_BITSIZE + 1)) * 2
                    + (n >> (32 - LEAF_BITSIZE + 1)) * 4;
            }
            else if (LEAF_BITSIZE <= 16) {
                return n*2
                    + (n >> (16 - LEAF_BITSIZE + 1)) * 2
                    + (n >> (32 - LEAF_BITSIZE + 1)) * 4;
            }
            else if (LEAF_BITSIZE <= 32) {
                return n*4
                    + (n >> (32 - LEAF_BITSIZE + 1)) * 4;
            }
            else {
                return n*8;
            }
        }

    };

}

#endif // __ITYPED_FENWICK_TREE_H__
