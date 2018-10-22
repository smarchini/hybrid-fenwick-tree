# Hybrid Compact Fenwick Tree and its applications for dynamic rank and selection

Different implementations of the [Fenwick
tree](https://en.wikipedia.org/wiki/Fenwick_tree) data structure to aim for a
better performace through cache efficiency and compression. Those
implementations are then applied to build an efficient data structure for
dynamic [rank/select
dictionaries](https://en.wikipedia.org/wiki/Succinct_data_structure#Succinct_dictionaries).

# The Fenwick tree data structure

You can find a brief description of each method in `include/fenwick/fenwick_tree.hpp` (link).


There are two different node layouts:
- the *classical* (**F**): better for *prefix* and *add*, and
- the *level-ordered* (**L**): better for *find* and *compfind*.

And there are some different types of compression:
- **Fixed**: no compression, aimed for small trees;
- **Byte**: medium compression;
- **Bit**: highest compression, aimed for huge trees.

There also is an experimental ***Type** compression strategy, you probably don't
wanna use it.

The Fenwick tree is born as a data structure to maintain the cumulative
frequency of a dynamic vector. Those implementations requires you to specify a
bound **B**, that is the maximum value your vector might hold. It might bothers
you now, but later on you will see that this value will be transparently used by
the dynamic rank and select data structure.

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

You also need to specify a parameter **c** (the *cut point*) to identify how
many levels you want for the *Bottom* tree; for big trees a value between 14 and
18 should be good.

Here's how you can define an hybrid tree: `template <size_t B> using
MyHybirdTree = Hybrid<ByteL, ByteF, B, 16>;`. This tree behave like any other
one defined above, so you will need to specify the template parameter **B**
(it's *bound*) when you are gonna use it.


# The dynamic rank & select data structure

You can find a brief description of each method in
`include/ranking/ranking.hpp`.

There are two different implementations:
- **Word**: the bit vector is divided in words (64-bits);
- **Stride**: the bitvector is divided in *k* words.

**Word** requires a bigger Fenwick tree (you might wanna use a compressed one!)
and it's good if linear rank and selection searches are slow (i.e. you don't
have some low-level assembly instructions to help you); while **Stride** takes a
template parameter *k* and performs linear searches on *k* words with a much
smaller underlining Fenwick tree. You probably need **Stride**.

Both these implementations relies on the Fenwick tree of your choice and they
are available under the `hft::ranking` namespace.

# Usage and examples

All you need is the `include` directory. This library is tested with a x86_64
Linux computer and GCC 8.2. The concepts behind this library are general, but in
fact this library uses some compiler-specific directives (i.e. the
`__attribute__((__may_alias__))`) and built-in functions (i.e.
`__builtin_popcountll`). Although it might works, it's functionality is NOT
tested with different compilers and operating systems. The following examples
che be built with `g++ -I/path/to/include example.cpp`.

## Fenwick tree
Compile it with `g++ -I/path/to/include example.cpp`.
``` cpp
#include <iostream>
#include <fenwick/fixedf.hpp>
#include <fenwick/bitl.hpp>
#include <fenwick/bytef.hpp>
#include <fenwick/bytel.hpp>
#include <fenwick/hybrid.hpp>

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
    MyHybird<BOUND> fen3(sequence, SIZE);     // the hybrid Fenwick tree defined above

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

#include <fenwick/fixedf.hpp>
#include <fenwick/bytef.hpp>
#include <fenwick/bytel.hpp>
#include <fenwick/hybrid.hpp>

#include <rankselect/word.hpp>
#include <rankselect/stride.hpp>

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

    // Each tree does a different thing
    bv1.toggle(600); bv1.toggle(200); bv1.toggle(100);
    bv2.update(5, 0b1111111111111111111111111111111111111111111111111111111111111111);

    // Printing the prefix sum in 8
    std::cout << "bv1.rank(100) = " << bv1.rank(100) << "\n"; // 48
    std::cout << "bv2.rank(100) = " << bv2.rank(100) << "\n"; // 48
    std::cout << "bv1.select(100) = " << bv1.select(300) << "\n"; // 573
    std::cout << "bv2.select(100) = " << bv2.select(300) << "\n"; // 508

    return 0;
}
```

# TODO
cambiare i nomi dei file hpp,
semplificare la roba per gli include,
implementazione diretta delle formule figlio pirlate al contrario (si può fare? maybe not),
ricontrollare tutto






# Dynamic Rank & Select data structure through Fenwick Tree

This library offers a space and time efficient implementation of dynamic rank
and select data structure. Given a bitvector *b*, _n_ bits long, the rank and
select operations are defined as following:

  ![rank]

  ![select]

In other words:
- rank returns how many 1-bits there are before a given 0-based index
- while select returns the 0-based index with a given rank.

To achieve those operations in a space and time efficient way, we try out
different implementations of a Fenwick tree data structure as an underlining
representation of the bitvector. Given a bitvector we precompute its rank at
some chosen intervals then, the resulting increasing sequence is stored in a
Fenwick tree.

Such an implementation permits you to compute rank and select in O(log n) time
and to change a 64-bit word in O(log n) time.

We offer two different implementation of such a rank and select data structure.
One of them take a sample every 64-bits word of the bitvector while the other
make you choose the granularity of the samples. The latter require less space
and it permits you to tune up the data structure to perform the best on your
system. We will discuss the performance implication of those implementations
later on.

You can see the supported operations at [rank_select.hpp].

## Fenwick tree

A [Fenwick tree](https://en.wikipedia.org/wiki/Fenwick_tree) (also known as
_binary indexed tree_) is a data structure for maintaining the cumulative
frequencies which are needed to support dynamic arithmetic data compression.

We offer different implementation of such a structure, but there are only two
main differences between them: compression ratio and memory layout.

The compression ratios you can choose are:
- bit, who offer you the best compression mechanism;
- byte, who round the required space for each element up to the closest byte;
- and datatype accuracy, who round it up to the closest datatype (from uint8\_t
  to uint64\_t).

Each of this can either have a naive memory layout (each element is disposed in
an increasing sequence) or a bottom-up level-order layout. We will discuss about
the performance implication of those implementations later on.

When you build a rank and select data structure you should chose what
underlining Fenwick tree implementation it will use. The same way, when you
build a Fenwick tree, you have to specify the bit-length of the increments it
will maintain.

You can see the supported operations at [fenwick_tree.hpp].

# How to use it
Since this implementation rely on some low level mechanism, some
compiler-dependent directives are used: this code rely on some GCC built-ins and
attributes, it also use few C++14 features. It's tested under GCC 6.3.1.

All you need to do is to add the include directory to your compiler additional
search paths, then you can use the library as following.

## Dynamic rank & select
``` cpp
#include <iostream>

#include <rankselect/word.hpp>
#include <rankselect/stride.hpp>

#include <fenwick/bytef.hpp>
#include <fenwick/bitf.hpp>

int main()
{
    using namespace std;
    using namespace dyn;

    size_t size;

    cout << "Length (in words) of the bitvector: ";
    cin >> size;

    // Initialize the bitvector
    uint64_t *bitvect = new uint64_t[size];
    for (size_t i = 0; i < size; i++) {
        cout << "Insert the " << i+1 << "^ word of the bitvector: ";
        cin >> bitvect[i];
    }

    // Two different implementation of rank&select data structure
    Word<ByteF> bv1(bitvect, size);   // a sample for each word, maintained into a ByteF
    Stride<BitF, 8> bv2(bitvect, size); // a sample every 8 words, maintained into a BitF

    // You don't need bitvect anymore, it's stored inside the data structure
    delete[] bitvect;

    // Some operations
    uint64_t newval;

    cout << "\nInsert a new value for the first word (bv1): ";
    cin >> newval;
    bv1.update(0, newval);

    cout << "Insert a new value for the last word (bv2): ";
    cin >> newval;
    bv2.update(size-1, newval);

    cout << "\nbv1.rank(size/2) = " << bv1.rank(size/2) << "\n";
    cout << "bv2.rank(size/2) = " << bv2.rank(size/2) << "\n\n";

    cout << "bv1.select(size/2) = " << bv1.select(size/2) << "\n";
    cout << "bv2.select(size/2) = " << bv2.select(size/2) << "\n";

    return 0;
}
```

## Fenwick tree
``` cpp
#include <iostream>

#include <fenwick/bytel.hpp>
#include <fenwick/bitf.hpp>

int main()
{
    using namespace std;
    using namespace dyn;

    constexpr size_t MAXVALUE = 64;
    size_t size;

    cout << "Length of the sequence: ";
    cin >> size;

    // Initialize the sequence
    uint64_t *sequence = new uint64_t[size];
    for (size_t i = 0; i < size; i++) {
        cout << "Insert the " << i+1 << "^ increment (0 - " << MAXVALUE << "): ";
        cin >> sequence[i];
    }

    // Two different implementation of Fenwick tree data structure
    ByteL<MAXVALUE> fen1(sequence, size); // bottom-up level-order layout and byte-aligned
    BitF<MAXVALUE> fen2(sequence, size);   // naive layout and bit-aligned

    // You don't need the sequence anymore
    delete[] sequence;

    // Some operations
    uint64_t inc;

    cout << "\nIncrease the first value of fen1: ";
    cin >> inc;
    fen1.add(0, inc);

    cout << "Increase the last value of fen2: ";
    cin >> inc;
    fen2.add(size-1, inc);

    cout << "\nfen1.find(100) = " << fen1.find(100) << "\n";
    cout << "fen2.find(100) = " << fen2.find(100) << "\n\n";

    for (size_t i = 0; i < size; i++) {
        cout << "fen1: " << i+1 << "^ element of the sequence: " << fen1.prefix(i) << "\n";
        cout << "fen2: " << i+1 << "^ element of the sequence:" << fen2.prefix(i) << "\n\n";
    }

    return 0;
}
```

# Performance analysis
TODO

# External dependencies
- [Google Test](https://github.com/google/googletest) C++ unit testing
  framework and [lcov](http://ltp.sourceforge.net/coverage/lcov.php).
- [DYNAMIC](https://github.com/xxsds/DYNAMIC) succinct dynamic bitvector
  (already included in the project and only used for testing and benchmark
  purpose).

[rank]: https://goo.gl/WN481H "\text{rank}\_\mathbf{b}(p) = | \{ i < p \ | \ \mathbf{b}\_i = 1 \} |"
[select]: https://goo.gl/AaY1S5 "\text{select}\_\mathbf{b}(k) = \max{ \{ p \ | \ \text{rank}\_\mathbf{b}(p) \le k \} }"
[rank_select.hpp]: https://github.com/pacman616/fenwick_tree/blob/master/include/rankselect/rank_select.hpp "rank\_select.hpp"
[fenwick_tree.hpp]: https://github.com/pacman616/fenwick_tree/blob/master/include/fenwick/fenwick_tree.hpp  "fenwick\_tree.hpp"
