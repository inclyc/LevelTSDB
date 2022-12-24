#include "Data.h"
#include "Testing/Testing.h"
#include <cassert>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>
#include <sys/resource.h>

template <class S> class Test {
public:
  static void testCorrect(uint32_t numCases, int maxn, S &storage) {
    std::random_device rd;  // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<uint64_t> distr(1, maxn); // define the range

    for (uint32_t i = 0; i < numCases; i++) {
      auto [l, r] = std::tuple{distr(gen), distr(gen)};
      if (l > r)
        std::swap(l, r);
      else if (l == r)
        continue;
      if (storage.query(l, r) != (r - l) * (l + r - 1) / 2) {
        std::cerr << "Failed :" << l << " " << r << std::endl;
        exit(-1);
      }
    }
    std::cout << "Correction test passed." << std::endl;
  }

  static void benchQuery(uint32_t numCases, int maxn, S &storage) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> distr(1, maxn);

    storage.resetCacheMiss();

    std::vector<std::tuple<uint32_t, uint32_t>> testCases;

    for (uint32_t i = 1; i <= numCases; i++) {
      auto [l, r] = std::tuple{distr(gen), distr(gen)};
      if (l > r) {
        std::swap(l, r);
      } else if (l == r) {
        i--;
        continue;
      }
      testCases.push_back({l, r});
    }
    auto start = std::chrono::high_resolution_clock::now();
    for (auto [l, r] : testCases) {
      storage.query(l, r);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = end - start;
    std::cout << "Average time per query (ns): "
              << static_cast<double>(
                     std::chrono::duration_cast<std::chrono::nanoseconds>(
                         duration)
                         .count()) /
                     numCases
              << std::endl;

    std::cout << "Cache misses per query: "
              << static_cast<double>(storage.cacheMiss()) / numCases
              << std::endl;
  }

  static S benchInsertion(int maxn) {
    S storage;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 1; i < maxn; i++) {
      storage.insert(i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = end - start;
    std::cout << "Average time per insertion (ns): "
              << static_cast<double>(
                     std::chrono::duration_cast<std::chrono::nanoseconds>(
                         duration)
                         .count()) /
                     maxn
              << std::endl;
    return storage;
  }

  static void batchTest(uint32_t scale) {
    for (uint32_t maxn = 100, log10 = 2; log10 < scale; log10++, maxn *= 10) {
      std::cout << "Dataset size: " << maxn << std::endl;
      auto storage = benchInsertion(maxn);
      testCorrect(10000, maxn, storage);
      benchQuery(10000, maxn, storage);
      printMemoryUsage();
      std::cout << std::endl;
    }
  }
};

int main() {
  using LevelTSDB::ArrayMap;
  using LevelTSDB::LruMap;
  using LevelTSDB::Map;
  using LevelTSDB::Storage;

#define TEST_STORAGE(TYPE, BATCH)                                              \
  std::cout << "Testing " #TYPE << "\n";                                       \
  Test<TYPE>::batchTest(BATCH);                                                \
  std::cout << "\n\n";
#define SINGLE_ARG(...) __VA_ARGS__

  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, ArrayMap<uint64_t, 10000>>), 9);
  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, LruMap<uint64_t, 10000>>), 8);
  return 0;
}
