# Hybrid Compact Fenwick Tree and its applications for dynamic rank and selection

Different implementations of the [Fenwick tree] data structure to aim for a
better performance through cache efficiency and compression. Those
implementations are then applied to build an efficient data structure for
dynamic [rank/select dictionaries].

# The Fenwick tree data structure

You can find a brief description of each method in
`include/fenwick/fenwick_tree.hpp` ([fenwick_tree.hpp]).


There are two different node layouts:
- the *classical* (**F**): better for *prefix* and *add*, and
- the *level-ordered* (**L**): better for *find* and *compfind*.

And there are some different types of compression:
- **Fixed**: no compression, aimed for small trees;
- **Byte**: medium compression;
- **Bit**: highest compression, aimed for huge trees.

There also is an experimental **Type** compression strategy, you probably don't
wanna use it.

The Fenwick tree is born as a data structure to maintain the cumulative
frequency of a dynamic vector. Those implementations requires you to specify an
additional bound **B**, that is the maximum value your vector can hold. It may
annoys you now, but later on you will see that this value will be transparently
used by the dynamic rank and select data structure.

The naming scheme used to represent a fenwick tree is given by its compression
followed by its node layout; so `FixedF` is the name of the non-compressed
Fenwick tree with a classical node layout. All those trees are available under
the `hft::fenwick` namespace.

## Hybird Fenwick tree

The hybird Fenwick tree is a way to combine two different implementations of the
Fenwick tree data structures defined above. This way, you can take advantage of
the benefits of different compression strategies and different layouts. This
data structure is splitted in two parts: a *Top* and *Bottom*. When you are
building an hybird Fenwick tree, always remember:
- if you wanna use two different node layouts, pick *level-ordered* (**L**) for
  the *Top* and the *classical* (**F**) for the *Bottom*;
- if you wanna use two different types of compression, choose an *higher* level
  of compression for the *Bottom*.

You can still combine them as you like; anyways, those are supposed to be sane
guidelines you'd be better follow in order to make good choices.

You also need to specify a parameter **c** (the *cut point*) to identify how
many levels you want for the *Bottom* tree; for big trees a value between 14 and
18 should be good.

You can define an hybrid tree as: `template <size_t B> using MyHybirdTree =
Hybrid<ByteL, ByteF, B, 16>;`. This tree behave like any other one defined
above, so you will need to specify the template parameter **B** (the *bound*)
when you are gonna use it.


# The dynamic rank & select data structure

You can find a brief description of each method in
`include/rankselect/rank_select.hpp` ([rank_select.hpp]).

There are two different implementations:
- **Word**: the bit vector is divided in words (64-bits);
- **Stride**: the bit vector is divided in *k* words.

**Word** requires a bigger Fenwick tree (using a compressed one might be a good
choice) and it's good if linear rank and selection searches are slow (i.e. you
don't have some low-level assembly instructions to help you); while **Stride**
takes a template parameter *k* and performs linear searches on *k* words with a
much smaller underlining Fenwick tree. You probably need **Stride**.

Both these implementations relies on a Fenwick tree (of your choice) and they
are available under the `hft::ranking` namespace.

# Usage and examples

All you need is the `include` directory. This library is tested on a x86_64
Linux computer with GCC 8.2. The concepts behind this library are general, but in
fact this library uses some compiler-specific directives (i.e. the
`__attribute__((__may_alias__))`) and built-in functions (i.e.
`__builtin_popcountll`). For this reason, if you intend to use it in a different
environment you better check everything works as expected.

The following examples can be built with `g++ -I/path/to/include example.cpp`.

## Fenwick tree
``` cpp
// uncomment the line below if you want to use HugeTLB
// #define HFT_USE_HUGETLB

#include <iostream>
#include <fenwick.hpp>

// Declaration of an hybrid Fenwick tree with:
//   a level-ordered layout Top and classical layout bottom,
//   where both of them have a medium (Byte) compression strategy
template <size_t B> using MyHybird = hft::fenwick::Hybrid<hft::fenwick::ByteL, hft::fenwick::ByteF, B, 16>;

int main()
{
    // Library (hybrid fenwick tree) namespace
    using namespace hft;

    // Definition of the sequence
    constexpr size_t BOUND = 63;
    constexpr size_t SIZE = 10;
    uint64_t sequence[SIZE] = { 1, 2, 3, 4, 5, 6, 7 , 8, 9, 10 };

    // Definition three different fenwick trees on the same sequence
    fenwick::FixedF<BOUND> fen1(sequence, SIZE);  // no compression, classical layout
    fenwick::BitL<BOUND> fen2(sequence, SIZE);    // high compression, level-ordered layout
    MyHybird<BOUND> fen3(sequence, SIZE);         // the hybrid Fenwick tree defined above

    // Each tree does a different thing
    fen1.add(0, 50);
    fen2.add(4, 10);
    fen3.add(7, -5);

    // Printing the prefix sum in 8
    std::cout << "fen1.prefix(8) = " << fen1.prefix(8) << "\n"; // 95
    std::cout << "fen2.prefix(8) = " << fen2.prefix(8) << "\n"; // 55
    std::cout << "fen3.prefix(8) = " << fen3.prefix(8) << "\n"; // 40

    return 0;
}
```

