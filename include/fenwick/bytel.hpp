#ifndef __FENWICK_LBYTE_HPP__
#define __FENWICK_LBYTE_HPP__

#include "fenwick_tree.hpp"

namespace hft {
    namespace fenwick {

        /**
         * class ByteL - byte compression and level ordered node layout.
         * @sequence: sequence of integers.
         * @size: number of elements.
         * @LEAF_MAXVAL: maximum value that @sequence can store.
         *
         */
        template<size_t LEAF_MAXVAL>
        class ByteL : public FenwickTree
        {
        public:
            static constexpr size_t LEAF_BITSIZE = log2(LEAF_MAXVAL);
            static_assert(LEAF_BITSIZE >= 1 && LEAF_BITSIZE <= 64, "Leaves can't be stored in a 64-bit word");

        protected:
            const size_t _size;
            DArray<uint8_t> tree;
            DArray<size_t> level;

        public:
            ByteL(uint64_t sequence[], size_t size) :
                _size(size),
                level(lambda(size+1)+2)
            {
                level[0] = 0;
                for (size_t i = 1; i < level.size(); i++)
                    level[i] = ((size + (1<<(i-1))) / (1<<i)) * get_size(i-1) + level[i-1];

                const size_t levels = level.size() - 1;
                tree = DArray<uint8_t>(level[levels] + 3); // +3 to prevent segfault on the last element

                for (size_t l = 0; l < levels; l++) {
                    for (size_t node = 1<<l; node <= size; node += 1 << (l+1)) {
                        const size_t curr_byte_pos = level[l] + get_size(l) * (node >> (l+1));
                        auint64_t * const curr_element = reinterpret_cast<auint64_t*>(&tree[curr_byte_pos]);

                        size_t sequence_idx = node-1;
                        uint64_t value = sequence[sequence_idx];
                        for (size_t j = 0; j < l; j++) {
                            sequence_idx >>= 1;
                            const size_t prev_byte_pos = level[j] + get_size(j) * sequence_idx;
                            const auint64_t * const prev_element = reinterpret_cast<auint64_t*>(&tree[prev_byte_pos]);

                            value += *prev_element & BYTE_MASK[get_size(j)];
                        }

                        *curr_element &= ~BYTE_MASK[get_size(l)];
                        *curr_element |= value & BYTE_MASK[get_size(l)];
                    }
                }
            }

            virtual uint64_t prefix(size_t idx) const
            {
                uint64_t sum = 0;

                while (idx != 0) {
                    const int height = rho(idx);
                    const size_t level_idx = idx >> (1 + height);
                    const size_t elem_size = get_size(height);
                    const size_t byte_pos = level[height] + elem_size * level_idx;
                    const auint64_t * const compact_element = reinterpret_cast<auint64_t*>(&tree[byte_pos]);
                    sum += *compact_element & BYTE_MASK[elem_size];

                    idx = clear_rho(idx);
                }

                return sum;
            }

            virtual void add(size_t idx, int64_t inc)
            {
                while (idx <= size()) {
                    const int height = rho(idx);
                    const size_t level_idx = idx >> (1 + height);
                    const size_t byte_pos = level[height] + get_size(height) * level_idx;
                    auint64_t * const compact_element = reinterpret_cast<auint64_t*>(&tree[byte_pos]);

                    *compact_element += inc;
                    idx += mask_rho(idx);
                }
            }

            using FenwickTree::find;
            virtual size_t find(uint64_t *val) const
            {
                size_t node = 0, idx = 0;

                for (size_t height = level.size() - 2; height != SIZE_MAX; height--) {
                    const size_t elem_size = get_size(height);
                    const size_t byte_pos = level[height] + elem_size * idx;

                    idx <<= 1;

                    if (byte_pos >= level[height+1]) continue;

                    const auint64_t * const compact_element = reinterpret_cast<auint64_t*>(&tree[byte_pos]);
                    uint64_t value = *compact_element & BYTE_MASK[elem_size];

                    if(*val >= value) {
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
                    const size_t elem_size = get_size(height);
                    const size_t byte_pos = level[height] + elem_size * idx;

                    idx <<= 1;

                    if (byte_pos >= level[height+1]) continue;

                    const auint64_t * const compact_element = reinterpret_cast<auint64_t*>(&tree[byte_pos]);
                    uint64_t value = (LEAF_MAXVAL << height) - (*compact_element & BYTE_MASK[elem_size]);

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
                return _size;
            }

            virtual size_t bit_count() const
            {
                return sizeof(ByteL<LEAF_BITSIZE>)*8
                    + tree.bit_count() - sizeof(tree)
                    + level.bit_count() - sizeof(level);
            }

        private:
            static inline size_t get_size(size_t height)
            {
                return (height+LEAF_BITSIZE-1) / 8 + 1;
            }
        };

    }
}

#endif // __FENWICK_LBYTE_HPP__
