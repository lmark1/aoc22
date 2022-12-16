#include <queue>
#include <unordered_map>
#include "util.hpp"
#include <cctype>
#include <unordered_set>
#include <string>
#include <ostream>
#include <limits>
#include <set>
using LLI = long long int;

/* Point Class */  //{
struct Point
{
  LLI x;
  LLI y;
  LLI range = -1;

  void rotate_45() {
    Point rot;
    rot.x = this->x - this->y;
    rot.y = this->x + this->y;
    *this = rot;
  }

  void unrotate_45() {
    Point unrot;
    unrot.x = (this->x + this->y) / 2;
    unrot.y = (this->y - this->x) / 2;
    *this   = unrot;
  }

  friend std::ostream& operator<<(std::ostream& o, const Point& p) {
    o << "[" << p.x << ", " << p.y << "]";
    return o;
  }
};
//}

/* Square Class */  //{
struct Square
{
  Point top_left;
  Point bot_right;

  void rotate_45() {
    top_left.rotate_45();
    bot_right.rotate_45();
  }

  void unrotate_45() {
    top_left.unrotate_45();
    bot_right.unrotate_45();
  }

  friend std::ostream& operator<<(std::ostream& o, const Square& s) {
    o << "{" << s.top_left << "} - {" << s.bot_right << "}";
    return o;
  }
};
//}

class Compare {
public:
  bool operator()(Square lhs, Square rhs) {
    if (lhs.top_left.x < rhs.top_left.x) {
      return true;
    }

    if (lhs.top_left.x > rhs.top_left.x) {
      return false;
    }

    if (lhs.top_left.y < rhs.top_left.y) {
      return true;
    }

    return false;
  }
};

class CompareReverse {
public:
  bool operator()(Square lhs, Square rhs) {
    if (lhs.top_left.y < rhs.top_left.y) {
      return true;
    }

    if (lhs.top_left.y > rhs.top_left.y) {
      return false;
    }

    if (lhs.top_left.x < rhs.top_left.x) {
      return true;
    }

    return false;
  }
};
class Solution {
public:
  /* part1() */  //{
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
  //}

  int part2(const std::string& input, int max_coords) {
    std::cout << "Part2 for: " << input << "\n";
    auto lines = aoc_util::get_lines(input);

    parse_input(lines);

    for (auto& sensor : m_sensor) {
      auto edges = get_edges(sensor);
      
      for (const auto& edge : edges) {
        if (edge.x < 0 || edge.x > max_coords || edge.y < 0 || edge.y > max_coords) {
          continue;
        }
        bool seen = false;
        for (auto& sensor : m_sensor) {
          auto d = manhatten_dist(sensor, edge);
          if (d <= sensor.range) {
            seen = true;
            break;
          }
        }

        if (seen){
          continue;
        }

        std::cout << "Solution: " << edge << "\n";
        LLI sol = edge.y * 4000000 + edge.x;
        std::cout << sol << "\n";
        return 0;
      }
    }
    int res = 0;
    return res;
  }

private:
  /* Private Variables */  //{
  std::vector<Point>  m_sensor;
  std::vector<Square> m_squares;
  std::vector<Point>  m_beacon;

  LLI m_min_x = -1;
  LLI m_max_x = -1;
  LLI m_min_y = -1;
  LLI m_max_y = -1;

  LLI m_N = -1;
  LLI m_M = -1;

  aoc_util::matrix<char> m_2d_cave;
  std::vector<char>      m_cave;
  //}

  /* get_edges() */  //{
  std::vector<Point> get_edges(const Point& sensor) {
    std::vector<Point> res;
    auto               d       = sensor.range + 1;
    int                offset  = 0;
    int                doffset = 1;
    for (int i = sensor.x - d; i <= sensor.x + d; i++) {
      if (i == sensor.x) {
        doffset *= -1;
      }

      if (i == sensor.x - d || i == sensor.x + d) {
        res.push_back({i, sensor.y});
      } else {
        res.push_back({i, sensor.y + offset});
        res.push_back({i, sensor.y - offset});
      }

      offset += doffset;
    }
    return res;
  }
  //}

  /* manhattan_dist() */  //{
  LLI manhatten_dist(const Point& lhs, const Point& rhs) {
    return std::abs(lhs.x - rhs.x) + std::abs(lhs.y - rhs.y);
  }
  //}

  /* update_cave(...) */  //{
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
  //}

  /* parse_input(...) */  //{
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

      const int dist        = manhatten_dist(m_sensor.back(), m_beacon.back());
      m_sensor.back().range = dist;

      // Make the square
      Square s;
      s.top_left  = {m_sensor.back().x - dist, m_sensor.back().y};
      s.bot_right = {m_sensor.back().x + dist, m_sensor.back().y};
      s.rotate_45();
      m_squares.push_back(s);

      m_min_x = std::min({m_min_x, m_sensor.back().x - dist, m_beacon.back().x, m_sensor.back().x + dist});
      m_min_y = std::min({m_min_y, m_sensor.back().y - dist, m_beacon.back().y, m_sensor.back().y + dist});

      m_max_x = std::max({m_max_x, m_sensor.back().x - dist, m_beacon.back().x, m_sensor.back().x + dist});
      m_max_y = std::max({m_max_y, m_sensor.back().y - dist, m_beacon.back().y, m_sensor.back().y + dist});
    }

    m_N    = m_max_x - m_min_x + 1;
    m_M    = m_max_y - m_min_y + 1;
    m_cave = std::vector<char>(m_M, '.');
  }
  //}

  /* print() */  //{
  void print() {
    aoc_util::ClearScreen();
    for (const auto& el : m_cave) {
      std::cout << el;
    }
    std::cout << "\n";
    aoc_util::sleep(std::chrono::milliseconds(100));
  }
  //}
};

int main() {
  std::cout << Solution().part1("input/day15_0.txt", 10) << "\n";
  std::cout << Solution().part1("input/day15_1.txt", 2000000) << "\n";

  std::cout << Solution().part2("input/day15_0.txt", 20) << "\n";
  std::cout << Solution().part2("input/day15_1.txt", 4000000) << "\n";
  return 0;
}

