#include "Data.h"
#include "Jena.h"
#include "Map.h"
#include "Test.h"

const char JENA_PATH[] = "/tmp/jena_climate_2009_2016.csv";
int main() {
  using namespace LevelTSDB; // Test for different disturbutions
  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 100>>),
               SINGLE_ARG(Jena::Jena<0, JENA_PATH>));
  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 1000>>),
               SINGLE_ARG(Jena::Jena<0, JENA_PATH>));
  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 10000>>),
               SINGLE_ARG(Jena::Jena<0, JENA_PATH>));

  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 100>>),
               SINGLE_ARG(Jena::Jena<1, JENA_PATH>));
  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 1000>>),
               SINGLE_ARG(Jena::Jena<1, JENA_PATH>));
  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 10000>>),
               SINGLE_ARG(Jena::Jena<1, JENA_PATH>));

  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 100>>),
               SINGLE_ARG(Jena::Jena<2, JENA_PATH>));
  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 1000>>),
               SINGLE_ARG(Jena::Jena<2, JENA_PATH>));
  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 10000>>),
               SINGLE_ARG(Jena::Jena<2, JENA_PATH>));

  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 100>>),
               SINGLE_ARG(Jena::Jena<3, JENA_PATH>));
  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 1000>>),
               SINGLE_ARG(Jena::Jena<3, JENA_PATH>));
  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 10000>>),
               SINGLE_ARG(Jena::Jena<3, JENA_PATH>));

  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 100>>),
               SINGLE_ARG(Jena::Jena<4, JENA_PATH>));
  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 1000>>),
               SINGLE_ARG(Jena::Jena<4, JENA_PATH>));
  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 10000>>),
               SINGLE_ARG(Jena::Jena<4, JENA_PATH>));

  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 100>>),
               SINGLE_ARG(Jena::Jena<5, JENA_PATH>));
  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 1000>>),
               SINGLE_ARG(Jena::Jena<5, JENA_PATH>));
  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 10000>>),
               SINGLE_ARG(Jena::Jena<5, JENA_PATH>));

  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 100>>),
               SINGLE_ARG(Jena::Jena<6, JENA_PATH>));
  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 1000>>),
               SINGLE_ARG(Jena::Jena<6, JENA_PATH>));
  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 10000>>),
               SINGLE_ARG(Jena::Jena<6, JENA_PATH>));

  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 100>>),
               SINGLE_ARG(Jena::Jena<7, JENA_PATH>));
  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 1000>>),
               SINGLE_ARG(Jena::Jena<7, JENA_PATH>));
  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 10000>>),
               SINGLE_ARG(Jena::Jena<7, JENA_PATH>));

  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 100>>),
               SINGLE_ARG(Jena::Jena<8, JENA_PATH>));
  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 1000>>),
               SINGLE_ARG(Jena::Jena<8, JENA_PATH>));
  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 10000>>),
               SINGLE_ARG(Jena::Jena<8, JENA_PATH>));

  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 100>>),
               SINGLE_ARG(Jena::Jena<9, JENA_PATH>));
  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 1000>>),
               SINGLE_ARG(Jena::Jena<9, JENA_PATH>));
  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 10000>>),
               SINGLE_ARG(Jena::Jena<9, JENA_PATH>));

  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 100>>),
               SINGLE_ARG(Jena::Jena<10, JENA_PATH>));
  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 1000>>),
               SINGLE_ARG(Jena::Jena<10, JENA_PATH>));
  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 10000>>),
               SINGLE_ARG(Jena::Jena<10, JENA_PATH>));

  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 100>>),
               SINGLE_ARG(Jena::Jena<11, JENA_PATH>));
  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 1000>>),
               SINGLE_ARG(Jena::Jena<11, JENA_PATH>));
  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 10000>>),
               SINGLE_ARG(Jena::Jena<11, JENA_PATH>));

  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 100>>),
               SINGLE_ARG(Jena::Jena<12, JENA_PATH>));
  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 1000>>),
               SINGLE_ARG(Jena::Jena<12, JENA_PATH>));
  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 10000>>),
               SINGLE_ARG(Jena::Jena<12, JENA_PATH>));

  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 100>>),
               SINGLE_ARG(Jena::Jena<13, JENA_PATH>));
  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 1000>>),
               SINGLE_ARG(Jena::Jena<13, JENA_PATH>));
  TEST_DATASET(SINGLE_ARG(Storage<double, ArrayMap<double, 10000>>),
               SINGLE_ARG(Jena::Jena<13, JENA_PATH>));
}
