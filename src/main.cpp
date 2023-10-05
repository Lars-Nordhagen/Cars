#include "./lexer.cpp"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


int main() {
  
  std::vector<Token> tokens;

  tokens = lexFile("../Test.txt");


  std::cout << "MAIN: " << tokens[2].getText() << "\n";

  return 0;
}
