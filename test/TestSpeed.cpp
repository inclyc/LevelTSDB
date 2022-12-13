#include "Data.h"
#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>

typedef Storage<uint64_t> TestStorage;

int main(void)
{
  const int maxn = 1e8;
  const int cases_num = 1e3; 
  TestStorage storage;
  
  {
    auto t_start = std::chrono::high_resolution_clock::now();
    for (int i = 1; i < maxn; i++) {
      storage.insert(i);
    }
    auto t_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<long long, std::nano> during = ( t_end - t_start );
    std::cout << during.count() / maxn << "ns" << std::endl;
  }
  
  {
    std::random_device rd;  // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<uint64_t> distr(1, maxn); // define the range

    auto t_start = std::chrono::high_resolution_clock::now();
    for (uint32_t i = 0; i < cases_num; i++) {
      auto [l, r] = std::tuple{distr(gen), distr(gen)};
      if (l > r) {
	std::swap(l, r);
      } else if (l == r) {
	i--;
	continue;
      }
    }
    auto t_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<long long, std::nano> during = ( t_end - t_start );
    std::cout << during.count() / cases_num << "ns" << std::endl;
  }
}
