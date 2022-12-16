#pragma once

#include <algorithm>
#include <cmath>
#include <deque>
#include <vector>

using std::size_t;

template <class T> class Storage {
  std::vector<std::deque<T>> data;
  size_t maxtime;

public:
  Storage() {
    data.resize(64);
    maxtime = 1;
  }

  void insert(T value) {
    for (size_t lvl = 0, idx = maxtime; idx && lvl < 64; lvl++, idx >>= 1) {
      auto &line = data[lvl];
      if (line.size() + 1 == idx) {
        line.push_back(value);
      } else {
        line[idx - 1] += value;
      }
    }
    maxtime++;
  }

  std::tuple<size_t, T> forwarding(std::size_t l, std::size_t r) const {
    size_t level = std::min(static_cast<size_t>(__builtin_ctz(l)),
                            static_cast<size_t>((log(r - l) / log(2))));
    return {1 << level, data[level][(l >> level) - 1]};
  }

  T query(std::size_t l, std::size_t r) const {
    if (l >= r) {
      return T();
    }
    auto [p, v] = forwarding(l, r);
    return query(l + p, r) + v;
  }
};
