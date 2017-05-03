#ifndef __FENWICK_COMPACT_H__
#define __FENWICK_COMPACT_H__

#include "../common.hpp"
#include "fenwick_tree.hpp"

namespace dyn {

   /**
    * class CompactFenwickTree
    * @size: Number of elements in the tree.
    * @tree: Byte indexted Fenwick Tree data.
    * @level: Lookup table, it store the starting bit-position of each level.
    *
    * Each node use exactly the number of bits it needs. It's supposed to store
    * increments up to 64, so each leaf needs 7 bit (LEAF_BITSIZE).
    *
    * The data is stored in a bottom-up level-order manner. Let @n be the number
    * of elements in the Fenwick Tree:
    * - from bit 0 to bit 7*n/2: leaves
    * - from bit 7*n/2 to bit 7*n/2+7*n/4: height 1 internal nodes
    * and so on until the end of the tree.
    */
    template<size_t LEAF_BITSIZE>
    class CompactFenwickTree : public FenwickTree
    {
        static_assert(LEAF_BITSIZE >= 1, "A leaf should be at least 1 bit long");
        static_assert(LEAF_BITSIZE <= 55, "A leaf should be at most 55 bit long");

    protected:
        const size_t size;
        DArray<uint8_t> tree;
        DArray<size_t> level;

    public:
        /**
         * CompactFenwickTree - Build a FenwickTree given a sequence of increments.
         * @sequence: Sequence of increments.
         * @size: Number of elements stored by the sequence.
         *
         * Running time: O(length)
         */
        CompactFenwickTree(uint64_t sequence[], size_t size) :
            size(size),
            level(msb(size) + 2)
        {
            level[0] = 0;
            for (size_t i = 1; i < level.size(); i++)
                level[i] = ((size + (1<<(i-1))) / (1<<i)) * (LEAF_BITSIZE-1+i) + level[i-1];

            const size_t levels = level.size() - 1;
            tree = DArray<uint8_t>((level[levels]-1) / 8 + 1 + 4); // +4 to prevent segfault on the last element

            for (size_t l = 0; l < levels; l++) {
                for (size_t node = 1<<l; node <= size; node += 1 << (l+1)) {
                    const size_t curr_bit_pos = level[l] + (LEAF_BITSIZE+l) * (node >> (l+1));
                    const size_t curr_shift = curr_bit_pos & 0b111;
                    const uint64_t curr_mask = compact_bitmask(LEAF_BITSIZE+l, curr_shift);
                    auint64_t * const curr_element = reinterpret_cast<auint64_t*>(&tree[curr_bit_pos/8]);

                    size_t sequence_idx = node-1;
                    uint64_t value = sequence[sequence_idx];
                    for (size_t j = 0; j < l; j++) {
                        sequence_idx >>= 1;

                        const size_t prev_bit_pos = level[j] + (LEAF_BITSIZE+j) * sequence_idx;
                        const size_t prev_shift = prev_bit_pos & 0b111;
                        const uint64_t prev_mask = compact_bitmask(LEAF_BITSIZE+j, prev_shift);
                        const auint64_t * const prev_element = reinterpret_cast<auint64_t*>(&tree[prev_bit_pos/8]);

                        value += (*prev_element & prev_mask) >> prev_shift;
                    }

                    *curr_element &= ~curr_mask;
                    *curr_element |= curr_mask & (value << curr_shift);
                }
            }
        }

        virtual uint64_t get(size_t idx) const
        {
            uint64_t sum = 0ULL;
            size_t index = 0ULL;

            for (idx++; idx != index;) {
                index += mask_last_set(idx ^ index);
                const int height = lsb(index);
                const size_t level_idx = index >> (1 + height);

                const size_t bit_pos = level[height] + (LEAF_BITSIZE+height) * level_idx;
                const size_t shift = bit_pos & 0b111;
                const uint64_t mask = compact_bitmask(LEAF_BITSIZE+height, shift);
                const auint64_t * const compact_element = reinterpret_cast<auint64_t*>(&tree[bit_pos/8]);

                sum += (*compact_element & mask) >> shift;
            }

            return sum;
        }

        virtual void set(size_t idx, int64_t inc)
        {
            for (idx = idx+1; idx <= size; idx += mask_first_set(idx)) {
                const int height = lsb(idx);
                const size_t level_idx = idx >> (1 + height);
                const size_t bit_pos = level[height] + (LEAF_BITSIZE+height) * level_idx;
                const size_t shift = bit_pos & 0b111;
                auint64_t * const compact_element = reinterpret_cast<auint64_t*>(&tree[bit_pos/8]);
                *compact_element += inc << shift;
            }
        }

        virtual size_t find(uint64_t val, bool complement=false) const
        {
            size_t node = 0, idx = 0;

            for (size_t height = level.size() - 2; height != -1ULL; height--) {
                const size_t bit_pos = level[height] + (LEAF_BITSIZE+height) * idx;
                const auint64_t * const compact_element = reinterpret_cast<auint64_t*>(&tree[bit_pos/8]);
                const size_t shift = bit_pos & 0b111;
                const uint64_t mask = compact_bitmask(LEAF_BITSIZE+height, 0);

                idx <<= 1;

                if (bit_pos >= level[height+1]) continue;
                uint64_t value = (*compact_element >> shift) & mask;

                if (complement)
                    value = (1ULL << (LEAF_BITSIZE + height - 1)) - value;

                if (val >= value) {
                    idx++;
                    val -= value;
                    node += 1 << height;
                }
            }

            return node <= size ? node-1 : size-1;
        }


        virtual size_t bit_count() const
        {
            return sizeof(CompactFenwickTree<LEAF_BITSIZE>)*8
                + tree.bit_count() - sizeof(tree)
                + level.bit_count() - sizeof(level);
        }
    };

}

#endif // __FENWICK_COMPACT_H__
