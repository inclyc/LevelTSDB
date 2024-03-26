#pragma once

#include "Distribution.h"
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <random>
#include <tuple>

using std::chrono::duration_cast;
using std::chrono::nanoseconds;

#define TEST_STORAGE(TYPE, BATCH, ED_TYPE)                                     \
  Test<TYPE>::batchTest<ED_TYPE>(BATCH, #TYPE, #ED_TYPE)
#define SINGLE_ARG(...) __VA_ARGS__

#define TEST_DATASET(TYPE, DS_TYPE)                                            \
  Test<TYPE>::onSetTest<DS_TYPE>(#TYPE, #DS_TYPE)
#define SINGLE_ARG(...) __VA_ARGS__

namespace LevelTSDB {

[[nodiscard]] static auto
genTestCases(uint32_t numCases,
             std::uniform_int_distribution<std::size_t> distr) {
  std::random_device rd;
  std::mt19937 gen(rd());

  std::vector<std::tuple<std::size_t, std::size_t>> testCases;

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

[[nodiscard]] inline auto genTestCasesUniform(uint32_t numCases,
                                              std::size_t maxn) {
  return genTestCases(numCases,
                      std::uniform_int_distribution<std::size_t>(1, maxn));
}

template <class S> class Test {

public:
  static void validate(uint32_t numCases, std::size_t maxn, S &storage,
                       ElementDistribution<typename S::DataTy> &ED) {
    auto testCases = genTestCasesUniform(numCases, maxn);
    for (auto [l, r] : testCases) {
      auto left = storage.query(l, r);
      auto right = ED.sum(l, r);
      if (left != right) {
        std::cerr << "Failed: " << l << " " << r << std::endl;
        std::cerr << "left: " << left << "right: " << right << std::endl;
        exit(-1);
      }
    }
  }

  /// \returns time per query (ns), external invokes per query
  [[nodiscard]] static std::tuple<double, double>
  benchQuery(uint32_t numCases, std::size_t maxn, S &storage) {
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
  [[nodiscard]] static std::tuple<S, double>
  benchInsertion(uint32_t maxn, ElementDistribution<typename S::DataTy> &ED) {
    S storage;
    double nanoDuration = 0;
    for (uint32_t i = 1; i < maxn; i++) {
      auto data = ED.get(i);
      auto start = std::chrono::high_resolution_clock::now();
      storage.insert(data);
      auto end = std::chrono::high_resolution_clock::now();
      nanoDuration += duration_cast<nanoseconds>(end - start).count();
    }
    return {std::move(storage), static_cast<double>(nanoDuration) / maxn};
  }
  template <class ED_TYPE>
  static void batchTest(uint32_t scale, const char *name, const char *ed_type) {
    ED_TYPE ED;
    for (uint32_t maxn = 100, log10 = 2; log10 < scale; log10++, maxn *= 10) {
      auto [storage, insertionTime] = benchInsertion(maxn, ED);
      if (ED.shouldValidate())
        validate(10000, maxn, storage, ED);
      auto [queryTime, externalInvokes] = benchQuery(10000, maxn, storage);
      std::cout << insertionTime << "," << queryTime << "," << externalInvokes
                << "," << maxn << "," << "\"" << name << "\"" << "," << "\""
                << ed_type << "\"" << "\n";
    }
  }
  template <class DS_TYPE>
  static void onSetTest(const char *name, const char *ds_type) {
    DS_TYPE DS;
    auto maxn = DS.size();
    auto [storage, insertionTime] = benchInsertion(maxn, DS);
    auto [queryTime, externalInvokes] = benchQuery(10000, maxn, storage);
    std::cout << insertionTime << "," << queryTime << "," << externalInvokes
              << "," << maxn << "," << "\"" << name << "\"" << "," << "\""
              << ds_type << "\"" << "\n";
  }
};

} // namespace LevelTSDB
