#include "./Earley.hpp"

int main() {
  Grammar_CF grammar("../tests/your_test.txt");
  EarleyParser parser(grammar);

  int32_t word_count;
  std::cin >> word_count;
  for (int32_t i = 0; i < word_count; ++i) {
    std::string word;
    std::cin >> word;
    std::cout << (parser.Parse(word) ? "YES" : "NO") << '\n';
  }

  return 0;
}