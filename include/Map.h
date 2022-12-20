#pragma once

#include <cstdint>
#include <vector>

namespace LevelTSDB {

using std::size_t;

template <class T> class Map {
public:
  T &operator[](size_t);
};

template <class T> class ArrayMap : public Map<T> {
  std::vector<T> data;

public:
  T &operator[](size_t x) override {
    if (data.size() <= x) {
      data.resize(2 * x);
      data[x] = T();
    }
    return data[x];
  }
};

} // namespace LevelTSDB
