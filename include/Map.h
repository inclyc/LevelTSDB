#pragma once

#include <cstddef>
#include <cstdint>
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

template <class T, int N = 999999> class ArrayMap : public Map<T> {
  std::vector<T> data;
  uint32_t _cacheMiss;

private:
  T &operator[](size_t x) {
    if (data.size() <= x) {
      data.resize(2 * x);
      data[x] = T();
    }
    if (data.size() > N && x < data.size() - N)
      _cacheMiss++;
    return data[x];
  }

public:
  ArrayMap() : _cacheMiss(0) {}
  T get(size_t x) { return this->operator[](x); }
  void set(size_t x, T value) { this->operator[](x) = value; }
  uint32_t &cacheMiss() { return _cacheMiss; }
  const uint32_t &cacheMiss() const { return _cacheMiss; }
};

template <class T, class B> class Lru {
private:
  struct Node {
    size_t key;
    T value;
    Node(size_t key, T value) : key(key), value(value) {}
  };

  size_t cap;

  uint32_t _cacheMiss;

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
        auto bKey = list.back().key;
        auto bValue = list.back().value;
        mp.erase(bKey);
        base.set(bKey, bValue);
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
      _cacheMiss++;
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
  uint32_t &cacheMiss() { return _cacheMiss; }
  const uint32_t &cacheMiss() const { return _cacheMiss; }
};

template <class T, int N> class LruMap : public Map<T> {
  Lru<T, ArrayMap<T>> cache = Lru<T, ArrayMap<T>>(N);

public:
  T get(size_t key) { return cache.get(key); }
  void set(size_t key, T value) { cache.set(key, value); }
  uint32_t &cacheMiss() { return cache.cacheMiss(); }
  const uint32_t &cacheMiss() const { return cache.cacheMiss(); }
};

} // namespace LevelTSDB
