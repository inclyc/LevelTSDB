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

template <class T>
class NaturalNumbersDistribution : public ElementDistribution<T> {
public:
  [[gnu::pure]] [[nodiscard]] T get(std::size_t x) override {
    return static_cast<T>(x);
  }
  [[gnu::pure]] [[nodiscard]] T sum(std::size_t l, std::size_t r) override {
    T ll = static_cast<T>(l);
    T rr = static_cast<T>(r);
    return (rr - ll) * (rr + ll - 1) / 2;
  }
  [[gnu::pure]] bool shouldValidate() override { return true; }

  virtual ~NaturalNumbersDistribution() = default;
};

template <class T> class RandomDistribution : public ElementDistribution<T> {
public:
  [[nodiscard]] T get(std::size_t x) override {
    return static_cast<T>(rand() % x);
  }
  [[noreturn]] T sum([[maybe_unused]] std::size_t l,
                     [[maybe_unused]] std::size_t r) override {
    __builtin_unreachable();
  }
  [[gnu::pure]] bool shouldValidate() override { return false; }

  virtual ~RandomDistribution() = default;
};
template <class T, int A, int O>
class SineDistribution : public ElementDistribution<T> {
public:
  [[nodiscard]] T get(std::size_t x) override {
    return static_cast<T>(A * sin(x) + O);
  }
  [[noreturn]] T sum([[maybe_unused]] std::size_t l,
                     [[maybe_unused]] std::size_t r) override {
    __builtin_unreachable();
  }
  [[gnu::pure]] bool shouldValidate() override { return false; }

  virtual ~SineDistribution() = default;
};

template <class T, int S, int M>
class GaussianDistribution : public ElementDistribution<T> {
public:
  [[nodiscard]] T get(std::size_t x) override {
    return static_cast<T>((1 / (S * sqrt(2 * M_PI))) *
                          exp(-0.5 * pow((x - M) / S, 2.0)));
  }
  [[noreturn]] T sum([[maybe_unused]] std::size_t l,
                     [[maybe_unused]] std::size_t r) override {
    __builtin_unreachable();
  }
  [[gnu::pure]] bool shouldValidate() override { return false; }

  virtual ~GaussianDistribution() = default;
};

template <class T, int L, int O, int A>
class ExpDistribution : public ElementDistribution<T> {
public:
  [[nodiscard]] T get(std::size_t x) override {
    return static_cast<T>(O + A * exp(-L * x));
  }
  [[noreturn]] T sum([[maybe_unused]] std::size_t l,
                     [[maybe_unused]] std::size_t r) override {
    __builtin_unreachable();
  }
  [[gnu::pure]] bool shouldValidate() override { return false; }

  virtual ~ExpDistribution() = default;
};

} // namespace LevelTSDB
