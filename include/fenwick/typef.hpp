#ifndef __FENWICK_TYPE_HPP__
#define __FENWICK_TYPE_HPP__

#include "fenwick_tree.hpp"

namespace hft {
    namespace fenwick {

        /**
         * class TypeF - closer type compression and classical node layout.
         * @sequence: sequence of integers.
         * @size: number of elements.
         * @LEAF_MAXVAL: maximum value that @sequence can store.
         *
         */
        template<size_t LEAF_MAXVAL>
        class TypeF : public FenwickTree
        {
        public:
            static constexpr size_t LEAF_BITSIZE = log2(LEAF_MAXVAL);
            static_assert(LEAF_BITSIZE >= 1 && LEAF_BITSIZE <= 64, "Leaves can't be stored in a 64-bit word");

        protected:
            const size_t _size;
            DArray<uint8_t> tree;

        public:
            TypeF(uint64_t sequence[], size_t size) :
                _size(size),
                tree(get_bytepos(size))
            {
                for (size_t i = 1; i <= size; i++) {
                    const size_t bytepos = get_bytepos(i-1);

                    switch (LEAF_BITSIZE + rho(i)) {
                    case 17 ... 64: reinterpret_cast<auint64_t&>(tree[bytepos]) = sequence[i-1]; break;
                    case  9 ... 16: reinterpret_cast<auint16_t&>(tree[bytepos]) = sequence[i-1]; break;
                    default:        reinterpret_cast< auint8_t&>(tree[bytepos]) = sequence[i-1];
                    }
                }

                for (size_t m = 2; m <= size; m <<= 1) {
                    for (size_t idx = m; idx <= size; idx += m) {
                        const size_t left = get_bytepos(idx-1);
                        const size_t right = get_bytepos(idx - m/2 - 1);

                        switch (LEAF_BITSIZE + rho(idx)) {
                        case 17 ... 64:
                            switch (LEAF_BITSIZE + rho(idx - m/2)) {
                            case 17 ... 64: reinterpret_cast<auint64_t&>(tree[left]) += *reinterpret_cast<auint64_t*>(&tree[right]); break;
                            case  9 ... 16: reinterpret_cast<auint64_t&>(tree[left]) += *reinterpret_cast<auint16_t*>(&tree[right]); break;
                            default:        reinterpret_cast<auint64_t&>(tree[left]) += *reinterpret_cast< auint8_t*>(&tree[right]);
                            }
                            break;
                        case  9 ... 16:
                            switch (LEAF_BITSIZE + rho(idx - m/2)) {
                            case  9 ... 16: reinterpret_cast<auint16_t&>(tree[left]) += *reinterpret_cast<auint16_t*>(&tree[right]); break;
                            default:        reinterpret_cast<auint16_t&>(tree[left]) += *reinterpret_cast< auint8_t*>(&tree[right]);
                            }
                            break;
                        default:
                            reinterpret_cast< auint8_t&>(tree[left]) += *reinterpret_cast< auint8_t*>(&tree[right]);
                        }
                    }
                }
            }

            virtual uint64_t prefix(size_t idx) const
            {
                uint64_t sum = 0;

                while (idx != 0) {
                    const size_t bytepos = get_bytepos(idx-1);

                    switch (LEAF_BITSIZE + rho(idx)) {
                    case 17 ... 64: sum += *reinterpret_cast<auint64_t*>(&tree[bytepos]); break;
                    case  9 ... 16: sum += *reinterpret_cast<auint16_t*>(&tree[bytepos]); break;
                    default:        sum += *reinterpret_cast< auint8_t*>(&tree[bytepos]);
                    }

                    idx = clear_rho(idx);
                }

                return sum;
            }

            virtual void add(size_t idx, int64_t inc)
            {
                while (idx <= size()) {
                    const size_t bytepos = get_bytepos(idx-1);

                    switch (LEAF_BITSIZE + rho(idx)) {
                    case 17 ... 64: *reinterpret_cast<auint64_t*>(&tree[bytepos]) += inc; break;
                    case  9 ... 16: *reinterpret_cast<auint16_t*>(&tree[bytepos]) += inc; break;
                    default:        *reinterpret_cast< auint8_t*>(&tree[bytepos]) += inc;
                    }

                    idx += mask_rho(idx);
                }
            }

            using FenwickTree::find;
            virtual size_t find(uint64_t *val) const
            {
                size_t node = 0;

                for (size_t m = mask_lambda(size()); m != 0; m >>= 1) {
                    if (node+m-1 >= size()) continue;

                    const size_t bytepos = get_bytepos(node+m-1);
                    const int bitlen = LEAF_BITSIZE + rho(node+m);

                    uint64_t value;
                    switch (bitlen) {
                    case 17 ... 64: value = *reinterpret_cast<auint64_t*>(&tree[bytepos]); break;
                    case  9 ... 16: value = *reinterpret_cast<auint16_t*>(&tree[bytepos]); break;
                    default:        value = *reinterpret_cast< auint8_t*>(&tree[bytepos]);
                    }

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

                    const size_t bytepos = get_bytepos(node+m-1);
                    const int height = rho(node+m);

                    uint64_t value = LEAF_MAXVAL << height;
                    switch (LEAF_BITSIZE + height) {
                    case 17 ... 64: value -= *reinterpret_cast<auint64_t*>(&tree[bytepos]); break;
                    case  9 ... 16: value -= *reinterpret_cast<auint16_t*>(&tree[bytepos]); break;
                    default:        value -= *reinterpret_cast< auint8_t*>(&tree[bytepos]);
                    }

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
                return sizeof(TypeF<LEAF_BITSIZE>)*8
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
}

#endif // __FENWICK_TYPE_HPP__
