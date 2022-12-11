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

using LLI             = long long int;
using Operation       = std::function<void(LLI&, bool)>;
using Check           = std::function<bool(LLI)>;
using Action          = std::function<void(bool)>;
using BinaryOperation = std::function<LLI(LLI, LLI)>;

BinaryOperation                           sum           = [](LLI lhs, LLI rhs) { return lhs + rhs; };
BinaryOperation                           times         = [](LLI lhs, LLI rhs) { return lhs * rhs; };
std::unordered_map<char, BinaryOperation> binary_op_map = {{'*', times}, {'+', sum}};

struct Monkey
{
  int            index         = -1;
  LLI            inspect_count = 0;
  std::list<LLI> items;
  Operation      operation;
  Check          check;
  Action         action;

  friend std::ostream& operator<<(std::ostream& o, Monkey& m) {
    o << "Monkey " << m.index << ": ";
    for (const auto& item : m.items) {
      o << item << ", ";
    }
    return o;
  }
};

class Solution {
public:
  /* Parse a single monkey */
  void parse_monkey(const std::vector<std::string>& monkey_info) {
    auto new_monkey   = std::make_shared<Monkey>();
    new_monkey->index = m_monkeys.size();

    // Process monkey items
    const auto items = aoc_util::split(monkey_info.at(1), ' ');
    for (int i = 4; i < items.size(); i++) {
      auto item = items.at(i);
      if (i != items.size() - 1) {
        item = item.substr(0, item.size() - 1);
      }

      new_monkey->items.push_back(aoc_util::get_int(item));
    }

    // Process operation
    auto operation                     = aoc_util::split(monkey_info.at(2), '=');
    operation                          = aoc_util::split(operation.back(), ' ');
    static constexpr auto old_or_parse = [](const std::string& lhs, LLI value) {
      LLI res = 0;
      if (lhs == "old") {
        res = value;
      } else {
        res = aoc_util::get_llong(lhs);
      }
      return res;
    };
    new_monkey->operation = [=](auto& value, bool divide_worry) {
      LLI lhs = old_or_parse(operation.at(1), value);
      LLI rhs = old_or_parse(operation.at(3), value);
      LLI tmp = binary_op_map.at(operation.at(2).front())(lhs, rhs);
      if (divide_worry) {
        tmp /= 3;
      }
      value = tmp;
    };

    // Process test
    auto test         = aoc_util::split(monkey_info.at(3), ' ');
    new_monkey->check = [=](LLI value) {
      bool tmp = value % aoc_util::get_llong(test.back()) == 0;
      return tmp;
    };

    // Process action
    auto index_true    = aoc_util::get_int(aoc_util::split(monkey_info.at(4), ' ').back());
    auto index_false   = aoc_util::get_int(aoc_util::split(monkey_info.at(5), ' ').back());
    new_monkey->action = [=](bool value) {
      auto item = new_monkey->items.front();
      new_monkey->items.pop_front();
      if (value) {
        m_monkeys.at(index_true)->items.push_back(item);
      } else {
        m_monkeys.at(index_false)->items.push_back(item);
      }
    };

    std::cout << *new_monkey << "\n";
    m_monkeys.push_back(new_monkey);
  }

  /* Parse all monkeys */
  void parse_monkeys(const std::vector<std::string>& lines) {
    std::vector<std::string> monkey_info;
    for (const auto& line : lines) {
      if (line.empty()) {
        parse_monkey(monkey_info);
        monkey_info.clear();
        continue;
      }

      monkey_info.push_back(line);
    }
    parse_monkey(monkey_info);
  }

  LLI solution(const std::string& input, int round_count, bool divide_worry = true) {
    std::cout << "Part1 for: " << input << "\n";
    m_divide_worry = divide_worry;
    auto lines     = aoc_util::get_lines(input);
    parse_monkeys(lines);

    // Process monkeys
    for (int i = 0; i < round_count; i++) {
      for (auto& monkey : m_monkeys) {
        while (!monkey->items.empty()) {
          monkey->inspect_count++;
          monkey->operation(monkey->items.front(), m_divide_worry);
          bool result = monkey->check(monkey->items.front());
          monkey->action(result);
        }
      }

      if (i == 0 || i == 19 || i == 999) {
        std::cout << "Round :" << i << "\n";
        for (auto& monkey : m_monkeys) {
          std::cout << "Moneky " << monkey->index << " - " << monkey->inspect_count << "\n";
        }
        std::cout << "\n";
      }
    }

    // Get solution
    std::priority_queue<LLI> pq;
    for (auto& monkey : m_monkeys) {
      std::cout << monkey->inspect_count << "\n";
      pq.push(monkey->inspect_count);
    }
    LLI res = pq.top();
    pq.pop();
    res *= pq.top();
    return res;
  }

private:
  std::vector<std::shared_ptr<Monkey>> m_monkeys;
  bool                                 m_divide_worry = true;
};

int main() {
  // std::cout << Solution().solution("input/day11_0.txt", 20) << "\n";
  // std::cout << Solution().solution("input/day11_1.txt", 20) << "\n";

  std::cout << Solution().solution("input/day11_0.txt", 10000, false) << "\n";
  // std::cout << Solution().solution("input/day11_1.txt", 10000, false) << "\n";
  return 0;
}

