#include <queue>
#include <unordered_map>
#include "util.hpp"

// A/X - Rock
// B/Y - Paper
// C/Z - Scissors

std::unordered_map<std::string, int> IDX_MAP = {{"A", 1}, {"B", 0}, {"C", 2}};
std::unordered_map<std::string, int> REWARD  = {{"A", 1}, {"B", 2}, {"C", 3}, {"X", 1}, {"Y", 2}, {"Z", 3}};
std::vector<std::string>             ENEMY   = {"B", "A", "C"};
std::vector<std::string>             FRIEND  = {"Y", "X", "Z"};

int part1(const std::string& input) {
  std::cout << "Part1 for: " << input << "\n";
  auto lines = aoc_util::get_lines(input);
  int  res   = 0;
  for (const auto& line : lines) {
    auto split       = aoc_util::split(line, ' ');
    auto  enemy_idx   = IDX_MAP[split[0]];
    auto  winning_idx = (enemy_idx - 1 + ENEMY.size()) % ENEMY.size();

    res += REWARD[split[1]];
    if (split[1] == FRIEND[enemy_idx]) {
      // Draw
      res += 3;
    } else if (split[1] == FRIEND[winning_idx]) {
      // Win
      res += 6;
    }
  }
  return res;
}

int part2(const std::string& input) {
  std::cout << "Part2 for: " << input << "\n";
  auto lines = aoc_util::get_lines(input);
  int  res   = 0;
  for (const auto& line : lines) {
    auto split       = aoc_util::split(line, ' ');
    auto  enemy_idx   = IDX_MAP[split[0]];
    auto  winning_idx = (enemy_idx - 1 + ENEMY.size()) % ENEMY.size();
    auto  losing_idx  = (enemy_idx + 1) % ENEMY.size();

    if (split[1] == "Y") {
      // Draw
      res += REWARD[split[0]] + 3;
    } else if (split[1] == "Z") {
      // Win
      res += REWARD[ENEMY[winning_idx]] + 6;
    } else {
      // Lose
      res += REWARD[ENEMY[losing_idx]];
    }
  }
  return res;
}

int main() {
  std::cout << part1("input/day02_0.txt") << "\n";
  std::cout << part1("input/day02_1.txt") << "\n";

  std::cout << part2("input/day02_0.txt") << "\n";
  std::cout << part2("input/day02_1.txt") << "\n";
  return 0;
}

