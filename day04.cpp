#include <queue>
#include <unordered_map>
#include "util.hpp"
#include <cctype>
#include <unordered_set>
#include <string>

struct Range
{
  int  left;
  int  right;
  void print() {
    std::cout << "[" << left << ", " << right << "]\n";
  }
};

bool isOverlap(const Range& lhs, const Range& rhs) {
  int maxLeft  = std::max(lhs.left, rhs.left);
  int minRight = std::min(lhs.right, rhs.right);
  return maxLeft <= minRight;
}

Range getRange(const std::string& el) {
  auto elf_string = aoc_util::split(el, '-');
  return {std::stoi(elf_string.at(0)), std::stoi(elf_string.at(1))};
}

int part1(const std::string& input) {
  std::cout << "Part1 for: " << input << "\n";
  auto lines = aoc_util::get_lines(input);
  int  res   = 0;
  for (auto& el : lines) {
    auto pairs      = aoc_util::split(el, ',');
    auto elf1_range = getRange(pairs.at(0));
    auto elf2_range = getRange(pairs.at(1));
    if (!isOverlap(elf1_range, elf2_range)) {
      continue;
    }

    if ((elf1_range.left >= elf2_range.left && elf1_range.right <= elf2_range.right) ||
        (elf2_range.left >= elf1_range.left && elf2_range.right <= elf1_range.right)) {
      res++;
    }
  }

  return res;
}

int part2(const std::string& input) {
  std::cout << "Part2 for: " << input << "\n";
  auto lines = aoc_util::get_lines(input);
  int  res   = 0;
  for (auto& el : lines) {
    auto pairs      = aoc_util::split(el, ',');
    auto elf1_range = getRange(pairs.at(0));
    auto elf2_range = getRange(pairs.at(1));
    if (isOverlap(elf1_range, elf2_range)) {
      res++;
    }
  }

  return res;
}

int main() {
  std::cout << part1("input/day04_0.txt") << "\n";
  std::cout << part1("input/day04_1.txt") << "\n";

  std::cout << part2("input/day04_0.txt") << "\n";
  std::cout << part2("input/day04_1.txt") << "\n";
  return 0;
}

