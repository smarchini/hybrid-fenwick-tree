#ifndef __FENWICK_HYBRID_HPP__
#define __FENWICK_HYBRID_HPP__

#include "../common.hpp"
#include "fenwick_tree.hpp"
#include <vector>
#include <iostream>

namespace hft {
    namespace fenwick {

        template <template<size_t> class TOP_TREE, template<size_t> class BOTTOM_TREE, size_t LEAF_MAXVAL, size_t BOTTOM_HEIGHT>
        class Hybrid : public FenwickTree
        {
        private:
            static constexpr size_t BOTTOM_ELEMENTS = 1ULL << BOTTOM_HEIGHT;

        protected:
            const size_t _size;
            std::vector<BOTTOM_TREE<LEAF_MAXVAL>> bottom_trees;
            TOP_TREE<LEAF_MAXVAL*BOTTOM_ELEMENTS> top_tree;

        public:
            Hybrid(uint64_t sequence[], size_t size) :
                _size(size),
                top_tree(build_top(sequence, size))
            {
                // bottom
                for (size_t i = 0; i < top_tree.size() + 1; i++) {
                    size_t length = BOTTOM_ELEMENTS*i + BOTTOM_ELEMENTS-1 <= size ? BOTTOM_ELEMENTS-1 : size % BOTTOM_ELEMENTS;
                    bottom_trees.push_back(BOTTOM_TREE<LEAF_MAXVAL>(sequence + BOTTOM_ELEMENTS*i, length));
                }

                // top
                for (size_t i = 0; i < top_tree.size(); i++)
                    top_tree.add(i, bottom_trees[i].prefix(bottom_trees[i].size()-1));
            }

            virtual uint64_t prefix(size_t idx) const
            {
                size_t top = (idx+1) / BOTTOM_ELEMENTS;
                size_t bottom = (idx+1) % BOTTOM_ELEMENTS;

                if (top == 0 || top_tree.size() == 0)
                    return bottom_trees[0].prefix(bottom-1);

                return top_tree.prefix(top-1)
                    + (bottom != 0 ? bottom_trees[top].prefix(bottom-1) : 0);
            }

            virtual void add(size_t idx, int64_t inc)
            {
                size_t top = (idx+1) / BOTTOM_ELEMENTS;
                size_t bottom = (idx+1) % BOTTOM_ELEMENTS;

                if (bottom == 0) {
                    top_tree.add(top-1, inc);
                }
                else if (top_tree.size() == 0) {
                    bottom_trees[top].add(bottom-1, inc);
                }
                else {
                    top_tree.add(top, inc);
                    bottom_trees[top].add(bottom-1, inc);
                }
            }

            using FenwickTree::find;
            virtual size_t find(uint64_t *val) const
            {
                size_t top = 0;
                if (top_tree.size() != 0)
                    top = top_tree.find(val) + 1;

                size_t bottom = top < bottom_trees.size() ? bottom_trees[top].find(val) : -1;
                return top*BOTTOM_ELEMENTS + bottom;
            }

            using FenwickTree::compfind;
            virtual size_t compfind(uint64_t *val) const
            {
                size_t top = 0;
                if (top_tree.size() != 0)
                    top = top_tree.compfind(val) + 1;

                size_t bottom = top < bottom_trees.size() ? bottom_trees[top].compfind(val) : -1;
                return top*BOTTOM_ELEMENTS + bottom;
            }

            virtual size_t size() const
            {
                return _size;
            }

            virtual size_t bit_count() const
            {
                size_t size = 0;
                for (auto &t : bottom_trees)
                    size += t.bit_count();

                return size
                    + sizeof(Hybrid<BOTTOM_TREE, TOP_TREE, LEAF_MAXVAL, BOTTOM_HEIGHT>)
                    + top_tree.bit_count();
            }

        private:
            static TOP_TREE<LEAF_MAXVAL*BOTTOM_ELEMENTS> build_top(const uint64_t sequence[], size_t size)
            {
                size_t length = size / BOTTOM_ELEMENTS;
                std::unique_ptr<uint64_t[]> subseq = std::make_unique<uint64_t[]>(length);

                for (size_t i = 1; i <= length; i++)
                    subseq[i-1] = sequence[i * BOTTOM_ELEMENTS - 1];

                return TOP_TREE<LEAF_MAXVAL*BOTTOM_ELEMENTS>(subseq.get(), length);
            }

        };

    }
}

#endif // __FENWICK_HYBRID_HPP__
