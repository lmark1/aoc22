#include <queue>
#include <unordered_map>
#include "util.hpp"
#include <cctype>
#include <set>
#include <string>
#include <limits>

using DepMap     = std::unordered_map<std::string, std::set<std::string>>;
using SizeMap    = std::unordered_map<std::string, int>;
using ReverseMap = std::unordered_map<std::string, std::string>;

void processCommand(const std::vector<std::string>& lines, int& i, DepMap& dep_map, SizeMap& size_map, ReverseMap& reverse_map, std::string& current_file) {
  auto words = aoc_util::split(lines.at(i), ' ');

  // Handle CD
  if (words[1] == "cd") {
    auto target      = words[2];
    auto full_target = current_file + words[2];

    // Go up
    if (target == "..") {
      current_file = reverse_map[current_file];

      // Regular directory
    } else {
      dep_map[current_file].insert(full_target);
      reverse_map[full_target] = current_file;
      size_map[full_target]    = -1;
      current_file             = full_target;
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

      auto newdir = current_file + new_words[1];
      dep_map[current_file].insert(newdir);
      reverse_map[newdir] = current_file;

      // Check if file
      if (new_words[0] != "dir") {
        size_map[newdir] = aoc_util::get_int(new_words[0]);

        // Handle directories
      } else {
        size_map[newdir] = -1;
      }
    }
  }
}

std::pair<DepMap, SizeMap> processInput(const std::vector<std::string>& lines) {
  DepMap     dep_map;
  ReverseMap reverse_map;
  SizeMap    size_map;

  std::string current_file = "NULL";

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
  dfs(dep_map, size_map, "NULL/", res);
  return res;
}

int part2(const std::string& input) {
  std::cout << "Part2 for: " << input << "\n";
  auto lines               = aoc_util::get_lines(input);
  auto [dep_map, size_map] = processInput(lines);

  static constexpr auto TOTAL_SPACE    = 70000000;
  static constexpr auto REQUIRED_SPACE = 30000000;
  int                   sol            = 0;
  const int             total_filesize = dfs(dep_map, size_map, "NULL/", sol);
  const int             free_space     = TOTAL_SPACE - total_filesize;

  sol = std::numeric_limits<int>::max();
  for (const auto [file, filesize] : size_map) {
    if (dep_map[file].empty()) {
      continue;
    }

    if (free_space + filesize > REQUIRED_SPACE) {
      sol = std::min(sol, filesize);
    }
  }
  return sol;
}

int main() {
  std::cout << part1("input/day07_0.txt") << "\n";
  std::cout << part1("input/day07_1.txt") << "\n";

  std::cout << part2("input/day07_0.txt") << "\n";
  std::cout << part2("input/day07_1.txt") << "\n";
  return 0;
}

