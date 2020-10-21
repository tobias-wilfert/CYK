//============================================================================
// Name        : ContextFreeGrammar.cpp
// Author      : Tobias Wilfert
//============================================================================

#include "ContextFreeGrammar.h"

#include <utility>

void CYK::Productions::addProduction(const std::string &variable,
                                     const CYK::Replacement &replacement) {
  productions[variable].insert(replacement);
  reverseProductions[replacement].insert(variable);
}
std::set<CYK::Replacement> CYK::Productions::getReplacementsFor(
    const std::string &variable) {
  return productions[variable];
}
std::set<std::string> CYK::Productions::getVariablesThatProduce(
    const CYK::Replacement &replacement) {
  return reverseProductions[replacement];
}

CYK::ContextFreeGrammar::ContextFreeGrammar(const json &j) {
  startSymbol = j["Start"];
  variables = j["Variables"].get<std::unordered_set<std::string>>();
  terminals = j["Terminals"].get<std::unordered_set<std::string>>();
  for (auto &element : j["Productions"]) {
    productions.addProduction(element["head"],
                              element["body"]);
  }
}

CYK::ContextFreeGrammar::ContextFreeGrammar(
    std::string start_symbol,
    CYK::Productions productions,
    std::unordered_set<std::string> variables,
    std::unordered_set<std::string> terminals) : startSymbol(std::move(start_symbol)),
                                                 productions(std::move(productions)),
                                                 variables(std::move(variables)),
                                                 terminals(std::move(terminals)) {}
