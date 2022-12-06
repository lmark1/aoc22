#include <queue>
#include <map>
#include "util.hpp"
#include <cctype>
#include <unordered_set>
#include <string>
#include <algorithm>
#include <stack>

int nonRepeatingIdx(const std::string& line, int window_size) {
  std::unordered_map<char, int> index_map;
  std::cout << "Solving for: " << line << "\n";
  int left  = 0;
  int right = 0;

  while (right - left < window_size && right < line.size()) {
    char el = line.at(right);

    // Check if already seen the element
    if (index_map.count(el) > 0) {

      // Forget all elements until the last occurence +1 of the current el
      int new_left = index_map[el] + 1;
      for (int i = left; i < new_left; i++) {
        index_map.erase(line.at(i));
      }

      // Update left pointer
      left = new_left;
    }

    // Update right pointer
    index_map[el] = right;
    right++;
  }

  return right;
}

void solution(const std::string& input, int size) {
  std::cout << "PartX for: " << input << "\n";
  auto lines = aoc_util::get_lines(input);
  for (const auto& line : lines) {
    std::cout << nonRepeatingIdx(line, size) << "\n";
  }
}

int main() {
  solution("input/day06_0.txt", 4);
  solution("input/day06_0.txt", 14);
  return 0;
}

