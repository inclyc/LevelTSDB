#pragma once

#include "Map.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <deque>
#include <vector>

namespace LevelTSDB {

using std::size_t;

template <class T, class Map> class Storage {
  std::vector<Map> data;
  size_t maxtime;

  static_assert(std::is_base_of_v<LevelTSDB::Map<T>, Map>);

public:
  typedef T DataTy;
  Storage() {
    data.resize(64);
    maxtime = 1;
  }

  void insert(T value) {
    for (size_t lvl = 0, idx = maxtime; idx && lvl < 64; lvl++, idx >>= 1) {
      auto &line = data[lvl];
      line.set(idx, line.get(idx) + value);
    }
    maxtime++;
  }

  std::tuple<size_t, T> forwarding(std::size_t l, std::size_t r) {
    size_t level = std::min(static_cast<size_t>(__builtin_ctz(l)),
                            static_cast<size_t>((log(r - l) / log(2))));
    return {1 << level, data[level].get(l >> level)};
  }

  T query(std::size_t l, std::size_t r) {
    if (l >= r) {
      return T();
    }
    auto [p, v] = forwarding(l, r);
    return query(l + p, r) + v;
  }

  uint32_t cacheMiss() {
    uint32_t sum = 0;
    for (int i = 0; i < 64; i++)
      sum += data[i].cacheMiss();
    return sum;
  }
  void resetCacheMiss() {
    for (int i = 0; i < 64; i++) {
      data[i].cacheMiss() = 0;
    }
  }
};

} // namespace LevelTSDB
