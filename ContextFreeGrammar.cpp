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
    const std::set<std::string> &set1, const std::set<std::string> &set2) {
  std::vector<Replacement> result;
  for(auto& p: set1){
    for(auto& q: set2){
      result.push_back({p,q});
    }
  }
  return result;
}

void CYK::ContextFreeGrammar::createHTMLRepresentation(
    const std::string &input, const CYK::Table &table) {
  std::string htmlDoc = "<html lang=\"en\" >\n"
                        "<style>\n"
                        "  table, td { border: 1px solid black;\n"
                        "              padding: 5px;}\n"
                        "  html *{font-family: Arial, Helvetica, sans-serif;}\n"
                        "</style>\n"
                        "<table>\n";
  htmlDoc += ("<caption>CYK table for \"" + input + "\"</caption>\n");
  // The Main table
  for(int i = table.size()-1; i > -1; --i){
    htmlDoc += "  <tr>\n";
    for(auto& col: table.at(i)){
      htmlDoc += "    <td>";
      for(auto& con: col){
        htmlDoc += con + ",";
      }
      if(htmlDoc.back() == ','){ htmlDoc.pop_back(); }
      htmlDoc += "</td>\n";
    }
    htmlDoc += "  </tr>\n";
  }
  // Add the input to the bottom of the table
  htmlDoc += "  <tr>\n";
  for(auto& s: input){
    htmlDoc += ("    <th>" + std::string{s} + "</th>\n");
  }
  htmlDoc += "  </tr>\n"
             "</table>\n"
             "</html>";
  std::ofstream out("CYKTable-" + input + ".html");
  out << htmlDoc;
  out.close();
}
