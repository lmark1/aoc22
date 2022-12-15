#include <queue>
#include <unordered_map>
#include "util.hpp"
#include <cctype>
#include <unordered_set>
#include <string>

struct Point
{
  int x;
  int y;
};

class Solution {
public:
  int solution(const std::string& input, bool with_floor = false) {
    std::cout << "Part1 for: " << input << "\n";
    auto lines = aoc_util::get_lines(input);
    init_cave(lines, with_floor);

    int res = 0;
    while (spawn_sand(0, 500)) {
      res++;
    }
    print_cave();
    return res;
  }

private:
  int                    m_M         = 600;
  int                    m_max_depth = -1;
  aoc_util::matrix<char> m_cave;
  void                   init_cave(const std::vector<std::string>& lines, bool with_floor = false) {
    m_cave = aoc_util::matrix<char>(1, std::vector<char>(m_M, '.'));

    for (const auto& line : lines) {
      auto               words = aoc_util::split(line, ' ');
      std::vector<Point> rock_line;
      for (int i = 0; i < words.size(); i += 2) {
        auto num_string = aoc_util::split(words.at(i), ',');
        rock_line.push_back({aoc_util::get_int(num_string.back()), aoc_util::get_int(num_string.front())});
      }
      draw_rocks(rock_line);
    }

    if (with_floor) {
      m_cave.emplace_back(std::vector<char>(m_M, '.'));
      m_cave.emplace_back(std::vector<char>(m_M, '#'));
    }
  }

  // Function to draw rocks from a set of lines
  void draw_rocks(const std::vector<Point>& rock_line) {
    for (int i = 0; i < rock_line.size() - 1; i++) {
      const auto& curr = rock_line.at(i);
      const auto& nxt  = rock_line.at(i + 1);

      int min_x = std::min(curr.x, nxt.x);
      int max_x = std::max(curr.x, nxt.x);

      int min_y = std::min(curr.y, nxt.y);
      int max_y = std::max(curr.y, nxt.y);

      if (m_max_depth < max_x) {
        m_max_depth = max_x;
      }

      while (m_cave.size() - 1 < max_x) {
        m_cave.push_back(std::vector<char>(m_M, '.'));
      }

      for (int x = min_x; x <= max_x; x++) {
        for (int y = min_y; y <= max_y; y++) {
          m_cave.at(x).at(y) = '#';
        }
      }
    }
  };


  /* True - sand fell into place, false - otherwise */
  bool spawn_sand(int x, int y) {
    while (x < m_cave.size() - 1) {

      if (y + 1 >= m_M) {
        int new_M = 2 * m_M;
        for (auto& row : m_cave) {
          row.resize(new_M);
          for (int i = m_M; i < new_M; i++) {
            row.at(i) = '.';
          }
        }
        m_M = new_M;
        draw_rocks({{m_max_depth + 2, 0}, {m_max_depth + 2, m_M - 1}});
      }

      char down       = m_cave.at(x + 1).at(y);
      char down_left  = m_cave.at(x + 1).at(y - 1);
      char down_right = m_cave.at(x + 1).at(y + 1);

      if (m_cave.at(x).at(y) != '.') {
        return false;
      }

      if (down == '.') {
        x++;
        continue;
      }

      if (down_left == '.') {
        x++;
        y--;
        continue;
      }

      if (down_right == '.') {
        x++;
        y++;
        continue;
      }

      m_cave.at(x).at(y) = 'o';
      // print_cave();
      return true;
    }

    return false;
  }

  void print_cave() {
    aoc_util::ClearScreen();
    for (const auto& row : m_cave) {
      for (int i = 200; i < row.size(); i++) {
        std::cout << row.at(i);
      }
      std::cout << "\n";
    }
    aoc_util::sleep(std::chrono::milliseconds(40));
  }
};

int main() {
  std::cout << Solution().solution("input/day14_0.txt") << "\n";
  std::cout << Solution().solution("input/day14_1.txt") << "\n";

  std::cout << Solution().solution("input/day14_0.txt", true) << "\n";
  std::cout << Solution().solution("input/day14_1.txt", true) << "\n";
  return 0;
}

