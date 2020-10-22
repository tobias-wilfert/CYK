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


/// Namespace used for the CYK algorithm
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
   * Get all the variables that have a certain replacement
   * @param replacement The replacement that the variable needs to have
   * @return A set of all the variables that have replacement replacement
   */
  std::set<std::string> getVariablesThatProduce(const Replacement& replacement);
};

/// A class representing the CFG with the addition of the CYK algorithm
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

  /**
   * Generates a table for the CYK table
   * @param size The size the table should have
   * @return A Table with the specified size
   */
  static Table generateCYKTable(int size);

  /**
   * Get all the permutation of two sets
   * @param set1 The first set that should be used for the permutation
   * @param set2  The second set that should be used for the permutation
   * @return A vector of vectors each containing two strings one from the first
   *    set followed by one from the second
   */
  static std::vector<Replacement> getPermutations(
      const std::set<std::string>& set1,
      const std::set<std::string>& set2) ;

  /// Creates an HTML representation of the CYK table
  static void createHTMLRepresentation(
      const std::string& input, const Table& table) ;

 public:
  /**
   * Initializes the CFG from a json representation of the CFG
   * @param j a json representation of the CFG
   */
  explicit ContextFreeGrammar(const json &j);

  /**
   * Checks whether input is in th language of the CFG
   * @param input The input string that is being checked
   */
  void CYK(const std::string& input);
};

} // namespace CYK

#endif//CYK__CONTEXTFREEGRAMMAR_H_
