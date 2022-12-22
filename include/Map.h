#pragma once

#include <cstddef>
#include <list>
#include <unordered_map>
#include <vector>

namespace LevelTSDB {

using std::size_t;

template <class T> class Map {
public:
  void set(size_t, T);
  T get(size_t);
};

template <class T> class ArrayMap : public Map<T> {
  std::vector<T> data;

private:
  T &operator[](size_t x) {
    if (data.size() <= x) {
      data.resize(2 * x);
      data[x] = T();
    }
    return data[x];
  }

public:
  T get(size_t x) { return this->operator[](x); }
  void set(size_t x, T value) { this->operator[](x) = value; }
};

template <class T, class B> class Lru {
private:
  struct Node {
    size_t key;
    T value;
    Node(size_t key, T value) : key(key), value(value) {}
  };

  size_t cap;

  std::list<Node> list;
  std::unordered_map<size_t, typename std::list<Node>::iterator> mp;

  static_assert(std::is_base_of<LevelTSDB::Map<T>, B>());
  B base;

public:
  Lru(size_t cap) : cap(cap) {}
  void set(size_t key, const T value) {
    auto it = mp.find(key);
    if (it == mp.end()) {
      if (list.size() == cap) {
        auto key = list.back().key;
        auto value = list.back().value;
        mp.erase(key);
        base.set(key, value);
        list.pop_back();
      }
      list.push_front(Node(key, value));
      mp[key] = list.begin();
    } else {
      list.erase(it->second);
      list.push_front(Node(key, value));
      mp[key] = list.begin();
    }
  }
  T get(size_t key) {
    auto it = mp.find(key);
    if (it == mp.end()) {
      auto value = base.get(key);
      if (list.size() == cap) {
        base.set(list.back().key, list.back().value);
        mp.erase(list.back().key);
        list.pop_back();
      }
      list.push_front(Node(key, value));
      mp[key] = list.begin();
      return value;
    }
    {
      auto value = (*it->second).value;
      list.erase(it->second);
      list.push_front(Node(key, value));
      mp[key] = list.begin();
      return value;
    }
  }
};

template <class T, int N> class LruMap : public Map<T> {
  Lru<T, ArrayMap<T>> cache = Lru<T, ArrayMap<T>>(N);

public:
  T get(size_t key) { return cache.get(key); }
  void set(size_t key, T value) { cache.set(key, value); }
};

}; // namespace LevelTSDB
