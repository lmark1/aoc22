#include <queue>
#include <map>
#include "util.hpp"
#include <cctype>
#include <unordered_set>
#include <string>
#include <algorithm>
#include <stack>

auto getEmptyIdx(const std::vector<std::string>& lines) {
  return (std::find_if(lines.begin(), lines.end(), [&](const auto& el) { return el.empty(); }) - lines.begin());
}

using ContainerMap = std::map<int, std::stack<char>>;
void extractLine(const std::string& line, ContainerMap& cm) {
  int start     = 1;
  int cargo_idx = 1;
  while (start < line.size()) {
    char el = line.at(start);
    if (el != ' ') {
      cm[cargo_idx].push(el);
    }
    start += 4;
    cargo_idx++;
  }
}

void moveCargo(int amount, int from, int to, ContainerMap& cm) {
  while (amount > 0) {
    char el = cm.at(from).top();
    cm.at(from).pop();
    cm.at(to).push(el);
    amount--;
  }
}

void moveCargo9001(int amount, int from, int to, ContainerMap& cm) {
  std::stack<char> help;
  while (amount > 0) {
    amount--;
    help.push(cm.at(from).top());
    cm.at(from).pop();
  }

  while (!help.empty()) {
    cm.at(to).push(help.top());
    help.pop();
  }
}

template<typename F>
std::string part1(const std::string& input, F move_cargo) {
  std::cout << "PartX for: " << input << "\n";
  auto lines = aoc_util::get_lines(input);

  int          empty_idx = getEmptyIdx(lines);
  int          tmp_idx   = empty_idx - 2;
  ContainerMap cm;
  while (tmp_idx >= 0) {
    extractLine(lines.at(tmp_idx), cm);
    tmp_idx--;
  }

  for (int i = empty_idx + 1; i < lines.size(); i++) {
    auto words  = aoc_util::split(lines.at(i), ' ');
    int  amount = aoc_util::get_int(words.at(1));
    int  from   = aoc_util::get_int(words.at(3));
    int  to     = aoc_util::get_int(words.at(5));
    move_cargo(amount, from, to, cm);
  }

  std::string res;
  for (auto& [key, value] : cm) {
    res += value.top();
  }
  return res;
}

int main() {
  std::cout << part1("input/day05_0.txt", moveCargo) << "\n";
  std::cout << part1("input/day05_1.txt", moveCargo) << "\n";

  std::cout << part1("input/day05_0.txt", moveCargo9001) << "\n";
  std::cout << part1("input/day05_1.txt", moveCargo9001) << "\n";
  return 0;
}

