#include <queue>
#include <unordered_map>
#include "util.hpp"
#include <cctype>
#include <unordered_set>

int getScore(char c) {
  int res;
  if (isupper(c)) {
    // Upper
    res = c - 'A' + 27;
  } else {
    // Lower
    res = c - 'a' + 1;
  }
  return res;
}

int part1(const std::string& input) {
  std::cout << "Part1 for: " << input << "\n";
  auto lines = aoc_util::get_lines(input);

  int res = 0;
  for (const auto& line : lines) {
    int                           N = line.size();
    std::unordered_map<char, int> char_map;
    for (int i = 0; i < N; i++) {
      if (i < N / 2) {
        char_map[line.at(i)]++;
        continue;
      }

      char right_char = line.at(i);
      if (char_map[right_char] > 0) {
        res += getScore(right_char);
        break;
      }
    }
  }
  return res;
}


int part2(const std::string& input) {
  std::cout << "Part2 for: " << input << "\n";
  auto lines = aoc_util::get_lines(input);
  int res = 0;
  for (int i = 0; i < lines.size(); i += 3) {
    auto line1 = lines.at(i);
    auto line2 = lines.at(i + 1);
    auto line3 = lines.at(i + 2);

    std::unordered_set<char> set1, set2, set3;
    auto                     unary = [&](auto& el) { return el; };
    std::transform(line1.begin(), line1.end(), std::inserter(set1, set1.begin()), unary);
    std::transform(line2.begin(), line2.end(), std::inserter(set2, set1.begin()), unary);
    std::transform(line3.begin(), line3.end(), std::inserter(set3, set1.begin()), unary);

    for (const auto& el : set1) {
      if (set2.count(el) > 0 && set3.count(el) > 0) {
        res += getScore(el);
      }
    }
  }
  return res;
}

int main() {
  std::cout << part1("input/day03_0.txt") << "\n";
  std::cout << part1("input/day03_1.txt") << "\n";

  std::cout << part2("input/day03_0.txt") << "\n";
  std::cout << part2("input/day03_1.txt") << "\n";
  return 0;
}

