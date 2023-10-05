#include "../h/Token.h"
#include "../src/tokenDetermine.cpp"

#include <fstream>
#include <string>
#include <iostream>
#include <vector>

std::vector<Token> tokens = {};

// Adds a token to the list
void sendToken(std::string type, std::string text) {
  Token token(type, text);
  tokens.push_back(token);
}

void lexLine(string line) {

  // Checks if the line is blank or if it is an end line
  bool blankLine = true;
  for (char c : line) {
    if (c != ' ') {
      blankLine = false;
    }

    // Checks if the line is a function terminator
    if (c == '!') {
      sendToken("FUNE", "");
      return;
    }
  }

  // Stops lexing if it is blank
  if (blankLine) {
    return;
  }

  // Goes through each char in the line
  // May skip over chars if they are part of a multi-char token
  int lineLength = line.length();
  for (int i = 0; i < lineLength; i++) {
    int synLen, numLen, oppLen, idLen;

    // Checks if the char is blank
    if (line[i] == ' ') {

    }

    // Syntax is a single char and activates any time a syntax char shows up
    else if ((synLen = isSyn(line, i)) != 0) {
      std::string tempSynStr = "";
      tempSynStr += line[i];
      sendToken("SYN", tempSynStr);
    }

    // IDs can contain letters, "_"s, "-"s, and digits
    // Must contain at least one non number char
    else if ((idLen = isID(line, i)) != 0) {
      i += idLen - 1;

      sendToken("ID  ", line.substr(i, idLen));
    }

    // Numbers contain only digits and can not be touching an ID specified char
    else if ((numLen = isNum(line, i)) != 0) {
      i += numLen - 1;

      sendToken("NUM ", line.substr(i, numLen));
    }

    //Operations are 1 or 2 chars and activate any time they appear
    else if ((oppLen = isOpp(line, i)) != 0) {

      i += oppLen - 1;

      sendToken("OPP ", line.substr(i, oppLen));
    }

    // Sends an unknown token when no other token types are found
    else {
      sendToken("UNK", std::string(1, line[i]));
    }
  }

  //----End of For Loop----//
  
  //line is over
  sendToken("ENDL", "");
}


std::vector<Token> lexFile(string fileName) {
  std::fstream file;
  file.open(fileName, std::ios::in);

  std::string line;

  // Lexes each line
  while (std::getline(file, line)) {
    lexLine(line);
  }

  std::cout << "TOKENS: \n\n";

  // Prints out the token info
  for (Token token : tokens) {
    std::cout << "[" << token.getType() << ": " << token.getText() << "]\n";
  }
  std::cout << "\n";

  return tokens;
}

