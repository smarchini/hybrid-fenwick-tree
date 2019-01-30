#include "microbench.h"
#include <iostream>

using namespace hft;
using namespace std;

static constexpr size_t BOUND = 64;
static constexpr size_t BOUNDSIZE = log2(BOUND);

inline size_t bytesize(size_t idx) {
  return ((rho(idx) + BOUNDSIZE - 1) >> 3) + 1;
}

inline size_t bytepos(size_t idx) {
    constexpr size_t NEXTBYTE = ((BOUNDSIZE - 1) | (8 - 1)) + 1;

    constexpr size_t SMALL = ((BOUNDSIZE - 1) >> 3) + 1;
    constexpr size_t MEDIUM = NEXTBYTE - BOUNDSIZE + 1;
    constexpr size_t LARGE = MEDIUM + 8;

    constexpr size_t MULTIPLIER = 8 - SMALL - 1;

    return idx * SMALL + (idx >> MEDIUM) + (idx >> LARGE) * MULTIPLIER;
}

int main() {
    size_t i = 1;

    for (size_t j = 10000000; j <= 10000100; j++) {
        cout << "bytesize(" << j << ") = " << bytesize(j * 2048) << endl;
        cout << "bytepos(" << j << ") = " << bytepos(j * 1024) % 8  << endl;

    }

    return 0;
}
