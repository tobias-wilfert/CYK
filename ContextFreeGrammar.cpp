//============================================================================
// Name        : ContextFreeGrammar.cpp
// Author      : Tobias Wilfert
//============================================================================

#include "ContextFreeGrammar.h"

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

void CYK::ContextFreeGrammar::CYK(const std::string &input) {
  Table  table = generateCYKTable(input.size());
  for(int i=0; i < input.size(); ++i){   // fill in the first row
    table[0][i] = productions.getVariablesThatProduce(
        {std::string{input.at(i)}});
  }
  // Fill in the rest of the table
  for(int i=1; i < table.size(); i++){
    for(int j=0; j < table.size()-i; ++j){ // Looking at (i,j)
     std::set<std::string> varsForCell;
     for(int k=0; k < i; ++k){ // Looking at (k,j) (i-k-1,j+k+1)
       for(const Replacement& rep: getPermutations(
                table.at(k).at(j),table.at(i-k-1).at(j+k+1))){
         std::set<std::string> vars = productions.getVariablesThatProduce(rep);
         varsForCell.insert(vars.begin(),vars.end());
       }
     }
     table.at(i).at(j) = varsForCell;
    }
  }

  createHTMLRepresentation(input, table);
}

CYK::Table CYK::ContextFreeGrammar::generateCYKTable(int size) {
  Table table;
  for(int i = 0; i < size; ++i){
    std::vector<std::set<std::string>> row;
    for(int j = i; j < size; ++j){ row.emplace_back(); }
    table.push_back(row);
  }
  return table;
}

std::vector<CYK::Replacement> CYK::ContextFreeGrammar::getPermutations(
    const std::set<std::string> &set1, const std::set<std::string> &set2) const {
  std::vector<Replacement> result;
  for(auto& p: set1){
    for(auto& q: set2){
      result.push_back({p,q});
    }
  }
  return result;
}

void CYK::ContextFreeGrammar::createHTMLRepresentation(
    const std::string &input, const CYK::Table &table) const {

  //TODO Make the HTML prettier
  std::string htmlDoc = "<html>\n"
                        "<style>\n"
                        "  table, th, td { border: 1px solid black;}\n"
                        "  html *{font-family: Arial, Helvetica, sans-serif;}\n"
                        "</style>\n";

  htmlDoc += ("<h3>CYK table for \"" + input + "\"</h3>\n");
  htmlDoc += "<table>\n";
  for(auto& row: table){
    htmlDoc += "  <tr>\n";
    for(auto& col: row){
      htmlDoc += "    <th>";
      for(auto& con: col){
        htmlDoc += con + ",";
      }
      if(htmlDoc.back() == ','){ htmlDoc.pop_back(); }
      htmlDoc += "</th>\n";
    }
    htmlDoc += "  </tr>\n";
  }
  htmlDoc += "</table>\n"
             "</html>";
  std::ofstream out("CYKTable-" + input + ".html");
  out << htmlDoc;
  out.close();
}
