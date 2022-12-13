#include <algorithm>
#include <bitset>
#include <cmath>
#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

namespace aoc_util
{

template <typename T>
int signum(T val) {
  return (T(0) < val) - (val < T(0));
}

template <typename T>
void sleep(T time) {
  std::this_thread::sleep_for(time);
}

void ClearScreen() {
  std::cout << std::string(100, '\n');
}

const auto get_long  = [](const auto &el) { return std::stol(el); };
const auto get_llong = [](const auto &el) { return std::stoll(el); };
const auto get_int   = [](const auto &el) { return std::stoi(el); };

template <typename T>
using matrix = std::vector<std::vector<T>>;

template <typename Container>
void pprint(const Container &c, const std::string sep = " ") {
  std::for_each(std::begin(c), std::end(c), [&](const auto &el) { std::cout << el << sep; });
}

matrix<char> get_matrix(const std::vector<std::string> &lines) {
  matrix<char> m;
  for (std::size_t i = 0; i < lines.size(); i++) {
    auto line = lines.at(i);
    m.emplace_back();
    for (std::size_t j = 0; j < line.size(); j++) {
      m[i].emplace_back(line.at(j));
    }
  }
  return m;
}

template <typename T, typename F>
std::vector<T> get_numbers(const std::vector<std::string> &lines, F &f) {
  std::vector<T> numbers;
  std::transform(lines.begin(), lines.end(), std::back_inserter(numbers), [&](const auto &el) { return f(el); });
  return numbers;
}

template <typename T>
std::vector<std::vector<T>> get_subsets(const std::vector<T> &set) {
  auto                        n = pow(2, set.size()) - 1;
  std::vector<std::vector<T>> res;
  for (auto i = 0; i < n; i++) {
    auto           binary_str = std::bitset<128>(i).to_string();
    std::vector<T> subset;
    auto           set_iter = 0;
    for (auto j = binary_str.size() - 1; j >= binary_str.size() - set.size(); j--) {
      char check = binary_str.at(j);
      if (check == '1') {
        subset.push_back(set.at(set_iter));
      }
      set_iter++;
    }

    if (subset.empty()) {
      continue;
    }
    res.emplace_back(subset);
  }

  res.emplace_back(set);
  return res;
}

std::vector<std::string> get_lines(const std::string &input_file) {
  std::ifstream            file(input_file);
  std::string              str;
  std::vector<std::string> lines;
  while (std::getline(file, str)) {
    lines.emplace_back(str);
  }
  return lines;
}

std::vector<std::string> split(const std::string &s, char delimiter) {
  std::vector<std::string> tokens;
  std::string              token;
  std::istringstream       tokenStream(s);
  while (std::getline(tokenStream, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

}  // namespace aoc_util
