#include <queue>
#include <unordered_map>
#include "util.hpp"
#include <cctype>
#include <unordered_set>
#include <string>
#include <functional>

class Solution {
public:
  using Funct    = std::function<void(int)>;
  using FunctMap = std::unordered_map<std::string, Funct>;

  // addx function
  Funct addx = [&](int num) {
    static constexpr auto ADDX_CYCLE = 2;
    for (int i = 0; i < ADDX_CYCLE; i++) {
      m_cycle++;
      update();
    }
    m_reg_x += num;
  };

  // noop function
  Funct noop = [&](int /* unused */) {
    m_cycle++;
    update();
  };

  FunctMap func_map = {{"addx", addx}, {"noop", noop}};

  std::pair<int, int> get_row_col(int idx) {
    int row = idx / m_WIDTH;
    int col = idx - row * m_WIDTH;
    return {row, col};
  }

  void update() {
    // Part 1
    if (m_cycle == m_next_cycle && m_next_cycle <= 220) {
      m_signal_strength += m_cycle * m_reg_x;
      m_next_cycle += 40;
    }

    // Part 2
    int cycle_hor = (m_cycle - 1) % 40;
    int cycle_vert = (m_cycle - 1) / 40;
    if (cycle_hor == m_reg_x || cycle_hor == m_reg_x + 1 || cycle_hor == m_reg_x - 1) {
      m_screen.at(cycle_vert).at(cycle_hor) = '#';
    }
  }

  void process_command(const std::string& line) {
    const auto words = aoc_util::split(line, ' ');
    const auto cmd   = words.front();
    int        val   = -1;
    if (words.size() == 2) {
      val = aoc_util::get_int(words.back());
    }
    func_map.at(cmd)(val);
  }

  int solution(const std::string& input) {
    std::cout << "PartX for: " << input << "\n";
    auto lines = aoc_util::get_lines(input);
    for (const auto& line : lines) {
      if (line.empty()) {
        break;
      }
      process_command(line);
    }

    for (const auto& row : m_screen) {
      for (const char el : row) {
        std::cout << el;
      }
      std::cout << "\n";
    }
    return m_signal_strength;
  }

private:
  int m_reg_x = 1;
  int m_cycle = 0;

  // Part 1
  int m_signal_strength = 0;
  int m_next_cycle      = 20;

  // Part 2
  const int              m_WIDTH  = 40;
  const int              m_HEIGHT = 6;
  aoc_util::matrix<char> m_screen = std::vector<std::vector<char>>(m_HEIGHT, std::vector<char>(m_WIDTH, '.'));
};

int main() {
  std::cout << Solution().solution("input/day10_1.txt") << "\n";
  std::cout << Solution().solution("input/day10_2.txt") << "\n";
  return 0;
}

