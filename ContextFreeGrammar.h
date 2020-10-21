//============================================================================
// Name        : ContextFreeGrammar.h
// Author      : Tobias Wilfert
//============================================================================

#ifndef CYK__CONTEXTFREEGRAMMAR_H_
#define CYK__CONTEXTFREEGRAMMAR_H_

#include <set>
#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <iostream>
#include <unordered_set>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

namespace CYK{

/// Representation of a single replacement a variable can have
using Replacement = std::vector<std::string>;

/// The datatype of the Table the CYK is using
using Table = std::vector<std::vector<std::set<std::string>>>;

/// A struct that represents the productions of a CFG
struct Productions {
 private:

  /**
   * Represents the productions of a CFG
   * Maps replacement to the variables
   * The keys of the map are the variables in the CFG
   * The values are sets of replacements that the variable can have
   */
  std::map<std::string, std::set<Replacement>> productions;

  /// Inverse of productions, mapping variables to replacements
  std::map<Replacement, std::set<std::string>> reverseProductions;

 public:
  /**
   * Add a production to productions
   * @param variable The variable of the production
   * @param replacement The replacement of the production
   */
  void addProduction(const std::string &variable, const Replacement &replacement);

  /**
   * Get the replacements for the variable
   * @param variable The variable the replacements are for
   * @return A set of replacements
   */
  std::set<Replacement> getReplacementsFor(const std::string &variable);

  /**
   * Get all the variables that have a certain replacement
   * @param replacement The replacement that the variable needs to have
   * @return A set of all the variables that have replacement replacement
   */
  std::set<std::string> getVariablesThatProduce(const Replacement& replacement);
};

class ContextFreeGrammar {
 private:
  /// The start symbol
  std::string startSymbol;

  /// The productions
  Productions productions;

  /// The finite set of variables
  std::unordered_set<std::string> variables;

  /// The finite set of terminals
  std::unordered_set<std::string> terminals;

  // TODO: Add documentation
  static Table generateCYKTable(int size);

  std::vector<Replacement> getPermutations(
      const std::set<std::string>& set1, const std::set<std::string>& set2) const;

  void createHTMLRepresentation(const std::string& input, const Table& table) const;

 public:
  /**
   * Initializes the CFG from a json representation of the CFG
   * @param j a json representation of the CFG
   */
  explicit ContextFreeGrammar(const json &j);

  /**
   * Normal constructor of the CFG
   * @param start_symbol The start symbol
   * @param productions The productions
   * @param variables The Variables
   * @param terminals The Terminals
   */
  ContextFreeGrammar(std::string start_symbol,
                     Productions productions,
                     std::unordered_set<std::string> variables,
                     std::unordered_set<std::string> terminals);

  // TODO: Add documentation
  void CYK(const std::string& input);
};

} // namespace CYK

#endif//CYK__CONTEXTFREEGRAMMAR_H_
