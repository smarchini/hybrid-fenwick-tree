#ifndef __LINE_RANK_SELECT_H__
#define __LINE_RANK_SELECT_H__

#include "rank_select.hpp"

namespace dyn {
    // TODO TODO TODO
    template <template<size_t> class T, size_t WORDS>
    class LineRankSelect {
    private:
        static constexpr size_t LEAF_BITSIZE = 7 + WORDS;
        T<LEAF_BITSIZE> tree;
        DArray<uint64_t> _bitvector;

    public:
        /**
         * LineRankSelect - Build a dynamic rank&select data structure
         * @bitvector: A bitvector of 64-bit words
         * @length: The length (in words) of the bitvector
         *
         * If you pass the ownership of @bitvector it will make a shallow copy
         * (copy the pointer) and if you don't it will make a deep copy (copy
         * the data) of the bitvector. This data structure works correctly as
         * long as you don't touch its underlining bitvector, so it prevents you
         * to do it.
         */
        LineRankSelect(uint64_t bitvector[], size_t length):
            tree(build_fenwick(bitvector, length)),
            _bitvector(DArray<uint64_t>(length))
        {
            std::copy_n(bitvector, length, _bitvector.get());
        }

        LineRankSelect(DArray<uint64_t> bitvector, size_t length):
            tree(build_fenwick(_bitvector.get(), length)),
            _bitvector(std::move(bitvector))
        {
        }

        virtual const uint64_t* bitvector() const
        {
            return _bitvector.get();
        }

        virtual size_t bitvector_size() const
        {
            return _bitvector.size();
        }

        virtual uint64_t rank(size_t pos) const
        {
            // TODO: spostare il controllo sul -1 dentro l'albero?
            uint64_t value = ((pos/(64*WORDS)) ? tree.get(pos/(64*WORDS) - 1) : 0);

            // TODO: implementare! è sbagliata quasi sicuramente
            for (size_t i = 0; i < WORDS; i++)
                value += popcount(_bitvector[pos/(64*WORDS)] & compact_bitmask(pos % (64*WORDS), 0));

            return value;
        }

        virtual uint64_t rank(size_t from, size_t to) const
        {
            return rank(to) - rank(from);
        }

        virtual uint64_t rankZero(size_t pos) const
        {
            return pos - rank(pos);
        }

        virtual uint64_t rankZero(size_t from, size_t to) const
        {
            return (to - from) - rank(from, to);
        }

        virtual size_t select(uint64_t rank) const
        {
            // TODO: spostare il controllo sul -1 dentro l'albero?
            const size_t idx = tree.find(rank) + 1;
            return idx*64 + select64(_bitvector[idx], rank - (idx != 0 ? tree.get(idx-1) : 0));
        }

        virtual size_t selectZero(uint64_t rank) const
        {
            // TODO: spostare il controllo sul -1 dentro l'albero?
            const size_t idx = tree.find(rank, true) + 1;
            return idx*64 + select64(~_bitvector[idx], rank - (64*idx - (idx != 0 ? tree.get(idx-1) : 0)));
        }

        virtual uint64_t update(size_t index, uint64_t word)
        {
            const uint64_t old = _bitvector[index];
            _bitvector[index] = word;
            tree.set(index, popcount(word) - popcount(old));

            return old;
        }

        virtual size_t bit_count() const
        {
            return sizeof(LineRankSelect<T, WORDS>)
                + _bitvector.size()*64
                + tree.bit_count() - sizeof(tree);
        }

    private:
        T<LEAF_BITSIZE> build_fenwick(const uint64_t bitvector[], size_t length)
        {
            uint64_t *sequence = new uint64_t[length];
            for (size_t i = 0; i < length; i++)
                sequence[i] = popcount(bitvector[i]);

            return T<LEAF_BITSIZE>(sequence, length);
        }
    };

}

#endif // __LINE_RANK_SELECT_H__
