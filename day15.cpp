#include <queue>
#include <unordered_map>
#include "util.hpp"
#include <cctype>
#include <unordered_set>
#include <string>
#include <ostream>
#include <limits>

using LLI = long long int;

struct Point
{
  LLI x;
  LLI y;

  friend std::ostream& operator<<(std::ostream& o, const Point& p) {
    o << "[" << p.x << ", " << p.y << "]";
    return o;
  }
};

class Solution {
public:
  int part1(const std::string& input, int row) {
    std::cout << "Part1 for: " << input << "\n";
    auto lines = aoc_util::get_lines(input);
    parse_input(lines);

    for (int i = 0; i < m_beacon.size(); i++) {
      const auto& beacon = m_beacon.at(i);
      const auto& sensor = m_sensor.at(i);
      update_cave(sensor, beacon, row);
    }

    for (int i = 0; i < m_beacon.size(); i++) {
      const auto& beacon = m_beacon.at(i);
      const auto& sensor = m_sensor.at(i);

      if (beacon.x == row) {
        m_cave.at(beacon.y - m_min_y) = 'B';
      }

      if (sensor.x == row) {
        m_cave.at(sensor.y - m_min_y) = 'S';
      }
    }

    int res = 0;
    for (auto el : m_cave) {
      if (el == '#') {
        res++;
      }
    }
    return res;
  }

  int part2(const std::string& input, int max_coords) {
    std::cout << "Part2 for: " << input << "\n";
    auto lines = aoc_util::get_lines(input);
    int  res   = 0;
    return res;
  }

private:
  std::vector<Point> m_sensor;
  std::vector<Point> m_beacon;

  LLI m_min_x = -1;
  LLI m_max_x = -1;
  LLI m_min_y = -1;
  LLI m_max_y = -1;

  LLI m_N = -1;
  LLI m_M = -1;
  // aoc_util::matrix<char> m_cave;
  std::vector<char> m_cave;

  LLI manhatten_dist(const Point& lhs, const Point& rhs) {
    return std::abs(lhs.x - rhs.x) + std::abs(lhs.y - rhs.y);
  }

  void update_cave(const Point& sensor, const Point& beacon, int target_row) {

    const auto dist = manhatten_dist(sensor, beacon);
    if (target_row > sensor.x + dist || target_row < sensor.x - dist) {
      return;
    }

    LLI offset = dist - std::abs(target_row - sensor.x);
    for (LLI j = sensor.y - offset; j <= sensor.y + offset; j++) {
      m_cave.at(j - m_min_y) = '#';
    }
  }

  void parse_input(const std::vector<std::string>& lines) {

    static constexpr auto get_index = [](std::string& line, bool skip_last = false) {
      if (!skip_last) {
        return aoc_util::get_int(line);
      }
      return aoc_util::get_int(line.substr(0, line.size() - 1));
    };

    m_min_x = std::numeric_limits<int>::max();
    m_min_y = std::numeric_limits<int>::max();

    m_max_x = std::numeric_limits<int>::min();
    m_max_y = std::numeric_limits<int>::min();

    for (const auto& line : lines) {
      auto part = aoc_util::split(line, ':');

      auto sensor_words = aoc_util::split(part.front(), ' ');
      auto sensor_x     = aoc_util::split(sensor_words.at(2), '=').back();
      auto sensor_y     = aoc_util::split(sensor_words.at(3), '=').back();
      m_sensor.push_back({get_index(sensor_y), get_index(sensor_x, true)});

      auto beacon_words = aoc_util::split(part.back(), ' ');
      auto beacon_x     = aoc_util::split(beacon_words.at(5), '=').back();
      auto beacon_y     = aoc_util::split(beacon_words.at(6), '=').back();
      m_beacon.push_back({get_index(beacon_y), get_index(beacon_x, true)});

      const int dist = manhatten_dist(m_sensor.back(), m_beacon.back());

      m_min_x = std::min({m_min_x, m_sensor.back().x - dist, m_beacon.back().x, m_sensor.back().x + dist});
      m_min_y = std::min({m_min_y, m_sensor.back().y - dist, m_beacon.back().y, m_sensor.back().y + dist});

      m_max_x = std::max({m_max_x, m_sensor.back().x - dist, m_beacon.back().x, m_sensor.back().x + dist});
      m_max_y = std::max({m_max_y, m_sensor.back().y - dist, m_beacon.back().y, m_sensor.back().y + dist});
    }

    m_N = m_max_x - m_min_x + 1;
    m_M = m_max_y - m_min_y + 1;
    m_cave = std::vector<char>(m_M, '.');
  }

  void print() {
    aoc_util::ClearScreen();
    for (const auto& el : m_cave) {
      std::cout << el;
    }
    std::cout << "\n";
    aoc_util::sleep(std::chrono::milliseconds(100));
  }
};

int main() {
  std::cout << Solution().part1("input/day15_0.txt", 10) << "\n";
  std::cout << Solution().part1("input/day15_1.txt", 2000000) << "\n";

  // std::cout << Solution().part2("input/day15_0.txt") << "\n";
  // std::cout << Solution().part2("input/day15_1.txt") << "\n";
  return 0;
}

