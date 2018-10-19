#ifndef __FENWICK_LTYPE_H__
#define __FENWICK_LTYPE_H__

#include "../common.hpp"
#include "fenwick_tree.hpp"

namespace hft {
    namespace fenwick {

        /**
         * class TypeL
         * @tree8-16-32-64: Fenwick Tree data.
         * @size: Number of elements in the tree.
         * @level: Lookup table, it store every starting level index.
         *
         * Each node is the smallest datatype capable of holding its data. It's supposed
         * to store increments up to 64.
         *
         * In each tree, the data is stored in a bottom-up level-order manner.
         */
        template<size_t LEAF_MAXVAL>
        class TypeL : public FenwickTree
        {
        public:
            static constexpr size_t LEAF_BITSIZE = log2(LEAF_MAXVAL);
            static_assert(LEAF_BITSIZE >= 1, "A leaf should be at least 1 bit long");
            static_assert(LEAF_BITSIZE <= 54, "A leaf should be at most 54 bit long");

        protected:
            const size_t _size;

            DArray<uint8_t >  tree8;
            DArray<uint16_t> tree16;
            DArray<uint64_t> tree64;

            DArray<size_t> level;

        public:
            TypeL(uint64_t sequence[], size_t size) :
                _size(size),
                level(msb(size) + 2)
            {
                size_t type_ends[3] = {0};
                level[0] = 0;

                size_t j = (LEAF_BITSIZE <= 8) ? 0 : (LEAF_BITSIZE <= 16) ? 1 : 2;
                for (size_t i = 1; i < level.size(); i++) {
                    type_ends[j] = level[i] = (size + (1<<(i-1))) / (1<<i) + level[i-1];

                    if (i-1 == 8-LEAF_BITSIZE || i-1 == 16-LEAF_BITSIZE) {
                        level[i] = 0;
                        j++;
                    }
                }

                switch (level.size() + LEAF_BITSIZE - 1) {
                case 17 ... 64: tree64 = DArray<uint64_t>(type_ends[2]);
                case  9 ... 16: tree16 = DArray<uint16_t>(type_ends[1]);
                default:         tree8 = DArray< uint8_t>(type_ends[0]);
                }

                // C++17 constexpr if
                if (LEAF_BITSIZE <= 8) {
                    fill_tree<uint8_t, LEAF_BITSIZE, 8>(tree8.get(), sequence);
                    fill_tree<uint16_t, 9, 16>(tree16.get(), sequence);
                    fill_tree<uint64_t, 17, 32>(tree64.get(), sequence);
                    fill_tree<uint64_t, 33, 64>(tree64.get(), sequence);
                }
                else if (LEAF_BITSIZE <= 16) {
                    fill_tree<uint16_t, LEAF_BITSIZE, 16>(tree16.get(), sequence);
                    fill_tree<uint64_t, 17, 32>(tree64.get(), sequence);
                    fill_tree<uint64_t, 33, 64>(tree64.get(), sequence);
                }
                else if (LEAF_BITSIZE <= 32) {
                    fill_tree<uint64_t, LEAF_BITSIZE, 32>(tree64.get(), sequence);
                    fill_tree<uint64_t, 33, 64>(tree64.get(), sequence);
                }
                else {
                    fill_tree<uint64_t, LEAF_BITSIZE, 64>(tree64.get(), sequence);
                }
            }

            virtual uint64_t prefix(size_t idx) const
            {
                uint64_t sum = 0ULL;
                size_t index = 0ULL;

                for (idx++; idx != index;) {
                    index += mask_last_set(idx ^ index);
                    const int height = lsb(index);
                    const size_t level_idx = index >> (1 + height);
                    const size_t tree_idx = level[height] + level_idx;

                    switch (height+LEAF_BITSIZE) {
                    case 17 ... 64: sum += tree64[tree_idx]; break;
                    case  9 ... 16: sum += tree16[tree_idx]; break;
                    default:        sum +=  tree8[tree_idx];
                    }
                }

                return sum;
            }

