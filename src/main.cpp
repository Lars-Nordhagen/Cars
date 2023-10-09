#include "./lexer.cpp"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


int main() {
  
  std::vector<Token> tokens;

  tokens = lexFile("./main.ca");

  return 0;
}
