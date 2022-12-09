#include <queue>
#include <unordered_map>
#include "util.hpp"
#include <cctype>
#include <unordered_set>
#include <string>
#include <functional>
#include <set>

struct Index
{
  int         x = -1;
  int         y = -1;
  std::string name;

  bool operator<(const Index& rhs) const {
    if (x < rhs.x)
      return true;
    if (x > rhs.x)
      return false;
    if (y < rhs.y)
      return true;
    return false;
  }

  friend std::ostream& operator<<(std::ostream& o, const Index& el) {
    o << el.name << " [" << el.x << ", " << el.y << "]";
    return o;
  }
};

class Solution {
public:
  void process_step(const char command, Index& idx) {
    switch (command) {
      case 'U': {
        idx.x += 1;
        break;
      }
      case 'D': {
        idx.x -= 1;
        break;
      }
      case 'L': {
        idx.y -= 1;
        break;
      }
      case 'R': {
        idx.y += 1;
        break;
      }
      default: {
        std::cout << "ERROR!!!\n\n";
        break;
      }
    }
  }

  void update_rest(Index& head, Index& tail) {
    int x_distance = std::abs(head.x - tail.x);
    int y_distance = std::abs(head.y - tail.y);

    // Close enough
    if (x_distance <= 1 && y_distance <= 1) {
      return;
    }

    int dx = aoc_util::signum(head.x - tail.x);
    int dy = aoc_util::signum(head.y - tail.y);

    tail.x += dx;
    tail.y += dy;
  }

  void process_command(const std::string& line) {
    const auto        words   = aoc_util::split(line, ' ');
    const std::string command = words.front();
    const int         number  = aoc_util::get_int(words.back());

    for (int i = 0; i < number; i++) {
      process_step(command.at(0), m_snake.front());
      for (int i = 0; i < m_snake.size() - 1; i++) {
        update_rest(m_snake.at(i), m_snake.at(i+1));
      }
      m_tail_visited.insert(m_snake.back());
    }
  }

  int solution(const std::string& input, int snake_size) {
    std::cout << "Part1 for: " << input << "\n";
    auto lines = aoc_util::get_lines(input);

    // Make a snake
    m_snake.push_back(Index{0, 0, "H"});
    for (int i = 1; i <= snake_size; i++) {
      m_snake.push_back(Index{0, 0, std::to_string(i)});
    }
    m_tail_visited.insert(m_snake.back());

    for (const auto& line : lines) {
      process_command(line);
    }

    return m_tail_visited.size();
  }


private:
  std::set<Index>    m_tail_visited;
  std::vector<Index> m_snake;
};

int main() {
  std::cout << Solution().solution("input/day09_0.txt", 1) << "\n";
  std::cout << Solution().solution("input/day09_1.txt", 1) << "\n";

  std::cout << Solution().solution("input/day09_0.txt", 9) << "\n";
  std::cout << Solution().solution("input/day09_2.txt", 9) << "\n";
  std::cout << Solution().solution("input/day09_1.txt", 9) << "\n";
  return 0;
}

