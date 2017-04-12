#ifndef __TYPED_FENWICK_TREE_H__
#define __TYPED_FENWICK_TREE_H__

#include "../common.hpp"
#include "fenwick_tree.hpp"

namespace dyn {

   /**
    * class TypedFenwickTree
    * @tree8-16-32-64: Fenwick Tree data.
    * @size: Number of elements in the tree.
    * @level: Lookup table, it store every starting level index.
    *
    * Each node is the smallest datatype capable of holding its data. It's supposed
    * to store increments up to 64.
    *
    * In each tree, the data is stored in a bottom-up level-order manner.
    */
    template<size_t LEAF_BITSIZE>
    class TypedFenwickTree : public FenwickTree
    {
    protected:
        const size_t size;

        size_t type_ends[4] = {0};
        DArray<uint64_t> tree64;
        DArray<uint32_t> tree32;
        DArray<uint16_t> tree16;
        DArray<uint8_t>  tree8;

        DArray<size_t> level;

    public:
        TypedFenwickTree(uint64_t sequence[], size_t size) :
            size(size),
            level(find_last_set(size)+1)
        {
            level[0] = 0;
            for (size_t i = 1, j = 0; i < level.size(); i++) {
                type_ends[j] = (size + (1<<(i-1))) / (1<<i) + level[i-1];
                level[i] = (i-1 == 8-LEAF_BITSIZE || i-1 == 16-LEAF_BITSIZE || i-1 == 32-LEAF_BITSIZE) ? 0 : type_ends[j];

                if (i-1 == 8-LEAF_BITSIZE || i-1 == 16-LEAF_BITSIZE || i-1 == 32-LEAF_BITSIZE) j++;
            }

            switch(level.size() + LEAF_BITSIZE - 1) {
            case 33 ... 64: tree64 = DArray<uint64_t>(type_ends[3]);
            case 17 ... 32: tree32 = DArray<uint32_t>(type_ends[2]);
            case 9 ... 16:  tree16 = DArray<uint16_t>(type_ends[1]);
            default:         tree8 = DArray<uint8_t>(type_ends[0]);
            }

            fill_tree<uint8_t, LEAF_BITSIZE, 8>(tree8.get(), sequence);
            fill_tree<uint16_t, 9, 16>(tree16.get(), sequence);
            fill_tree<uint32_t, 17, 32>(tree32.get(), sequence);
            fill_tree<uint64_t, 33, 64>(tree64.get(), sequence);
        }

        virtual uint64_t get(size_t idx) const
        {
            uint64_t sum = 0ULL;

            idx++;
            size_t index = 0ULL;

            do {
                index += mask_last_set(idx ^ index);
                const size_t height = find_first_set(index) - 1;
                const size_t level_idx = index >> (1 + height);
                const size_t tree_idx = level[height] + level_idx;

                switch(height+LEAF_BITSIZE) {
                case 33 ... 64: sum += tree64[tree_idx]; break;
                case 17 ... 32: sum += tree32[tree_idx]; break;
                case  9 ... 16: sum += tree16[tree_idx]; break;
                default:        sum +=  tree8[tree_idx]; break;
                }
            } while (idx ^ index);

            return sum;
        }

        virtual void set(size_t idx, int64_t inc)
        {
            for (idx = idx+1; idx <= size; idx += mask_first_set(idx)) {
                const size_t height = find_first_set(idx) - 1;
                const size_t level_idx = idx >> (1 + height);
                const size_t tree_idx = level[height] + level_idx;

                switch(height+LEAF_BITSIZE) {
                case 33 ... 64: tree64[tree_idx] += inc; break;
                case 17 ... 32: tree32[tree_idx] += inc; break;
                case  9 ... 16: tree16[tree_idx] += inc; break;
                default       :  tree8[tree_idx] += inc; break;
                }
            }
        }

        virtual size_t find(uint64_t val, bool complement=false) const
        {
            size_t node = 0, idx = 0;

            for (uint64_t height = level.size() - 2; height != -1ULL; height--) {
                const size_t tree_idx = level[height] + idx;

                uint64_t value = 0;
                switch(height+LEAF_BITSIZE) {
                case 33 ... 64:
                    if (tree_idx >= type_ends[3]) value = -1ULL;
                    else value = tree64[tree_idx];
                    break;
                case 17 ... 32:
                    if (tree_idx >= type_ends[2]) value = -1ULL;
                    else value = tree32[tree_idx];
                    break;
                case 9 ... 16:
                    if (tree_idx >= type_ends[1]) value = -1ULL;
                    else value = tree16[tree_idx];
                    break;
                default:
                    if (tree_idx >= type_ends[0]) value = -1ULL;
                    else value =  tree8[tree_idx];
                    break;
                }

                if (complement)
                    value = (1ULL << (LEAF_BITSIZE + height - 1)) - value;

                idx <<= 1;

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
            return sizeof(TypedFenwickTree<LEAF_BITSIZE>)*8
                + type_ends[0] * 8
                + type_ends[1] * 16
                + type_ends[2] * 32
                + type_ends[3] * 64;
        }

    private:
        template <typename T, size_t start, size_t end>
        inline void fill_tree(T *tree, uint64_t sequence[]) {
            const size_t levels = level.size() - 1;
            for (size_t l = start-LEAF_BITSIZE; l < levels && l <= end-LEAF_BITSIZE; l++) {
                for (size_t node = 1<<l; node <= size; node += 1 << (l+1)) {
                    size_t sequence_idx = node-1;
                    T value = sequence[sequence_idx];

                    for (size_t j = 0; j < l && j <= 8-LEAF_BITSIZE; j++) {
                        sequence_idx >>= 1;
                        value += tree8[level[j] + sequence_idx];
                    }
                    for (size_t j = 8-LEAF_BITSIZE+1; j < l && j <= 16-LEAF_BITSIZE; j++) {
                        sequence_idx >>= 1;
                        value += tree16[level[j] + sequence_idx];
                    }
                    for (size_t j = 16-LEAF_BITSIZE+1; j < l && j <= 32-LEAF_BITSIZE; j++) {
                        sequence_idx >>= 1;
                        value += tree32[level[j] + sequence_idx];
                    }
                    for (size_t j = 32-LEAF_BITSIZE+1; j < l && j <= 64-LEAF_BITSIZE; j++) {
                        sequence_idx >>= 1;
                        value += tree64[level[j] + sequence_idx];
                    }

                    tree[level[l] + (node >> (l+1))] = value;
                }
            }
        }
    };

}

#endif // __TYPED_FENWICK_TREE_H__
