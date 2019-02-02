#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>

#include <darray.hpp>
#include <fenwick.hpp>

#ifdef HUGE
static constexpr hft::PageKind page = hft::PageKind::Huge;
#elif TRANSPARENT
static constexpr hft::PageKind page = hft::PageKind::Transparent;
#else
static constexpr hft::PageKind page = hft::PageKind::Default;
#endif

template <typename T>
void find(const char *name, size_t size, size_t queries, std::mt19937 re) {
  using namespace std;
  using namespace std::chrono;
  using namespace hft::fenwick;

  chrono::high_resolution_clock::time_point begin, end;

  uint64_t u = 0;
  const double c = 1. / queries;
  constexpr size_t BOUND = 64, REPS = 5, IDXMID = (REPS - 1) / 2;
  uniform_int_distribution<uint64_t> seqdist(0, BOUND);
  uniform_int_distribution<uint64_t> cumseqdist(0, BOUND * size);
  uniform_int_distribution<size_t> idxdist(1, size);

  unique_ptr<uint64_t[]> sequence = make_unique<uint64_t[]>(size);
  for (size_t i = 0; i < size; i++)
    sequence[i] = seqdist(re);

  // constructor
  begin = chrono::high_resolution_clock::now();
  T fenwick(sequence.get(), size, page);
  end = chrono::high_resolution_clock::now();
  auto ctor = chrono::duration_cast<chrono::nanoseconds>(end - begin).count();
  cout << name << ": " << fenwick.bitCount() / (double)size << " b/item\n";
  cout << "ctor: " << ctor / (double)size << setw(12) << " ns/item" << endl;

  // find
  cout << "find: " << flush;
  vector<chrono::nanoseconds::rep> find;
  for (size_t r = 0; r < REPS; r++) {
    begin = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < queries; i++)
      u ^= fenwick.find(cumseqdist(re));
    end = chrono::high_resolution_clock::now();
    find.push_back(duration_cast<chrono::nanoseconds>(end - begin).count());
  }
  sort(find.begin(), find.end());
  cout << find[IDXMID] * c << setw(12) << " ns/item" << endl;

  const volatile uint64_t __attribute__((unused)) unused = u;
}

template <typename T>
void add(const char *name, size_t size, size_t queries, std::mt19937 re) {
  using namespace std;
  using namespace std::chrono;
  using namespace hft::fenwick;

  chrono::high_resolution_clock::time_point begin, end;

  uint64_t u = 0;
  const double c = 1. / queries;
  constexpr size_t BOUND = 64, REPS = 5, IDXMID = (REPS - 1) / 2;
  uniform_int_distribution<uint64_t> seqdist(0, BOUND);
  uniform_int_distribution<uint64_t> cumseqdist(0, BOUND * size);
  uniform_int_distribution<size_t> idxdist(1, size);

  unique_ptr<uint64_t[]> sequence = make_unique<uint64_t[]>(size);
  for (size_t i = 0; i < size; i++)
    sequence[i] = seqdist(re);

  // constructor
  begin = chrono::high_resolution_clock::now();
  T fenwick(sequence.get(), size, page);
  end = chrono::high_resolution_clock::now();
  auto ctor = chrono::duration_cast<chrono::nanoseconds>(end - begin).count();
  cout << name << ": " << fenwick.bitCount() / (double)size << " b/item\n";
  cout << "ctor: " << ctor / (double)size << setw(12) << " ns/item" << endl;

  // add
  cout << "add: " << flush;
  vector<chrono::nanoseconds::rep> add;
  for (size_t r = 0; r < REPS; r++) {
    begin = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < queries; i++) {
      size_t idx = idxdist(re);
      fenwick.add(idx, sequence[idx] > 0 ? -seqdist(re) : seqdist(re));
    }
    end = chrono::high_resolution_clock::now();
    add.push_back(duration_cast<chrono::nanoseconds>(end - begin).count());
  }
  sort(add.begin(), add.end());
  cout << add[IDXMID] * c << setw(12) << " ns/item" << endl;

  // The add cannot be erased
  u ^= fenwick.prefix(idxdist(re));
  const volatile uint64_t __attribute__((unused)) unused = u;
}

template <typename T>
void prefix(const char *name, size_t size, size_t queries, std::mt19937 re) {
  using namespace std;
  using namespace std::chrono;
  using namespace hft::fenwick;

  chrono::high_resolution_clock::time_point begin, end;

  uint64_t u = 0;
  const double c = 1. / queries;
  constexpr size_t BOUND = 64, REPS = 5, IDXMID = (REPS - 1) / 2;
  uniform_int_distribution<uint64_t> seqdist(0, BOUND);
  uniform_int_distribution<uint64_t> cumseqdist(0, BOUND * size);
  uniform_int_distribution<size_t> idxdist(1, size);

  unique_ptr<uint64_t[]> sequence = make_unique<uint64_t[]>(size);
  for (size_t i = 0; i < size; i++)
    sequence[i] = seqdist(re);

  // constructor
  begin = chrono::high_resolution_clock::now();
  T fenwick(sequence.get(), size, page);
  end = chrono::high_resolution_clock::now();
  auto ctor = chrono::duration_cast<chrono::nanoseconds>(end - begin).count();
  cout << name << ": " << fenwick.bitCount() / (double)size << " b/item\n";
  cout << "ctor: " << ctor / (double)size << setw(12) << " ns/item" << endl;

  // find
  cout << "prefix: " << flush;
  vector<chrono::nanoseconds::rep> prefix;
  for (size_t r = 0; r < REPS; r++) {
    begin = high_resolution_clock::now();
    for (uint64_t i = 0; i < queries; ++i)
      u ^= fenwick.prefix(idxdist(re));
    end = high_resolution_clock::now();
    prefix.push_back(duration_cast<chrono::nanoseconds>(end - begin).count());
  }
  std::sort(prefix.begin(), prefix.end());
  cout << prefix[IDXMID] * c << setw(12) << " ns/item" << endl;

  const volatile uint64_t __attribute__((unused)) unused = u;
}
