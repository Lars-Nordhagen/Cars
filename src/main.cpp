#include "./lexer.cpp"
#include "./parser.cpp"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


int main() {
  
  std::vector<Token> tokens;

  tokens = lexFile("./main.ca");
  parseTokens(tokens); 

  return 0;
}