            virtual void add(size_t idx, int64_t inc)
            {
                for (idx = idx+1; idx <= size(); idx += mask_first_set(idx)) {
                    const int height = lsb(idx);
                    const size_t level_idx = idx >> (1 + height);
                    const size_t tree_idx = level[height] + level_idx;

                    switch (height+LEAF_BITSIZE) {
                    case 17 ... 64: tree64[tree_idx] += inc; break;
                    case  9 ... 16: tree16[tree_idx] += inc; break;
                    default:         tree8[tree_idx] += inc;
                    }
                }
            }

            using FenwickTree::find;
            virtual size_t find(uint64_t *val) const
            {
                size_t node = 0, idx = 0;

                for (size_t height = level.size() - 2; height != SIZE_MAX; height--) {
                    const size_t tree_idx = level[height] + idx;

                    idx <<= 1;

                    uint64_t value;
                    switch (height+LEAF_BITSIZE) {
                    case 17 ... 64:
                        if (tree_idx >= tree64.size()) continue;
                        value = tree64[tree_idx]; break;
                    case 9 ... 16:
                        if (tree_idx >= tree16.size()) continue;
                        value = tree16[tree_idx]; break;
                    default:
                        if (tree_idx >=  tree8.size()) continue;
                        value =  tree8[tree_idx];
                    }

                    if (*val >= value) {
                        idx++;
                        *val -= value;
                        node += 1 << height;
                    }
                }

                return node <= size() ? node-1 : size()-1;
            }

            using FenwickTree::compfind;
            virtual size_t compfind(uint64_t *val) const
            {
                size_t node = 0, idx = 0;

                for (size_t height = level.size() - 2; height != SIZE_MAX; height--) {
                    const size_t tree_idx = level[height] + idx;

                    idx <<= 1;

                    uint64_t value = LEAF_MAXVAL << height;
                    switch (height+LEAF_BITSIZE) {
                    case 17 ... 64:
                        if (tree_idx >= tree64.size()) continue;
                        value -= tree64[tree_idx]; break;
                    case 9 ... 16:
                        if (tree_idx >= tree16.size()) continue;
                        value -= tree16[tree_idx]; break;
                    default:
                        if (tree_idx >=  tree8.size()) continue;
                        value -=  tree8[tree_idx];
                    }

                    if (*val >= value) {
                        idx++;
                        *val -= value;
                        node += 1 << height;
                    }
                }

                return node <= size() ? node-1 : size()-1;
            }

            virtual size_t size() const
            {
                return _size;
            }

            virtual size_t bit_count() const
            {
                return sizeof(TypeL<LEAF_BITSIZE>)*8
                    +  tree8.bit_count() - sizeof(tree8)
                    + tree16.bit_count() - sizeof(tree16)
                    + tree64.bit_count() - sizeof(tree64)
                    + level.bit_count() - sizeof(level);
            }

        private:
            template <typename T, size_t start, size_t end>
            inline void fill_tree(T *tree, uint64_t sequence[])
            {
                const size_t levels = level.size() - 1;

                for (size_t l = start-LEAF_BITSIZE; l < levels && l <= end-LEAF_BITSIZE; l++) {
                    for (size_t node = 1<<l; node <= size(); node += 1 << (l+1)) {
                        size_t sequence_idx = node-1;
                        T value = sequence[sequence_idx];

                        size_t j = 0;
                        switch (LEAF_BITSIZE) {
                        case  1 ...  8:
                            for (; j < l && j <= 8-LEAF_BITSIZE; j++) {
                                sequence_idx >>= 1;
                                value += tree8[level[j] + sequence_idx];
                            }
                        case  9 ... 16:
                            for (; j < l && j <= 16-LEAF_BITSIZE; j++) {
                                sequence_idx >>= 1;
                                value += tree16[level[j] + sequence_idx];
                            }
                        case 17 ... 32:
                            for (; j < l && j <= 32-LEAF_BITSIZE; j++) {
                                sequence_idx >>= 1;
                                value += tree64[level[j] + sequence_idx];
                            }
                        default:
                            for (; j < l && j <= 64-LEAF_BITSIZE; j++) {
                                sequence_idx >>= 1;
                                value += tree64[level[j] + sequence_idx];
                            }
                        }

                        tree[level[l] + (node >> (l+1))] = value;
                    }
                }
            }
        };

    }
}

#endif // __FENWICK_LTYPE_H__
