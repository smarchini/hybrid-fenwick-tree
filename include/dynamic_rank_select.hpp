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
    std::uint64_t *bitvector;
    std::size_t size;

public:
    // TODO: rule of five sulle implementazioni di fenwick tree (anche qui e e ovunque gestisco la memoria a mano)
    DynRankSelect(std::uint64_t bitvector[], std::size_t length);

    /**
     * rank - compute the numbers of ones preceding a specified position
     * @pos: an index of the bit vector
     *
     * Returns: how many ones there are before the index @pos
     */
    std::uint64_t rank(std::size_t pos) const;

    /**
     * rank - compute the numbers of ones between a given range
     * @from: starting index of the bit vector
     * @from: ending index of the bit vector
     *
     * Returns: how many ones there are between @from and @to
     */
    std::uint64_t rank(std::size_t from, std::size_t to) const;

    /**
     * rankZero - compute the numbers of zeroes preceding a specified position
     * @pos: an index of the bit vector
     *
     * Returns: how many zeroes there are before @pos and @to
     */
    std::uint64_t rankZero(std::size_t pos) const;

    /**
     * rank - compute the numbers of zeroes between a given range
     * @from: starting index of the bit vector
     * @from: ending index of the bit vector
     *
     * Returns: how many zeroes there are between @from and @to
     */
    std::uint64_t rankZero(std::size_t from, std::size_t to) const;

    /**
     * select - compute the index position of the bit of given rank
     * @rank: number of ones
     *
     * This method returns the greatest position that is preceded by the
     * specified number of ones.
     *
     * Returns: the greatest position that is preceded by @rank ones
     * TODO: cosa deve succedere se questo valore non esiste? (ritornare -1ULL ?)
     */
    std::size_t select(std::uint64_t rank) const;

    /**
     * selectZero - compute the index position of the bit of given zero rank
     * @rank: number of zeroes
     *
     * This method returns the greatest position that is preceded by the
     * specified number of zeroes.
     *
     * Returns: the greatest position that is preceded by @rank zeroes
     * TODO: cosa deve succedere se questo valore non esiste? (ritornare -1ULL ?)
     */
    std::size_t selectZero(std::uint64_t rank) const;


    // TODO: aggiungere la parte dinamica


    /**
     * bit_count - Returns an estimation of the number of bits used by the this
     * structure.
     */
    std::size_t bit_count() const;
};

template class DynRankSelect<SimpleFenwickTree>;
template class DynRankSelect<TypedFenwickTree>;
template class DynRankSelect<ByteFenwickTree>;
template class DynRankSelect<CompactFenwickTree>;
template class DynRankSelect<ShrankFenwickTree>;

#endif // __DYNAMIC_RANK_SELECT_H__
