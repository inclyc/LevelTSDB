#pragma once

#include <cassert>
#include <cmath>
#include <cstdint>
#include <random>

namespace LevelTSDB {

template <class T> class ElementDistribution {
public:
  /// \returns The element at timestamp "x".
  virtual T get(std::size_t x) = 0;

  /// \returns The sum between [l, r) for validator.
  virtual T sum(std::size_t l, std::size_t r) = 0;

  /// \returns If the result should be validated.
  virtual bool shouldValidate() = 0;

  virtual ~ElementDistribution() = default;
};
  class JenaDegree : public ElementDistribution<float> {
  public:
    [[nodiscard]] float get(std::size_t x [[maybe_unused]]) override {
      return 0;
    }
    [[noreturn]] float sum([[maybe_unused]] std::size_t l,
		       [[maybe_unused]] std::size_t r) override {
      __builtin_unreachable();
    }
    [[gnu::pure]] bool shouldValidate() override { return false; }

    virtual ~JenaDegree() = default;
  };
} // namespace LevelTSDB 
