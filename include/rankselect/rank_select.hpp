#ifndef __RANK_SELECT_HPP__
#define __RANK_SELECT_HPP__

#include "../common.hpp"

namespace hft {
    namespace ranking {

        template<typename T>
        class RankSelect {
        public:
            /**
             * RankSelect - Build a dynamic rank&select data structure
             * @bitvector: A bitvector of 64-bit words
             * @length: The length (in words) of the bitvector
             *
             * This data structure works correctly as long as you don't touch its
             * underlining bitvector, so it prevents you to do it: if you pass the
             * ownership of @bitvector it will make a shallow copy (copy the
             * pointer), if you don't it will make a deep copy (copy the data).
             */
            RankSelect(uint64_t bitvector[], size_t length);
            RankSelect(DArray<uint64_t> bitvector, size_t length);

            virtual ~RankSelect() = default;

            /**
             * bitvector() - Give you the underlining bitvector
             *
             * You can't change the junk inside unless you explicitly cast away the
             * constness, but you're not supposed to do it.
             *
             * Returns: The underlining bitvector
             */
            virtual const uint64_t* bitvector() const;

            /**
             * size() - Returns the length of the bitvector
             *
             * Returns: How many elements (words) have the bitvector
             */
            virtual size_t size() const;

            /**
             * rank() - Compute the numbers of ones preceding a specified position
             * @pos: An index of the bit vector
             *
             * Returns: How many ones there are before the index @pos
             */
            virtual uint64_t rank(size_t pos) const;

            /**
             * rank() - Compute the numbers of ones between a given range
             * @from: Starting index of the bit vector
             * @from: Ending index of the bit vector
             *
             * Returns: how many ones there are between @from and @to
             */
            virtual uint64_t rank(size_t from, size_t to) const;

            /**
             * rankZero() - Compute the numbers of zeroes preceding a specified position
             * @pos: an index of the bit vector
             *
             * Returns: how many zeroes there are before @pos and @to
             */
            virtual uint64_t rankZero(size_t pos) const;

            /**
             * rankZero() - Compute the numbers of zeroes between a given range
             * @from: Starting index of the bit vector
             * @from: Ending index of the bit vector
             *
             * Returns: How many zeroes there are between @from and @to
             */
            virtual uint64_t rankZero(size_t from, size_t to) const;

            /**
             * select() - Compute the index position of the bit of given rank
             * @rank: Number of ones
             *
             * This method returns the greatest position that is preceded by the
             * specified number of ones.
             *
             * Returns: the greatest position that is preceded by @rank ones or
             * SIZE_MAX if there isn't one
             */
            virtual size_t select(uint64_t rank) const;

            /**
             * selectZero() - Compute the index position of the bit of given zero rank
             * @rank: Number of zeroes
             *
             * This method returns the greatest position that is preceded by the
             * specified number of zeroes.
             *
             * Returns: The greatest position that is preceded by @rank zeroes or
             * SIZE_MAX if there isn't one
             */
            virtual size_t selectZero(uint64_t rank) const;

            /**
             * update() - replace a given word in the bitvector
             * @index: index of the bitvector
             * @word: new value for @bitvector[@index]
             *
             * Returns: The replaced value.
             */
            virtual uint64_t update(size_t index, uint64_t word);

            /**
             * set() - set to 1 a given bit in the bitvector
             * @index: index of a bit in the bitvector
             *
             * Returns: The previous value.
             */
            virtual void set(size_t index);

            /**
             * clear() - set to 0 a given bit in the bitvector
             * @index: index of a bit in the bitvector
             *
             * Returns: The previous value.
             */
            virtual void clear(size_t index);

            /**
             * toggle() - change the value of a given bit in the bitvector
             * @index: index of a bit in the bitvector
             *
             * Returns: The previous value.
             */
            virtual bool toggle(size_t index);

            /**
             * bit_count() - Compute an estimation of the size (in bits) of this structure
             *
             * Returns: An estimation of the number of bits used by the this structure
             */
            virtual size_t bit_count() const;
        };

    }
}

#endif // __RANK_SELECT_HPP__
