#ifndef __RANKSELECT_WORD_H__
#define __RANKSELECT_WORD_H__

#include "rank_select.hpp"

namespace dyn {

    template <template<size_t> class T>
    class WordRankSelect {
    private:
        static constexpr size_t LEAF_BITSIZE = 64;
        T<LEAF_BITSIZE> tree;
        DArray<uint64_t> _bitvector;

    public:
        /**
         * WordRankSelect - Build a dynamic rank&select data structure
         * @bitvector: A bitvector of 64-bit words
         * @length: The length (in words) of the bitvector
         *
         * If you pass the ownership of @bitvector it will make a shallow copy
         * (copy the pointer) and if you don't it will make a deep copy (copy
         * the data) of the bitvector. This data structure works correctly as
         * long as you don't touch its underlining bitvector, so it prevents you
         * to do it.
         */
        WordRankSelect(uint64_t bitvector[], size_t length):
            tree(build_fenwick(bitvector, length)),
            _bitvector(DArray<uint64_t>(length))
        {
            std::copy_n(bitvector, length, _bitvector.get());
        }

        WordRankSelect(DArray<uint64_t> bitvector, size_t length)
            : tree(build_fenwick(bitvector.get(), length)),
              _bitvector(std::move(bitvector)) {}

        virtual const uint64_t* bitvector() const
        {
            return _bitvector.get();
        }

        virtual size_t size() const
        {
            return _bitvector.size();
        }

        virtual uint64_t rank(size_t pos) const
        {
            return ((pos/64) ? tree.prefix(pos/64 - 1) : 0)
                + popcount(_bitvector[pos/64] & compact_bitmask(pos % 64, 0));
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
            const size_t idx = tree.find(&rank) + 1;

            if (idx >= _bitvector.size()) return SIZE_MAX;

            const uint64_t rank_chunk = popcount(_bitvector[idx]);
            if (rank < rank_chunk)
                return idx*64 + select64(_bitvector[idx], rank);

            return SIZE_MAX;
        }

        virtual size_t selectZero(uint64_t rank) const
        {
            const size_t idx = tree.find_complement(&rank) + 1;

            if (idx >= _bitvector.size()) return SIZE_MAX;

            const uint64_t rank_chunk = popcount(~_bitvector[idx]);
            if (rank < rank_chunk)
                return idx*64 + select64(~_bitvector[idx], rank);

            return SIZE_MAX;
        }

        virtual uint64_t update(size_t index, uint64_t word)
        {
            const uint64_t old = _bitvector[index];
            _bitvector[index] = word;
            tree.set(index, popcount(word) - popcount(old));

            return old;
        }

        // TODO: testare
        virtual bool set(size_t index) {
          const uint64_t old = _bitvector[index / 64];
          _bitvector[index / 64] |= uint64_t(1) << (index % 64);
          bool is_changed = _bitvector[index / 64] != old;
          tree.set(index / 64, is_changed);

          return !is_changed;
        }

        // TODO: testare
        virtual bool clear(size_t index) {
          const uint64_t old = _bitvector[index / 64];
          _bitvector[index / 64] &= ~(uint64_t(1) << (index % 64));
          bool is_changed = _bitvector[index / 64] != old;
          tree.set(index / 64, -is_changed);

          return is_changed;
        }

        // TODO: testare
        virtual bool toggle(size_t index) {
          const uint64_t old = _bitvector[index / 64];
          _bitvector[index / 64] ^= uint64_t(1) << (index % 64);
          tree.set(index / 64, _bitvector[index / 64] > old ? 1 : -1);

          return _bitvector[index / 64] > old ? 0 : 1;
        }

        virtual size_t bit_count() const
        {
            return sizeof(WordRankSelect<T>)
                + _bitvector.bit_count() - sizeof(_bitvector)
                + tree.bit_count() - sizeof(tree);
        }

    private:
        T<LEAF_BITSIZE> build_fenwick(const uint64_t bitvector[], size_t length)
        {
            uint64_t *sequence = new uint64_t[length];
            for (size_t i = 0; i < length; i++)
                sequence[i] = popcount(bitvector[i]);

            T<LEAF_BITSIZE> tree(sequence, length);
            delete[] sequence;
            return tree;
        }
    };

}

#endif // __RANKSELECT_WORD_H__
