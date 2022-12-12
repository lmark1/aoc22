#include <queue>
#include <unordered_map>
#include "util.hpp"
#include <cctype>
#include <unordered_set>
#include <string>
#include <queue>
#include <functional>
#include <ostream>
#include <list>
#include <limits>

struct Node
{
  int i;
  int j;
  int dist;
};

class CompareNode {
public:
  bool operator()(Node lhs, Node rhs) {
    return lhs.dist > rhs.dist;
  }
};

class Solution {
public:
  std::pair<std::pair<int, int>, std::pair<int, int>> init(const std::vector<std::string>& lines) {
    m_N   = lines.size();
    m_M   = lines.front().size();
    int i = 0;
    int j = 0;

    std::pair<int, int> start;
    std::pair<int, int> end;
    for (const auto& line : lines) {
      std::cout << line << "\n";
      m_heightmap.emplace_back();
      j = 0;
      for (const char el : line) {
        m_heightmap.back().push_back(el);
        if (el == 'S') {
          start = std::make_pair(i, j);
        }
        if (el == 'E') {
          end = std::make_pair(i, j);
        }
        j++;
      }
      i++;
    }
    return std::make_pair(start, end);
  }

  int dikstra(int start_i, int start_j, int end_i, int end_j) {
    aoc_util::matrix<int> distance_map(m_N, std::vector<int>(m_M, std::numeric_limits<int>::max()));
    distance_map[start_i][start_j] = 0;

    const auto START = m_heightmap[start_i][start_j];
    const auto END   = m_heightmap[end_i][end_j];

    std::priority_queue<Node, std::vector<Node>, CompareNode> pq;
    pq.push({start_i, start_j, 0});

    while (!pq.empty()) {
      auto node = pq.top();
      pq.pop();

      // Initialize current
      char curr_char      = m_heightmap[node.i][node.j];
      int  current_height = curr_char - 'a';
      if (curr_char == START) {
        current_height = 0;
      }

      for (const auto& n_idx : neighbour_idx) {
        int next_i = node.i + n_idx.first;
        int next_j = node.j + n_idx.second;

        // Check range
        if (next_i < 0 || next_i >= m_N || next_j < 0 || next_j >= m_M) {
          continue;
        }

        // Initialize end
        char next_char   = m_heightmap[next_i][next_j];
        int  next_height = next_char - 'a';
        if (next_char == END) {
          next_height = 'z' - 'a' + 1;
        }

        // Check height - not during start or end
        if (next_height > current_height + 1) {
          continue;
        }

        // Update distance
        int new_distance = distance_map[node.i][node.j] + 1;
        if (distance_map[next_i][next_j] > new_distance) {
          if (next_char != END) {
            // Update node normally
            distance_map[next_i][next_j] = new_distance;
            pq.push({next_i, next_j, distance_map[next_i][next_j]});
          } else if (curr_char == 'z') {
            // Only update E if we reached z
            distance_map[next_i][next_j] = new_distance;
          }
        }
      }
    }

    return distance_map.at(end_i).at(end_j);
  }

  int part1(const std::string& input) {
    std::cout << "PartX for: " << input << "\n";
    auto lines = aoc_util::get_lines(input);

    // Initialize
    auto [start, end] = init(lines);
    return dikstra(start.first, start.second, end.first, end.second);
  }

  int part2(const std::string& input) {
    std::cout << "PartX for: " << input << "\n";
    auto lines = aoc_util::get_lines(input);

    // Initialize
    auto [_start, end] = init(lines);
    std::vector<std::pair<int, int>> starting_points;
    for (int i = 0; i < m_heightmap.size(); i++) {
      for (int j = 0; j < m_heightmap.front().size(); j++) {
        auto& el = m_heightmap[i][j];
        if (el == 'S' || el == 'a') {
          el = 'a';
          starting_points.emplace_back(i, j);
        }
      }
    }

    int min_path = std::numeric_limits<int>::max();
    for (const auto& start : starting_points) {
      m_heightmap[start.first][start.second] = 'S';
      min_path                               = std::min(dikstra(start.first, start.second, end.first, end.second), min_path);
      m_heightmap[start.first][start.second] = 'a';
    }
    return min_path;
  }

private:
  int m_N;
  int m_M;

  aoc_util::matrix<char> m_heightmap;

  std::vector<std::pair<int, int>> neighbour_idx = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
};

int main() {
  std::cout << Solution().part1("input/day12_0.txt") << "\n";
  std::cout << Solution().part1("input/day12_1.txt") << "\n";

  std::cout << Solution().part2("input/day12_0.txt") << "\n";
  std::cout << Solution().part2("input/day12_1.txt") << "\n";
  return 0;
}

