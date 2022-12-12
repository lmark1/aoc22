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

using LLI = long long int;
struct Item
{
  using Ptr = std::shared_ptr<Item>;

  LLI                          initial_value;
  std::unordered_map<LLI, LLI> divisor_map;
};

using Operation       = std::function<void(Item&, bool, LLI)>;
using Check           = std::function<bool(Item&, bool)>;
using Action          = std::function<void(bool)>;
using BinaryOperation = std::function<LLI(LLI, LLI)>;

BinaryOperation                           sum           = [](LLI lhs, LLI rhs) { return lhs + rhs; };
BinaryOperation                           times         = [](LLI lhs, LLI rhs) { return lhs * rhs; };
std::unordered_map<char, BinaryOperation> binary_op_map = {{'*', times}, {'+', sum}};

struct Monkey
{
  int                  index         = -1;
  LLI                  inspect_count = 0;
  std::list<Item::Ptr> items;
  Operation            operation;
  Check                check;
  Action               action;

  friend std::ostream& operator<<(std::ostream& o, Monkey& m) {
    o << "Monkey " << m.index << ": ";
    for (const auto& item : m.items) {
      o << item->initial_value << ", ";
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

      auto new_item           = std::make_shared<Item>();
      new_item->initial_value = aoc_util::get_llong(item);
      new_monkey->items.push_back(new_item);
    }

    // Process operation
    auto operation                     = aoc_util::split(monkey_info.at(2), '=');
    operation                          = aoc_util::split(operation.back(), ' ');
    static constexpr auto old_or_parse = [](const std::string& lhs, LLI value) {
      LLI  res    = 0;
      bool is_new = true;
      if (lhs == "old") {
        is_new = false;
        res    = value;
      } else {
        res = aoc_util::get_llong(lhs);
      }
      return std::make_pair(is_new, res);
    };
    
    
    new_monkey->operation = [operation](Item& item, bool divide_worry, LLI common_divisor) {
      auto [is_newl, lhs] = old_or_parse(operation.at(1), item.initial_value);
      auto [is_newr, rhs] = old_or_parse(operation.at(3), item.initial_value);
      if (divide_worry) {
        LLI tmp = binary_op_map.at(operation.at(2).front())(lhs, rhs);
        tmp /= 3;
        item.initial_value = tmp;
        return;
      }

      // Use the common divisor
      // item.initial_value = binary_op_map.at(operation.at(2).front())(item.initial_value, new_val);
      // item.initial_value = item.initial_value % common_divisor;

      // Option 2: use the divisor map
      for (auto& el : item.divisor_map) {
        auto [is_newl, lhs] = old_or_parse(operation.at(1), el.second);
        auto [is_newr, rhs] = old_or_parse(operation.at(3), el.second);

        // Figure out the "new value"
        LLI new_val = lhs;
        if (is_newr) {
          new_val = rhs;
        }
        el.second = binary_op_map.at(operation.at(2).front())(el.second, new_val);
        el.second %= el.first;
      }
    };

    // Process test
    auto test         = aoc_util::split(monkey_info.at(3), ' ');
    LLI  test_divisor = aoc_util::get_llong(test.back());
    m_divisors.push_back(test_divisor);
    new_monkey->check = [test_divisor](Item& item, bool divide_worry) {
      if (divide_worry) {
        return item.initial_value % test_divisor == 0;
      }

      // Test only the initial value
      // LLI tmp = item.initial_value % test_divisor;
      return item.divisor_map[test_divisor] == 0;
    };

    // Process action
    auto index_true    = aoc_util::get_int(aoc_util::split(monkey_info.at(4), ' ').back());
    auto index_false   = aoc_util::get_int(aoc_util::split(monkey_info.at(5), ' ').back());
    new_monkey->action = [=](bool value) {
      auto item = new_monkey->items.front();
      new_monkey->items.pop_front();
      // std::cout << "Throw " << item->initial_value << " to Monkey ";
      if (value) {
        // std::cout << index_true << "\n";
        m_monkeys.at(index_true)->items.push_back(item);
      } else {
        // std::cout << index_false << "\n";
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

  void initialize_divisor_map() {
    for (auto& monkey : m_monkeys) {
      for (auto& item : monkey->items) {
        for (auto divisor : m_divisors) {
          item->divisor_map[divisor] = item->initial_value % divisor;
        }
      }
    }
    for (auto divisor : m_divisors) {
      m_common_divisor *= divisor;
    }
  }

  LLI solution(const std::string& input, int round_count, bool divide_worry = true) {
    std::cout << "Part1 for: " << input << "\n";
    m_divide_worry = divide_worry;
    auto lines     = aoc_util::get_lines(input);
    parse_monkeys(lines);
    initialize_divisor_map();

    // Process monkeys
    for (int i = 0; i < round_count; i++) {
      for (auto& monkey : m_monkeys) {
        while (!monkey->items.empty()) {
          monkey->inspect_count++;
          monkey->operation(*monkey->items.front(), m_divide_worry, m_common_divisor);
          bool result = monkey->check(*monkey->items.front(), m_divide_worry);
          monkey->action(result);
        }
      }
    }

    // Get solution
    std::priority_queue<LLI> pq;
    for (auto& monkey : m_monkeys) {
      pq.push(monkey->inspect_count);
    }
    LLI res = pq.top();
    pq.pop();
    res *= pq.top();
    return res;
  }

private:
  std::vector<std::shared_ptr<Monkey>> m_monkeys;
  std::vector<LLI>                     m_divisors;
  LLI                                  m_common_divisor = 1;
  bool                                 m_divide_worry   = true;
};

int main() {
  std::cout << Solution().solution("input/day11_0.txt", 20) << "\n";
  std::cout << Solution().solution("input/day11_1.txt", 20) << "\n";

  std::cout << Solution().solution("input/day11_0.txt", 10000, false) << "\n";
  std::cout << Solution().solution("input/day11_1.txt", 10000, false) << "\n";
  return 0;
}

