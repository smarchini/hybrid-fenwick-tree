#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <random>
#include <string>

#include <darray.hpp>
#include <fenwick.hpp>

template <typename T>
void fen_to_file(std::string name, size_t size, std::mt19937 re) {
  using namespace std;
  using namespace hft::fenwick;
  constexpr size_t BOUND = 64;

  uniform_int_distribution<uint64_t> seqdist(0, BOUND);
  unique_ptr<uint64_t[]> sequence = make_unique<uint64_t[]>(size);
  for (size_t i = 0; i < size; i++)
    sequence[i] = seqdist(re);

  string filename = name + "." + to_string(BOUND) + ".bin";
  ofstream myfile(filename, ios::binary);
  myfile << T(sequence.get(), size);
}

template <typename T>
T file_to_fen(std::string name) {
  using namespace std;
  using namespace hft::fenwick;
  constexpr size_t BOUND = 64;
  T fenwick(nullptr, 0);

  string filename = name + "." + to_string(BOUND) + ".bin";
  ifstream file(filename.c_str(), ios::binary);
  if (!file) {
    cerr << "Bad Fenwick: " << filename << " does not exists\n";
    exit(-1);
  }

  file >> fenwick;
  return fenwick;
}

template <typename T>
void find(std::string name, size_t queries, std::mt19937 re) {
  using namespace std;
  using namespace std::chrono;
  using namespace hft::fenwick;
  T fenwick = file_to_fen<T>(name);

  chrono::high_resolution_clock::time_point begin, end;

  uint64_t u = 0;
  const double c = 1. / queries;
  constexpr size_t BOUND = 64, REPS = 5, IDXMID = (REPS - 1) / 2;
  uniform_int_distribution<uint64_t> cumseqdist(0, BOUND * fenwick.size());

  cout << "find: " << flush;
  vector<chrono::nanoseconds::rep> find;
  for (size_t r = 0; r < REPS; r++) {
    begin = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < queries; i++)
      u ^= fenwick.find(cumseqdist(re) ^ (u & 1));
    end = chrono::high_resolution_clock::now();
    find.push_back(duration_cast<chrono::nanoseconds>(end - begin).count());
  }
  sort(find.begin(), find.end());
  cout << find[IDXMID] * c << setw(12) << " ns/item" << endl;

  const volatile uint64_t __attribute__((unused)) unused = u;
}

template <typename T>
void add(std::string name, size_t queries, std::mt19937 re) {
  using namespace std;
  using namespace std::chrono;
  using namespace hft::fenwick;
  T fenwick = file_to_fen<T>(name);

  chrono::high_resolution_clock::time_point begin, end;

  uint64_t u = 0;
  const double c = 1. / queries;
  constexpr size_t BOUND = 64, REPS = 5, IDXMID = (REPS - 1) / 2;
  uniform_int_distribution<uint64_t> seqdist(0, BOUND);
  uniform_int_distribution<size_t> idxdist(1, fenwick.size());

  cout << "add: " << flush;
  vector<chrono::nanoseconds::rep> add;
  for (size_t r = 0; r < REPS; r++) {
    begin = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < queries; i++) {
      size_t idx = idxdist(re);
      fenwick.add(idx, i % 2 ? -seqdist(re) ^ (u & 1) : seqdist(re) ^ (u & 1));
    }
    end = chrono::high_resolution_clock::now();
    add.push_back(duration_cast<chrono::nanoseconds>(end - begin).count());
  }
  sort(add.begin(), add.end());
  cout << add[IDXMID] * c << setw(12) << " ns/item" << endl;

  // The add cannot be erased by the compiler
  u ^= fenwick.prefix(idxdist(re));
  const volatile uint64_t __attribute__((unused)) unused = u;
}

template <typename T>
void prefix(std::string name, size_t queries, std::mt19937 re) {
  using namespace std;
  using namespace std::chrono;
  using namespace hft::fenwick;
  T fenwick = file_to_fen<T>(name);

  chrono::high_resolution_clock::time_point begin, end;

  uint64_t u = 0;
  const double c = 1. / queries;
  constexpr size_t REPS = 5, IDXMID = (REPS - 1) / 2;
  uniform_int_distribution<size_t> idxdist(1, fenwick.size());

  cout << "prefix: " << flush;
  vector<chrono::nanoseconds::rep> prefix;
  for (size_t r = 0; r < REPS; r++) {
    begin = high_resolution_clock::now();
    for (uint64_t i = 0; i < queries; ++i)
      u ^= fenwick.prefix(idxdist(re) ^ (u & 1));
    end = high_resolution_clock::now();
    prefix.push_back(duration_cast<chrono::nanoseconds>(end - begin).count());
  }
  std::sort(prefix.begin(), prefix.end());
  cout << prefix[IDXMID] * c << setw(12) << " ns/item" << endl;

  const volatile uint64_t __attribute__((unused)) unused = u;
}
