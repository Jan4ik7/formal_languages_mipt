#include "Grammar.hpp"

Grammar_CF::State::State() {}

Grammar_CF::State::State(const char new_symbol, bool is_terminal)
    : symbol_(new_symbol), is_terminal_(is_terminal) {}

bool operator==(const Grammar_CF::State &lhs, const Grammar_CF::State &rhs) {
  return lhs.symbol_ == rhs.symbol_ && lhs.is_terminal_ == rhs.is_terminal_;
}

bool operator==(const Grammar_CF::State &lhs, const char &rhs) {
  return lhs.symbol_ == rhs;
}

bool Grammar_CF::State::IsTerminal() const { return is_terminal_; }

std::istream &operator>>(std::istream &stream, Grammar_CF::State &obj) {
  stream >> obj.symbol_ >> obj.is_terminal_;
  return stream;
}

///////////////////////////////////////////////////

Grammar_CF::Rule::Rule() {}

Grammar_CF::Rule::Rule(const State &start_state) : start_state_(start_state) {}

Grammar_CF::Rule::Rule(const State &start_state,
                       const std::vector<Grammar_CF::State> &lead_states)
    : start_state_(start_state), lead_states_(lead_states) {}

bool operator==(const Grammar_CF::Rule &lhs, const Grammar_CF::Rule &rhs) {
  return lhs.start_state_ == rhs.start_state_ &&
         lhs.lead_states_ == rhs.lead_states_;
}

Grammar_CF::State &Grammar_CF::Rule::operator[](size_t index) {
  return lead_states_[index];
}

///////////////////////////////////////////////////

Grammar_CF::Grammar_CF() {}

Grammar_CF::Grammar_CF(const std::string &file_name) {
  std::ifstream input(file_name);
  if (!input) {
    throw std::logic_error("Failed to open file: " + file_name);
  }
  size_t term_states_size = 0;
  size_t non_term_states_size = 0;
  size_t rules_size = 0;
  input >> non_term_states_size;
  input >> term_states_size;
  input >> rules_size;

  std::string nonterminal;
  input >> nonterminal;

  E_.insert({'_', State('_')});
  E_['_'].is_terminal_ = true; // '_' - epsilon

  for (size_t i = 0; i < non_term_states_size; ++i) {
    N_.insert({nonterminal[i], State(nonterminal[i], false)});
  }

  std::string terminal;
  input >> terminal;

  for (size_t i = 0; i < term_states_size; ++i) {
    E_.insert({terminal[i], State(terminal[i], true)});
  }
  std::string cur_rule;
  bool met_an_arrow = false;
  for (size_t i = 0; i < rules_size; ++i) {
    input >> cur_rule;
    Rule rule;
    size_t idx = cur_rule.find(rule.kRuleDelimiter);
    if (idx == std::string::npos) {
      std::cout << cur_rule << '\n';
      std::cout << i << '\n';
      throw std::runtime_error("rule doesn't contain two parts");
    }
    char start_symbol = cur_rule[0];
    if (N_.find(start_symbol) == N_.end()) {
      continue;
    }
    rule.start_state_ = N_[start_symbol];
    std::string str_rule_rhs =
        cur_rule.substr(idx + rule.kRuleDelimiter.size());
    if (str_rule_rhs.empty()) {
      throw std::runtime_error("one of states is empty");
    }
    for (auto j : str_rule_rhs) {
      if (N_.find(j) != N_.end()) {
        rule.lead_states_.push_back(N_[j]);
      } else if (E_.find(j) != E_.end()) {
        rule.lead_states_.push_back(E_[j]);
      } else {
        throw std::logic_error(
            "Invalid Format of rule: no such states in grammatic in file: " +
            file_name);
      }
    }
    R_.push_back(rule);
  }

  char cur_start_symbol;
  input >> cur_start_symbol;

  if (N_.find(cur_start_symbol) == N_.end()) {
    throw std::logic_error(
        "There no starting Nonterminal S in grammatic in file: " + file_name);
  }
  SetStartSymbol(cur_start_symbol);
}
