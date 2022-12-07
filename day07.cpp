#include <queue>
#include <unordered_map>
#include "util.hpp"
#include <cctype>
#include <set>
#include <string>


using DepMap     = std::unordered_map<std::string, std::set<std::string>>;
using SizeMap    = std::unordered_map<std::string, int>;
using ReverseMap = std::unordered_map<std::string, std::string>;

void processCommand(const std::vector<std::string>& lines, int& i, DepMap& dep_map, SizeMap& size_map, ReverseMap& reverse_map, std::string& current_file) {
  auto words = aoc_util::split(lines.at(i), ' ');

  // Handle CD
  if (words[1] == "cd") {
    auto target = words[2];

    // Go up
    if (target == "..") {
      current_file = reverse_map[current_file];

      // Regular directory
    } else {
      dep_map[current_file].insert(target);
      reverse_map[target] = current_file;
      size_map[target]    = -1;
      current_file        = target;
    }

    // Handle LS
  } else if (words[1] == "ls") {

    // Parse all the directories / files
    i++;
    for (; i < lines.size(); i++) {
      auto new_words = aoc_util::split(lines.at(i), ' ');

      // Found a new command - break
      if (new_words[0] == "$") {
        i--;
        break;
      }

      dep_map[current_file].insert(new_words[1]);
      reverse_map[new_words[1]] = current_file;

      // Check if file
      if (new_words[0] != "dir") {
        size_map[new_words[1]] = aoc_util::get_int(new_words[0]);

        // Handle directoryk
      } else {
        size_map[new_words[1]] = -1;
      }
    }
  }
}

std::pair<DepMap, SizeMap> processInput(const std::vector<std::string>& lines) {
  DepMap     dep_map;
  ReverseMap reverse_map;
  SizeMap    size_map;

  std::string current_file = "NULL";
  size_map[current_file]   = 0;

  for (int i = 0; i < lines.size(); i++) {
    processCommand(lines, i, dep_map, size_map, reverse_map, current_file);
  }

  return {dep_map, size_map};
}

int dfs(DepMap& dep_map, SizeMap& size_map, std::string current, int& solution) {
  // Check if its a file
  if (dep_map[current].empty()) {
    std::cout << "File: " << current << " size: " << size_map[current] << "\n";
    return size_map[current];
  }

  // Calculate folder size
  int folder_size = 0;
  for (const auto& child : dep_map[current]) {
    if (size_map[child] > 0) {
      folder_size += size_map[child];
    } else {
      folder_size += dfs(dep_map, size_map, child, solution);
    }
  }

  size_map[current] = folder_size;
  if (folder_size <= 100000) {
    solution += folder_size;
  }
  return folder_size;
}

int part1(const std::string& input) {
  std::cout << "Part1 for: " << input << "\n";
  auto lines               = aoc_util::get_lines(input);
  auto [dep_map, size_map] = processInput(lines);

  int res = 0;
  dfs(dep_map, size_map, "/", res);
  return res;
}

int part2(const std::string& input) {
  std::cout << "Part2 for: " << input << "\n";
  auto lines = aoc_util::get_lines(input);
  int  res   = 0;
  return res;
}

int main() {
  std::cout << part1("input/day07_0.txt") << "\n";
  std::cout << part1("input/day07_1.txt") << "\n";

  std::cout << part2("input/day07_0.txt") << "\n";
  std::cout << part2("input/day07_1.txt") << "\n";
  return 0;
}

