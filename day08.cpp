#include <queue>
#include <unordered_map>
#include "util.hpp"
#include <cctype>
#include <unordered_set>
#include <string>

class Solution {
public:
  void paseLines(const std::vector<std::string>& lines) {
    for (const auto& line : lines) {
      m_forest.emplace_back();
      for (char el : line) {
        m_forest.back().push_back(el - '0');
      }
    }
  }

  void initSeen() {
    m_seen = std::vector<std::vector<bool>>(N, std::vector<bool>(M, false));

    // Initialize top and bottom
    for (int j = 0; j < M; j++) {
      m_seen[0][j]     = true;
      m_seen[N - 1][j] = true;
    }

    for (int i = 0; i < N; i++) {
      m_seen[i][0]     = true;
      m_seen[i][M - 1] = true;
    }
  }

  int count_visible() {
    int visible = 0;
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < M; j++) {
        if (m_seen[i][j])
          visible++;
      }
    }
    return visible;
  }

  void visible_trees(int x, int y, int dx, int dy, int max_so_far) {

    while (!(x < 0 || x >= N || y < 0 || y >= M)) {
      auto current = m_forest.at(x).at(y);

      if (max_so_far < current) {
        if (!m_seen[x][y]) {
          m_visible++;
        }
        m_seen.at(x).at(y) = true;
      }

      max_so_far = std::max(max_so_far, current);
      x += dx;
      y += dy;
    }
  }

  int count_trees(int x, int y, int dx, int dy, int max_so_far) {

    int tree_count = 0;
    while (!(x < 0 || x >= N || y < 0 || y >= M)) {
      auto current = m_forest.at(x).at(y);

      if (current >= max_so_far) {
        tree_count++;
        break;
      }

      max_so_far = std::max(max_so_far, current);
      x += dx;
      y += dy;
      tree_count++;
    }

    return tree_count;
  }

  int part1(const std::string& input) {
    std::cout << "Part1 for: " << input << "\n";
    auto lines = aoc_util::get_lines(input);
    N          = lines.size();
    M          = lines.front().size();
    paseLines(lines);
    initSeen();

    // Count horizontal
    for (int i = 1; i < N - 1; i++) {
      visible_trees(i, 1, 0, 1, m_forest[i][0]);
      visible_trees(i, M - 2, 0, -1, m_forest[i][M - 1]);
    }

    // Count vertical
    for (int j = 1; j < M - 1; j++) {
      visible_trees(1, j, 1, 0, m_forest[0][j]);
      visible_trees(N - 2, j, -1, 0, m_forest[N - 1][j]);
    }

    return m_visible + N * 2 + (M - 2) * 2;
  }


  int part2(const std::string& input) {
    std::cout << "Part2 for: " << input << "\n";
    auto lines = aoc_util::get_lines(input);
    N          = lines.size();
    M          = lines.front().size();
    paseLines(lines);
    initSeen();

    int res = 0;
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < M; j++) {
        int  current = m_forest[i][j];
        int temp = count_trees(i - 1, j, -1, 0, current) * count_trees(i, j - 1, 0, -1, current) *
                   count_trees(i + 1, j, 1, 0, current) * count_trees(i, j + 1, 0, 1, current);
        res = std::max(temp, res);
      }
    }
    return res;
  }

private:
  aoc_util::matrix<int>  m_forest;
  aoc_util::matrix<bool> m_seen;
  int                    N;
  int                    M;
  int                    m_visible = 0;
};

int main() {
  std::cout << Solution().part1("input/day08_0.txt") << "\n";
  std::cout << Solution().part1("input/day08_1.txt") << "\n";

  std::cout << Solution().part2("input/day08_0.txt") << "\n";
  std::cout << Solution().part2("input/day08_1.txt") << "\n";
  return 0;
}

