#pragma once

#include <cassert>
#include <ctime>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace Jena {

typedef std::vector<double> JenaEntry;

/* //perhaps we can use it later?
struct JenaEntry {
    double p;
    double T;
    double Tpot;
    double Tdew;
    double rh;
    double VPmax;
    double VPact;
    double VPdef;
    double sh;
    double H2OC;
    double rho;
    double wv;
    double max_mn;
    double wd;
  };
*/

class Jena {
public:
  Jena(const std::string &path) {
    std::ifstream file(path);
    std::string line;
    auto split = [](const std::string &s, char delimiter) {
      std::vector<std::string> tokens;
      std::string token;
      std::istringstream tokenStream(s);
      while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
      }
      return tokens;
    };

    // ignore the first line
    std::getline(file, line);

    while (std::getline(file, line)) {
      std::vector<std::string> row = split(line, ',');
      JenaEntry tmp;
      for (int i(1); i <= 14; i++)
        tmp.push_back(std::stod(row[i]));
      data.push_back(tmp);
    }
  }

private:
  std::vector<JenaEntry> data;
};

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
  [[nodiscard]] float get(std::size_t x [[maybe_unused]]) override { return 0; }
  [[noreturn]] float sum([[maybe_unused]] std::size_t l,
                         [[maybe_unused]] std::size_t r) override {
    __builtin_unreachable();
  }
  [[gnu::pure]] bool shouldValidate() override { return false; }

  virtual ~JenaDegree() = default;
};
} // namespace Jena
