#pragma once

#include <cstdint>

#include "Distribution.h"

namespace LevelTSDB {
  template <class T> class DataSet :public ElementDistribution<T> {
public:
  /// \returns The element at timestamp "x".
  virtual T get(std::size_t x) = 0;

  /// \returns The sum between [l, r) for validator.
  virtual T sum(std::size_t l, std::size_t r) = 0;

  /// \returns If the result should be validated.
  virtual bool shouldValidate() = 0;

  virtual std::size_t size() = 0;

  virtual ~DataSet() = default;
};

} // namespace LevelTSDB
