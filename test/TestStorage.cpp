#include "Data.h"
#include <cassert>
#include <cstdint>
#include <iostream>
#include <random>
#include <sys/resource.h>

typedef Storage<uint64_t> TestStorage;

void testCorrect(uint32_t numCases, int maxn, const TestStorage &storage) {
  std::random_device rd;  // obtain a random number from hardware
  std::mt19937 gen(rd()); // seed the generator
  std::uniform_int_distribution<uint64_t> distr(1, maxn); // define the range

  for (uint32_t i = 0; i < numCases; i++) {
    auto [l, r] = std::tuple{distr(gen), distr(gen)};
    if (l > r) {
      std::swap(l, r);
    } else if (l == r) {
      continue;
    }
    if (storage.query(l, r) != (r - l) * (l + r - 1) / 2) {
      std::cerr << "Failed :" << l << " " << r << std::endl;
      exit(-1);
    }
  }
  std::cout << "Correction test passed." << std::endl;
}

int main() {
  const int maxn = 1e8;
  TestStorage storage;
  for (int i = 1; i < maxn; i++) {
    storage.insert(i);
  }

  testCorrect(10000, maxn, storage);
  struct rusage usage;

  if (!getrusage(RUSAGE_SELF, &usage)) {
    std::cout << "Maximum resident set size (KB): " << usage.ru_maxrss
              << std::endl;
  } else {
    std::cerr << "getrusage";
    exit(1);
  }

  return 0;
}
