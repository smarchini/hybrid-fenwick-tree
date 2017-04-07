#ifndef __DYNAMIC_RANK_SELECT_H__
#define __DYNAMIC_RANK_SELECT_H__

#include "fenwick_tree.hpp"
#include "simple_fenwick_tree.hpp"
#include "typed_fenwick_tree.hpp"
#include "byte_fenwick_tree.hpp"
#include "compact_fenwick_tree.hpp"
#include "shrank_fenwick_tree.hpp"

template<typename T>
class DynRankSelect {
private:
    T tree;
    DArray<std::uint64_t> _bitvector;

public:
    /**
     * DynRankSelect - Build a dynamic rank&select data structure
     * @bitvector: A bitvector of 64-bit words
     * @length: The length (in words) of the bitvector
     *
     * If you pass the ownership of @bitvector it will make a shallow copy (copy
     * the pointer) and if you don't it will make a deep copy (copy the data) of
     * the bitvector. This data structure works correctly as long as you don't
     * touch its underlining bitvector, so it prevents you to do it.
     */
    DynRankSelect(std::uint64_t bitvector[], std::size_t length);
    DynRankSelect(DArray<std::uint64_t> bitvector, size_t length);

    /**
     * get_bitvector() - Give you the underlining bitvector
     *
     * You can't change the junk inside unless you explicitly cast away the
     * constness, but you're not supposed to do it.
     *
     * Returns: The underlining bitvector
     */
    const uint64_t* bitvector() const;

    /**
     * bitvector_size() - Returns the length of the bitvector
     *
     * Returns: How many elements (words) have the bitvector
     */
    size_t bitvector_size() const;

    /**
     * rank() - Compute the numbers of ones preceding a specified position
     * @pos: An index of the bit vector
     *
     * Returns: How many ones there are before the index @pos
     */
    std::uint64_t rank(std::size_t pos) const;

    /**
     * rank() - Compute the numbers of ones between a given range
     * @from: Starting index of the bit vector
     * @from: Ending index of the bit vector
     *
     * Returns: how many ones there are between @from and @to
     */
    std::uint64_t rank(std::size_t from, std::size_t to) const;

    /**
     * rankZero() - Compute the numbers of zeroes preceding a specified position
     * @pos: an index of the bit vector
     *
     * Returns: how many zeroes there are before @pos and @to
     */
    std::uint64_t rankZero(std::size_t pos) const;

    /**
     * rank() - Compute the numbers of zeroes between a given range
     * @from: Starting index of the bit vector
     * @from: Ending index of the bit vector
     *
     * Returns: How many zeroes there are between @from and @to
     */
    std::uint64_t rankZero(std::size_t from, std::size_t to) const;

    /**
     * select() - Compute the index position of the bit of given rank
     * @rank: Number of ones
     *
     * This method returns the greatest position that is preceded by the
     * specified number of ones.
     *
     * Returns: the greatest position that is preceded by @rank ones
     * TODO: cosa deve succedere se questo valore non esiste? (ritornare -1ULL ?)
     */
    std::size_t select(std::uint64_t rank) const;

    /**
     * selectZero() - Compute the index position of the bit of given zero rank
     * @rank: Number of zeroes
     *
     * This method returns the greatest position that is preceded by the
     * specified number of zeroes.
     *
     * Returns: The greatest position that is preceded by @rank zeroes
     * TODO: cosa deve succedere se questo valore non esiste? (ritornare -1ULL ?)
     */
    std::size_t selectZero(std::uint64_t rank) const;

    /**
     * updateWord() - replace a word of the given in the bitvector
     * @index: index of the bitvector
     * @word: new value for @bitvector[@index]
     *
     * Returns: The replaced value.
     */
    std::uint64_t update(std::size_t index, std::uint64_t word);

    /**
     * bit_count() - An estimation of the size (in bits) of this data structure
     *
     * It doesn't count the addictional data used by the structure since its
     * just a small constant value. It doesn't depends by size of the bitvector.
     *
     * Returns: An estimation of the number of bits used by the this structure
     */
    std::size_t bit_count() const;
};

template class DynRankSelect<SimpleFenwickTree>;
template class DynRankSelect<TypedFenwickTree>;
template class DynRankSelect<ByteFenwickTree>;
template class DynRankSelect<CompactFenwickTree>;
template class DynRankSelect<ShrankFenwickTree>;

#endif // __DYNAMIC_RANK_SELECT_H__
