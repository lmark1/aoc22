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
  using Ptr = std::shared_ptr<Node>;

  std::string            name         = "NULL";
  int                    single_value = -1;
  std::vector<Node::Ptr> nested;

  static Node::Ptr make_node(int val) {
    auto single          = std::make_shared<Node>();
    single->single_value = val;
    return single;
  }

  friend std::ostream& operator<<(std::ostream& o, const Node& root) {
    o << "Node: " << root.single_value << "\n";
    if (!root.nested.empty()) {
      o << root.nested.size() << " Children: \n";
    }
    for (const auto& n : root.nested) {
      std::cout << *n << "\n";
    }
    return o;
  };

  static int cmp(const Node& lhs, const Node& rhs) {

    // Both single values
    if (lhs.single_value >= 0 && rhs.single_value >= 0) {
      return lhs.single_value - rhs.single_value;
    }

    // Check if node needs nesting - if single
    static constexpr auto get_nested = [](const Node& node) {
      auto temp = std::make_shared<Node>(node);
      if (temp->nested.empty()) {
        temp = Node::make_node(-1);
        if (node.single_value >= 0) {
          temp->nested.emplace_back(make_node(node.single_value));
        }
      }
      return temp;
    };

    auto temp_lhs = get_nested(lhs);
    auto temp_rhs = get_nested(rhs);

    // Compare both nests
    int idx = 0;
    while (idx < temp_lhs->nested.size() && idx < temp_rhs->nested.size()) {
      auto res = cmp(*temp_lhs->nested.at(idx), *temp_rhs->nested.at(idx));

      // If not same, keep comparing
      if (res != 0) {
        return res;
      }
      idx++;
    }

    // If no resolution found, check the nested size
    return temp_lhs->nested.size() - temp_rhs->nested.size();
  }
};

class Solution {
public:
  Node::Ptr parse(const std::string& line, int& start) {
    auto       root        = std::make_shared<Node>();
    const auto add_to_nest = [&](std::string& number) {
      if (number.empty()) {
        return;
      }
      root->nested.emplace_back(Node::make_node(aoc_util::get_int(number)));
      number.clear();
    };

    std::string number;
    for (; start < line.size() - 1; start++) {
      char el = line.at(start);

      // Build a number
      if (std::isdigit(el)) {
        number += el;
        continue;
      }

      // Terminate number
      if (el == ',') {
        add_to_nest(number);
        continue;
      }

      // Terminate array
      if (el == ']') {
        add_to_nest(number);
        return root;
      }

      // Start new array
      if (el == '[') {
        start++;
        auto new_array = parse(line, start);
        root->nested.push_back(new_array);
        continue;
      }
    }

    add_to_nest(number);
    return root;
  }

  bool lesser_than(const std::vector<std::string>& pair) {
    const auto& lhs = pair.front();
    const auto& rhs = pair.back();

    int  start    = 0;
    auto lhs_node = parse(lhs, start);

    start         = 0;
    auto rhs_node = parse(rhs, start);

    return Node::cmp(*lhs_node, *rhs_node) < 0;
  }

  int part1(const std::string& input) {
    std::cout << "PartX for: " << input << "\n";
    auto lines = aoc_util::get_lines(input);

    std::vector<std::string> pairs;
    int                      index = 1;
    int                      res   = 0;
    for (const auto& line : lines) {
      if (line.empty()) {
        continue;
      }

      pairs.push_back(line);
      if (pairs.size() < 2) {
        continue;
      }

      if (lesser_than(pairs)) {
        res += index;
      }
      pairs.clear();
      index++;
    }
    return res;
  }

  int part2(const std::string& input) {
    std::cout << "PartX for: " << input << "\n";
    auto lines = aoc_util::get_lines(input);

    std::vector<std::string> unsorted;
    for (const auto& line : lines) {
      if (line.empty()) {
        continue;
      }
      unsorted.push_back(line);
    }

    static constexpr auto ADD_1 = "[[2]]";
    static constexpr auto ADD_2 = "[[6]]";

    unsorted.push_back(ADD_1);
    unsorted.push_back(ADD_2);

    std::vector<Node> unsorted_nodes;
    int               start = 0;
    for (const auto& el : unsorted) {
      start = 0;
      unsorted_nodes.push_back(*parse(el, start));
      unsorted_nodes.back().name = el;
    }

    std::sort(unsorted_nodes.begin(), unsorted_nodes.end(), [&](const auto& lhs, const auto& rhs) { return Node::cmp(lhs, rhs) < 0; });
    int res = 1;
    for (int i = 0; i < unsorted_nodes.size(); i++) {
      const auto& el = unsorted_nodes.at(i);
      if (el.name == ADD_1 || el.name == ADD_2) {
        res *= i + 1;
      }
    }
    return res;
  }

private:
};

int main() {
  std::cout << Solution().part1("input/day13_0.txt") << "\n";
  std::cout << Solution().part1("input/day13_1.txt") << "\n";

  std::cout << Solution().part2("input/day13_0.txt") << "\n";
  std::cout << Solution().part2("input/day13_1.txt") << "\n";
  return 0;
}

