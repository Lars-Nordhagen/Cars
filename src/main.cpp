#include "./lexer.cpp"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


int main() {
  
  std::vector<Token> tokens;

  tokens = lexFile("./Test2.txt");

  return 0;
}
