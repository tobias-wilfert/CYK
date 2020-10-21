#include <fstream>
#include <iostream>

#include "ContextFreeGrammar.h"

int main(int argc, char *argv[]) {
  for (int i = 1; i < argc; ++i) {
    json j;
    std::ifstream ifs(argv[i]);
    ifs >> j;
    CYK::ContextFreeGrammar grammar{j};
    std::cout << " Done ";
  }
  return 0;
}