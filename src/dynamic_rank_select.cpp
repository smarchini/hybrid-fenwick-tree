#include "../include/dynamic_rank_select.hpp"
#include "../include/broadword.hpp"
#include <algorithm>
#include <iostream>

using std::size_t; using std::uint64_t; using std::uint32_t; using std::uint16_t; using std::uint8_t;

template<typename T>
T build_fenwick(const uint64_t bitvector[], size_t length)
{
    uint64_t *sequence = new uint64_t[length];
    for (size_t i = 0; i < length; i++)
        sequence[i] = popcount(bitvector[i]);

    return T(sequence, length);
}

template<typename T>
DynRankSelect<T>::DynRankSelect(uint64_t bitvector[], size_t length) :
    tree(build_fenwick<T>(bitvector, length)),
    _bitvector(DArray<uint64_t>(length))
{
    std::copy_n(bitvector, length, _bitvector.get());
}

template<typename T>
DynRankSelect<T>::DynRankSelect(DArray<uint64_t> bitvector, size_t length) :
    tree(build_fenwick<T>(_bitvector.get(), length)),
    _bitvector(std::move(bitvector))
{
}

template<typename T>
const uint64_t* DynRankSelect<T>::bitvector() const
{
    return _bitvector.get();
}

template<typename T>
size_t DynRankSelect<T>::bitvector_size() const
{
    return _bitvector.size();
}

template<typename T>
uint64_t DynRankSelect<T>::rank(size_t pos) const
{
    // TODO: spostare il controllo sul -1 dentro l'albero?
    return ((pos/64) ? tree.get(pos/64 - 1) : 0)
        + popcount(_bitvector[pos/64] & compact_bitmask(pos % 64, 0));
}

template<typename T>
uint64_t DynRankSelect<T>::rank(size_t from, size_t to) const
{
    return rank(to) - rank(from);
}

template<typename T>
uint64_t DynRankSelect<T>::rankZero(size_t pos) const
{
    return pos - rank(pos);
}

template<typename T>
uint64_t DynRankSelect<T>::rankZero(size_t from, size_t to) const
{
    return (to - from) - rank(from, to);
}

template<typename T>
size_t DynRankSelect<T>::select(uint64_t rank) const
{
    // TODO: spostare il controllo sul -1 dentro l'albero?
    const size_t idx = tree.find(rank) + 1;
    return idx*64 + select64(_bitvector[idx], rank - (idx != 0 ? tree.get(idx-1) : 0));
}

template<typename T>
size_t DynRankSelect<T>::selectZero(uint64_t rank) const
{
    // TODO: spostare il controllo sul -1 dentro l'albero?
    const size_t idx = tree.find(rank, true) + 1;
    return idx*64 + select64(~_bitvector[idx], rank - (64*idx - (idx != 0 ? tree.get(idx-1) : 0)));
}

template<typename T>
uint64_t DynRankSelect<T>::update(size_t index, uint64_t word)
{
    const uint64_t old = _bitvector[index];
    _bitvector[index] = word;
    tree.set(index, popcount(word) - popcount(old));

    return old;
}

template<typename T>
size_t DynRankSelect<T>::bit_count() const
{
    return _bitvector.size()*64 + tree.bit_count();
}
