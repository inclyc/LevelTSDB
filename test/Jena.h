#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <ctime>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "DataSet.h"

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

template <uint32_t Pos, const char *Path>
class Jena : public LevelTSDB::DataSet<double> {
  static_assert(Pos <= 13, "Element position excceed");

public:
  Jena() {
    std::ifstream file(Path);
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
  [[nodiscard]] double get(std::size_t x) override { return data[x][Pos]; }
  [[noreturn]] double sum([[maybe_unused]] std::size_t l,
                          [[maybe_unused]] std::size_t r) override {
    __builtin_unreachable();
  }
  [[nodiscard]] std::size_t size() override { return data.size(); }
  [[gnu::pure]] bool shouldValidate() override { return false; }

private:
  std::vector<JenaEntry> data;
};
} // namespace Jena
