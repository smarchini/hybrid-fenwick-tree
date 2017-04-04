#include "../include/dynamic_rank_select.hpp"
#include "../include/broadword.hpp"
#include <algorithm>
#include <iostream>

using std::size_t; using std::uint64_t; using std::uint32_t; using std::uint16_t; using std::uint8_t;

template<typename T>
T build_fenwick(std::uint64_t bitvector[], std::size_t length)
{
    uint64_t *sequence = new uint64_t[length];
    for (size_t i = 0; i < length; i++)
        sequence[i] = popcount(bitvector[i]);

    return T(sequence, length);
}

template<typename T>
DynRankSelect<T>::DynRankSelect(std::uint64_t bitvector[], std::size_t length) :
    tree(build_fenwick<T>(bitvector, length)),
    size(length)
{
    this->bitvector = new uint64_t[length];
    std::copy_n(bitvector, length, this->bitvector);
}

template<typename T>
std::uint64_t DynRankSelect<T>::rank(std::size_t pos) const
{
    // TODO: spostare il controllo sul -1 dentro l'albero?
    return (pos/64 ? tree.get(pos/64 - 1) : 0)
        + popcount(bitvector[pos/64] & compact_bitmask(pos % 64, 0));
}

template<typename T>
std::uint64_t DynRankSelect<T>::rank(std::size_t from, std::size_t to) const
{
    // forse si può sfruttare il fatto che sono fenwick tree per beccare la differenza fare to-from in un unico passaggio?
    return rank(to) - rank(from);
}

template<typename T>
std::uint64_t DynRankSelect<T>::rankZero(std::size_t pos) const
{
    return pos - rank(pos);
}

template<typename T>
std::uint64_t DynRankSelect<T>::rankZero(std::size_t from, std::size_t to) const
{
    return (to - from) - rank(from, to);
}

template<typename T>
std::size_t DynRankSelect<T>::select(std::uint64_t rank) const
{
    // TODO: spostare il controllo sul -1 dentro l'albero?
    const size_t idx = tree.find(rank) + 1;
    return idx*64 + select64(bitvector[idx], rank - (idx != 0 ? tree.get(idx-1) : 0));
}

template<typename T>
std::size_t DynRankSelect<T>::selectZero(std::uint64_t rank) const
{
    // TODO: spostare il controllo sul -1 dentro l'albero?
    const size_t idx = tree.find(rank, true) + 1;
    return idx*64 + select64(~bitvector[idx], rank - (64*idx - (idx != 0 ? tree.get(idx-1) : 0)));
}

template<typename T>
std::size_t DynRankSelect<T>::bit_count() const
{
    return size + tree.bit_count();
}


template<typename T>
std::uint64_t DynRankSelect<T>::update(std::size_t index, std::uint64_t word)
{
    const uint64_t old = bitvector[index];
    bitvector[index] = word;
    tree.set(index, popcount(word) - popcount(old));

    return old;
}
