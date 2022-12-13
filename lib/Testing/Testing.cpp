#include <iostream>
#include <sys/resource.h>

void printMemoryUsage() {
  struct rusage usage;
  if (!getrusage(RUSAGE_SELF, &usage)) {
    std::cout << "Maximum resident set size (KB): " << usage.ru_maxrss
              << std::endl;
  } else {
    std::cerr << "getrusage";
    exit(1);
  }
}
