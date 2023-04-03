#include "Data.h"
#include "Map.h"
#include "Test.h"

int main() {
  using namespace LevelTSDB; // Test for different disturbutions

  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, LruMap<uint64_t, 1000>>), 8,
               SINGLE_ARG(RandomDistribution<uint64_t>));
  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, LruMap<uint64_t, 10000>>), 8,
               SINGLE_ARG(RandomDistribution<uint64_t>));

  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, LruMap<uint64_t, 1000>>), 8,
               SINGLE_ARG(SineDistribution<uint64_t, 100, 10000>));
  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, LruMap<uint64_t, 10000>>), 8,
               SINGLE_ARG(SineDistribution<uint64_t, 100, 10000>));

  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, LruMap<uint64_t, 1000>>), 8,
               SINGLE_ARG(GaussianDistribution<uint64_t, 100, 10000>));
  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, LruMap<uint64_t, 10000>>), 8,
               SINGLE_ARG(GaussianDistribution<uint64_t, 100, 10000>));

  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, LruMap<uint64_t, 1000>>), 8,
               SINGLE_ARG(ExpDistribution<uint64_t, 4, 1000, 1000>));
  TEST_STORAGE(SINGLE_ARG(Storage<uint64_t, LruMap<uint64_t, 10000>>), 8,
               SINGLE_ARG(ExpDistribution<uint64_t, 4, 1000, 1000>));
}
