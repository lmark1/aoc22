#include <queue>
#include "util.hpp"

int part1(const std::string& input) {
  std::cout << "Part1 for: " << input << "\n";
  auto lines   = aoc_util::get_lines(input);
  int  current = 0;
  int  max_sol = 0;
  for (auto& line : lines) {
    if (line.empty()) {
      max_sol = std::max(max_sol, current);
      current = 0;
      continue;
    }

    current += aoc_util::get_int(line);
  }
  max_sol = std::max(max_sol, current);
  return max_sol;
}

int part2(const std::string& input) {
  std::cout << "Part2 for: " << input << "\n";
  auto                     lines   = aoc_util::get_lines(input);
  int                      current = 0;
  std::priority_queue<int> pq;
  for (auto& line : lines) {
    if (line.empty()) {
      pq.push(current);
      current = 0;
      continue;
    }

    current += aoc_util::get_int(line);
  }
  pq.push(current);

  int                   sol = 0;
  static constexpr auto NUM = 3;
  for (int i = 0; i < NUM; i++) {
    sol += pq.top();
    pq.pop();
  }
  return sol;
}

int main() {
  std::cout << part1("input/day01_0.txt") << "\n";
  std::cout << part1("input/day01_1.txt") << "\n";

  std::cout << part2("input/day01_0.txt") << "\n";
  std::cout << part2("input/day01_1.txt") << "\n";
  return 0;
}

