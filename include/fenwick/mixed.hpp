#ifndef __FENWICK_MIXED_H__
#define __FENWICK_MIXED_H__

#include "../common.hpp"
#include "fenwick_tree.hpp"
#include <vector>
#include <iostream>

namespace dyn {

    template <template<size_t> class BOTTOM_TREE, template<size_t> class TOP_TREE, size_t LEAF_MAXVAL, size_t BOTTOM_ELEMENTS>
    class MixedFenwickTree : public FenwickTree
    {
    protected:
        const size_t _size;
        std::vector<BOTTOM_TREE<LEAF_MAXVAL>> bottom_trees;
        TOP_TREE<LEAF_MAXVAL*BOTTOM_ELEMENTS> top_tree;

    public:
        MixedFenwickTree(uint64_t sequence[], size_t size) :
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
                top_tree.set(i, bottom_trees[i].get(bottom_trees[i].size()-1));
        }

        virtual uint64_t get(size_t idx) const
        {
            size_t top = (idx+1) / BOTTOM_ELEMENTS;
            size_t bottom = (idx+1) % BOTTOM_ELEMENTS;

            if (top == 0 || top_tree.size() == 0)
                return bottom_trees[0].get(bottom-1);

            return top_tree.get(top-1)
                + (bottom != 0 ? bottom_trees[top].get(bottom-1) : 0);
        }

        virtual void set(size_t idx, int64_t inc)
        {
            size_t top = (idx+1) / BOTTOM_ELEMENTS;
            size_t bottom = (idx+1) % BOTTOM_ELEMENTS;

            if (bottom == 0) {
                top_tree.set(top-1, inc);
            }
            else if (top_tree.size() == 0) {
                bottom_trees[top].set(bottom-1, inc);
            }
            else {
                top_tree.set(top, inc);
                bottom_trees[top].set(bottom-1, inc);
            }
        }

        // TODO: devo eliminare la chiamata alla get, è lenta. Possibile
        // soluzione: overload sulla find, una accetta int* e l'altra accetta
        // int e richiama la precedente. Se funziona, è possibile ottimizzare
        // anche la select.
        virtual size_t find(uint64_t val) const
        {
            size_t top = 0;
            if (top_tree.size() != 0) {
                top = top_tree.find(val) + 1;
                val -= top != 0 ? top_tree.get(top-1) : 0; 
            }

            size_t bottom = top < bottom_trees.size() ? bottom_trees[top].find(val) : -1;
            return top*BOTTOM_ELEMENTS + bottom;
        }

        virtual size_t find_complement(uint64_t val) const
        {
            size_t top = 0;
            if (top_tree.size() != 0) {
                top = top_tree.find_complement(val) + 1;
                if (top != 0 ) val -= LEAF_MAXVAL*BOTTOM_ELEMENTS*top - top_tree.get(top-1);
            }

            size_t bottom = top < bottom_trees.size() ? bottom_trees[top].find_complement(val) : -1;
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
                + sizeof(MixedFenwickTree<BOTTOM_TREE, TOP_TREE, LEAF_MAXVAL, BOTTOM_ELEMENTS>)
                + top_tree.bit_count();
        }

    private:
        static TOP_TREE<LEAF_MAXVAL*BOTTOM_ELEMENTS> build_top(const uint64_t sequence[], size_t size)
        {
            size_t length = size / BOTTOM_ELEMENTS;
            uint64_t *subseq = new uint64_t[length];

            for (size_t i = 1; i <= length; i++)
                subseq[i-1] = sequence[i * BOTTOM_ELEMENTS - 1];

            // TODO: assicurarsi che tutti i Fenwick si comportino bene con length=0 (eg. Bit da problemi)
            TOP_TREE<LEAF_MAXVAL*BOTTOM_ELEMENTS> tree(subseq, length);
            delete[] subseq;
            return tree;
        }

    };

}

#endif // __FENWICK_MIXED_H__
