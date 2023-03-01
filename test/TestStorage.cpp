#include "Data.h"
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <random>

using std::chrono::duration_cast;
using std::chrono::nanoseconds;
namespace {

template <class T> class ElementDistribution {
public:
  /// \returns The element at timestamp "x".
  virtual T get(std::size_t x) = 0;

  /// \returns The sum between [l, r) for validator.
  virtual T sum(std::size_t l, std::size_t r) = 0;

  /// \returns If the result should be validated.
  virtual bool shouldValidate() = 0;

  virtual ~ElementDistribution() = default;
};

template <class T>
class NaturalNumbersDistribution : public ElementDistribution<T> {
public:
  [[gnu::pure]] [[nodiscard]] T get(std::size_t x) override {
    return static_cast<T>(x);
  }
  [[gnu::pure]] [[nodiscard]] T sum(std::size_t l, std::size_t r) override {
    T ll = static_cast<T>(l);
    T rr = static_cast<T>(r);
    return (rr - ll) * (rr + ll - 1) / 2;
  }
  [[gnu::pure]] bool shouldValidate() override { return true; }

  virtual ~NaturalNumbersDistribution() = default;
};

template <class T> class RandomDistribution : public ElementDistribution<T> {
public:
  [[nodiscard]] T get(std::size_t x) override {
    return static_cast<T>(rand() % x);
  }
  [[noreturn]] T sum([[maybe_unused]] std::size_t l,
                     [[maybe_unused]] std::size_t r) override {
    __builtin_unreachable();
  }
  [[gnu::pure]] bool shouldValidate() override { return false; }

  virtual ~RandomDistribution() = default;
};
template <class T, int A, int O>
class SineDistribution : public ElementDistribution<T> {
public:
  [[nodiscard]] T get(std::size_t x) override {
    return static_cast<T>(A * sin(x) + O);
  }
  [[noreturn]] T sum([[maybe_unused]] std::size_t l,
                     [[maybe_unused]] std::size_t r) override {
    __builtin_unreachable();
  }
  [[gnu::pure]] bool shouldValidate() override { return false; }

  virtual ~SineDistribution() = default;
};

template <class T, int S, int M>
class GaussianDistribution : public ElementDistribution<T> {
public:
  [[nodiscard]] T get(std::size_t x) override {
    return static_cast<T>((1 / (S * sqrt(2 * M_PI))) *
                          exp(-0.5 * pow((x - M) / S, 2.0)));
  }
  [[noreturn]] T sum([[maybe_unused]] std::size_t l,
                     [[maybe_unused]] std::size_t r) override {
    __builtin_unreachable();
  }
  [[gnu::pure]] bool shouldValidate() override { return false; }

  virtual ~GaussianDistribution() = default;
};

template <class T, int L, int O, int A>
class ExpDistribution : public ElementDistribution<T> {
public:
  [[nodiscard]] T get(std::size_t x) override {
    return static_cast<T>(O + A * exp(-L * x));
  }
  [[noreturn]] T sum([[maybe_unused]] std::size_t l,
                     [[maybe_unused]] std::size_t r) override {
    __builtin_unreachable();
  }
  [[gnu::pure]] bool shouldValidate() override { return false; }

  virtual ~ExpDistribution() = default;
};

[[nodiscard]] auto
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

[[nodiscard]] static auto genTestCasesUniform(uint32_t numCases,
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
    auto start = std::chrono::high_resolution_clock::now();
    for (uint32_t i = 1; i < maxn; i++) {
      storage.insert(ED.get(i));
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto nanoDuration = duration_cast<nanoseconds>(end - start).count();
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
                << "," << maxn << ","
                << "\"" << name << "\""
                << ","
                << "\"" << ed_type << "\""
                << "\n";
    }
  }
};

} // namespace

int main() {
  using LevelTSDB::ArrayMap;
  using LevelTSDB::LruMap;
  using LevelTSDB::Map;
  using LevelTSDB::Storage;

  std::cout << "Insertion (ns),Query (ns),External Invokes,Dataset "
               "Size,Name,Distribution Type"
            << "\n";

#define TEST_STORAGE(TYPE, BATCH, ED_TYPE)                                     \
  Test<TYPE>::batchTest<ED_TYPE>(BATCH, #TYPE, #ED_TYPE)
#define SINGLE_ARG(...) __VA_ARGS__

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

  // Test for different disturbutions

  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, ArrayMap<uint64_t, 100>>), 8,
               SINGLE_ARG(RandomDistribution<uint64_t>));
  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, ArrayMap<uint64_t, 1000>>), 8,
               SINGLE_ARG(RandomDistribution<uint64_t>));
  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, ArrayMap<uint64_t, 10000>>), 8,
               SINGLE_ARG(RandomDistribution<uint64_t>));

  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, LruMap<uint64_t, 1000>>), 8,
               SINGLE_ARG(RandomDistribution<uint64_t>));
  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, LruMap<uint64_t, 10000>>), 8,
               SINGLE_ARG(RandomDistribution<uint64_t>));

  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, ArrayMap<uint64_t, 100>>), 8,
               SINGLE_ARG(SineDistribution<uint64_t, 100, 10000>));
  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, ArrayMap<uint64_t, 1000>>), 8,
               SINGLE_ARG(SineDistribution<uint64_t, 100, 10000>));
  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, ArrayMap<uint64_t, 10000>>), 8,
               SINGLE_ARG(SineDistribution<uint64_t, 100, 10000>));

  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, LruMap<uint64_t, 1000>>), 8,
               SINGLE_ARG(SineDistribution<uint64_t, 100, 10000>));
  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, LruMap<uint64_t, 10000>>), 8,
               SINGLE_ARG(SineDistribution<uint64_t, 100, 10000>));

  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, ArrayMap<uint64_t, 100>>), 8,
               SINGLE_ARG(GaussianDistribution<uint64_t, 100, 10000>));
  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, ArrayMap<uint64_t, 1000>>), 8,
               SINGLE_ARG(GaussianDistribution<uint64_t, 100, 10000>));
  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, ArrayMap<uint64_t, 10000>>), 8,
               SINGLE_ARG(GaussianDistribution<uint64_t, 100, 10000>));

  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, LruMap<uint64_t, 1000>>), 8,
               SINGLE_ARG(GaussianDistribution<uint64_t, 100, 10000>));
  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, LruMap<uint64_t, 10000>>), 8,
               SINGLE_ARG(GaussianDistribution<uint64_t, 100, 10000>));

  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, ArrayMap<uint64_t, 100>>), 8,
               SINGLE_ARG(ExpDistribution<uint64_t, 4, 1000, 1000>));
  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, ArrayMap<uint64_t, 1000>>), 8,
               SINGLE_ARG(ExpDistribution<uint64_t, 4, 1000, 1000>));
  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, ArrayMap<uint64_t, 10000>>), 8,
               SINGLE_ARG(ExpDistribution<uint64_t, 4, 1000, 1000>));

  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, LruMap<uint64_t, 1000>>), 8,
               SINGLE_ARG(ExpDistribution<uint64_t, 4, 1000, 1000>));
  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, LruMap<uint64_t, 10000>>), 8,
               SINGLE_ARG(ExpDistribution<uint64_t, 4, 1000, 1000>));
  return 0;
}
