#include "Data.h"
#include "Distribution.h"
#include "Test.h"
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <random>

using std::chrono::nanoseconds;

int main() {
  using namespace LevelTSDB;

  std::cout << "Insertion (ns),Query (ns),External Invokes,Dataset "
               "Size,Name,Distribution Type"
            << "\n";

  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, ArrayMap<uint64_t, 100>>), 8,
               SINGLE_ARG(NaturalNumbersDistribution<uint64_t>));
  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, ArrayMap<uint64_t, 1000>>), 8,
               SINGLE_ARG(NaturalNumbersDistribution<uint64_t>));
  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, ArrayMap<uint64_t, 10000>>), 8,
               SINGLE_ARG(NaturalNumbersDistribution<uint64_t>));

  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, LruMap<uint64_t, 1000>>), 8,
               SINGLE_ARG(NaturalNumbersDistribution<uint64_t>));
  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, LruMap<uint64_t, 10000>>), 8,
               SINGLE_ARG(NaturalNumbersDistribution<uint64_t>));
  return 0;
}