## Rank and selection
``` cpp
#include <iostream>
#include <fenwick.hpp>
#include <rankselect.hpp>

// Declaration of an hybrid Fenwick tree with:
//   a level-ordered layout Top and classical layout bottom,
//   where both of them have a medium (Byte) compression strategy
template <size_t B> using MyHybrid = hft::fenwick::Hybrid<hft::fenwick::ByteL, hft::fenwick::ByteF, B, 16>;

int main()
{
    // Library (hybrid fenwick tree) namespace
    using namespace hft;

    // Definition of the bitvector
    constexpr size_t SIZE = 10;
    uint64_t bitvector[SIZE] = { 0b0010110010111010100101011100010000010011010000110000101101110101,
                                 0b1010010011110010010000100111010111001101001110110011101001100100,
                                 0b0011111111100011100111101011110110100001001111011111101110101000,
                                 0b1110101010110010110010100010001111101001100010101100101110111110,
                                 0b0101101011101010001001001111110000010101011101010110101000010011,
                                 0b1011011111110100010001101000010010101110010100000011001100111110,
                                 0b1001101100110111000111110101101111010101100110001001001011111110,
                                 0b0101000010110001110111010110000010100010101111000011111011100110,
                                 0b1111100001110111111010100001111100100010110010111101001010100100,
                                 0b1101001001110001010010001111111101000100110000000001101111111100 };

    // Definition two different dynamic rank and select data structures
    ranking::Word<fenwick::ByteF> bv1(bitvector, SIZE); // ByteF and stride of 1 word
    ranking::Stride<MyHybrid, 8> bv2(bitvector, SIZE);  // MyHybrid and stride of 8 words

    // We change the internal bit vector of those structures
    bv1.toggle(600); bv1.toggle(200); bv1.toggle(100);
    bv2.update(5, 0b1111111111111111111111111111111111111111111111111111111111111111);

    // Printing rank of 100 and selection of 300
    std::cout << "bv1.rank(100) = " << bv1.rank(100) << "\n"; // 48
    std::cout << "bv2.rank(100) = " << bv2.rank(100) << "\n"; // 48
    std::cout << "bv1.select(100) = " << bv1.select(300) << "\n"; // 573
    std::cout << "bv2.select(100) = " << bv2.select(300) << "\n"; // 508

    return 0;
}
```

## Additional notes
As you see, the bit vector is an array of `uint64_t`, so we can use some
built-in functions with no issues. If you need bigger vectors you may want them
in the heap memory. You can do it your own way (i.e. with [placement new]) or
you can use `hft::DArray<T>`.

At the moment the data structures in this library are dynamic as in *dynamic
arrays*: they deal with mutable data of fixed size; even if an implementation
with extendibility properties is indeed possible.


## Huge TLB pages
A `#define HFT_USE_HUGETLB` (before any `#include`) makes `hft::Darray<T>` use
Linux's 2MB huge TLB pages capabilities. This feature should be first (manually)
enabled in your operating system. For more information refer to [hugetlbpage] on
the Linux kernel documentation.


# TODO
- License


[rank/select dictionaries]: https://en.wikipedia.org/wiki/Succinct_data_structure#Succinct_dictionaries "rank/select dictionaries"
[Fenwick tree]: https://en.wikipedia.org/wiki/Fenwick_tree "Fenwick tree"
[rank_select.hpp]: https://github.com/pacman616/fenwick_tree/blob/master/include/rankselect/rank_select.hpp "rank\_select.hpp"
[fenwick_tree.hpp]: https://github.com/pacman616/fenwick_tree/blob/master/include/fenwick/fenwick_tree.hpp  "fenwick\_tree.hpp"
[placement new]: https://en.cppreference.com/w/cpp/language/new#Placement_new "placement new"
[hugetlbpage]: https://www.kernel.org/doc/Documentation/vm/hugetlbpage.txt
