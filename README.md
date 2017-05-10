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
#include <rankselect/line.hpp>

#include <fenwick/byte.hpp>
#include <fenwick/bit.hpp>

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
    WordRankSelect<ByteFenwickTree> bv1(bitvect, size);   // a sample for each word, maintained into a ByteFenwickTree
    LineRankSelect<BitFenwickTree, 8> bv2(bitvect, size); // a sample every 8 words, maintained into a BitFenwickTree

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

#include <fenwick/lbyte.hpp>
#include <fenwick/bit.hpp>

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
    LByteFenwickTree<MAXVALUE> fen1(sequence, size); // bottom-up level-order layout and byte-aligned
    BitFenwickTree<MAXVALUE> fen2(sequence, size);   // naive layout and bit-aligned

    // You don't need the sequence anymore
    delete[] sequence;

    // Some operations
    uint64_t inc;

    cout << "\nIncrease the first value of fen1: ";
    cin >> inc;
    fen1.set(0, inc);

    cout << "Increase the last value of fen2: ";
    cin >> inc;
    fen2.set(size-1, inc);

    cout << "\nfen1.find(100) = " << fen1.find(100) << "\n";
    cout << "fen2.find(100) = " << fen2.find(100) << "\n\n";

    for (size_t i = 0; i < size; i++) {
        cout << "fen1: " << i+1 << "^ element of the sequence: " << fen1.get(i) << "\n";
        cout << "fen2: " << i+1 << "^ element of the sequence:" << fen2.get(i) << "\n\n";
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
