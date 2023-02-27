#include "Data.h"
#include <cassert>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>

using std::chrono::duration_cast;
using std::chrono::nanoseconds;
template <class S> class Test {

public:
  [[nodiscard]] static std::vector<std::tuple<uint32_t, uint32_t>>
  genTestCases(uint32_t numCases,
               std::uniform_int_distribution<uint64_t> distr) {
    std::random_device rd;
    std::mt19937 gen(rd());

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

    return testCases;
  }

  [[nodiscard]] static std::vector<std::tuple<uint32_t, uint32_t>>
  genTestCasesUniform(uint32_t numCases, uint32_t maxn) {
    return genTestCases(numCases,
                        std::uniform_int_distribution<uint64_t>(1, maxn));
  }
  static void testCorrect(uint32_t numCases, uint32_t maxn, S &storage) {
    auto testCases = genTestCasesUniform(numCases, maxn);
    for (auto [l, r] : testCases) {
      auto left = storage.query(l, r);
      auto right = static_cast<decltype(left)>((r - l)) * (l + r - 1) / 2;
      if (left != right) {
        std::cerr << "Failed: " << l << " " << r << std::endl;
        std::cerr << "left: " << left << "right: " << right << std::endl;
        exit(-1);
      }
    }
  }

  /// \returns time per query (ns), external invokes per query
  [[nodiscard]] static std::tuple<double, double>
  benchQuery(uint32_t numCases, uint32_t maxn, S &storage) {
    storage.resetCacheMiss();
    auto testCases = genTestCasesUniform(numCases, maxn);
    auto start = std::chrono::high_resolution_clock::now();
    for (auto [l, r] : testCases)
      storage.query(l, r);

    auto end = std::chrono::high_resolution_clock::now();
    auto nanoDuration = duration_cast<nanoseconds>(end - start).count();
    return {static_cast<double>(nanoDuration) / numCases,
            static_cast<double>(storage.cacheMiss()) / numCases};
  }

  /// \returns the storage and average time per insertion (ns)
  [[nodiscard]] static std::tuple<S, double> benchInsertion(uint32_t maxn) {
    S storage;
    auto start = std::chrono::high_resolution_clock::now();
    for (uint32_t i = 1; i < maxn; i++) {
      storage.insert(i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto nanoDuration = duration_cast<nanoseconds>(end - start).count();
    return {std::move(storage), static_cast<double>(nanoDuration) / maxn};
  }

  static void batchTest(uint32_t scale, const char *name) {
    for (uint32_t maxn = 100, log10 = 2; log10 < scale; log10++, maxn *= 10) {
      auto [storage, insertionTime] = benchInsertion(maxn);
      testCorrect(10000, maxn, storage);
      auto [queryTime, externalInvokes] = benchQuery(10000, maxn, storage);
      std::cout << insertionTime << "," << queryTime << "," << externalInvokes
                << "," << maxn << "," << name << "\n";
    }
  }
};

int main() {
  using LevelTSDB::ArrayMap;
  using LevelTSDB::LruMap;
  using LevelTSDB::Map;
  using LevelTSDB::Storage;

  std::cout
      << "Insertion (ns), Query (ns), External Invokes, Dataset Size, Name"
      << "\n";

#define TEST_STORAGE(TYPE, BATCH) Test<TYPE>::batchTest(BATCH, #TYPE);
#define SINGLE_ARG(...) __VA_ARGS__

  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, ArrayMap<uint64_t, 100>>), 8);
  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, ArrayMap<uint64_t, 1000>>), 8);
  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, ArrayMap<uint64_t, 10000>>), 8);

  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, LruMap<uint64_t, 1000>>), 8);
  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, LruMap<uint64_t, 10000>>), 8);

  // Test for different disturbutions

  return 0;
}
