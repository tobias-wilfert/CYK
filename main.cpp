#include <iostream>
#include "ContextFreeGrammar.h"

int main(int argc, char *argv[]) {
  json j;
  std::ifstream ifs(argv[1]);
  ifs >> j;
  CYK::ContextFreeGrammar grammar{j};

  for (int i = 2; i < argc; ++i) {
    std::cout << "Now simulating \"" << argv[i] << "\"" << std::endl;
    grammar.CYK(argv[i]);
    std::cout << "Finished simulating" << std::endl;
  }
  return 0;
}