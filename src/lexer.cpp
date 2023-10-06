#include "../h/Token.h"
#include "../src/tokenDetermine.cpp"

#include <fstream>
#include <string>
#include <unordered_set>
#include <iostream>
#include <vector>

std::vector<Token> tokens = {};
std::unordered_set<string> keywords;

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
      sendToken("SYN", line.substr(i, synLen));
      i += synLen - 1;
    }

    // IDs can contain letters, "_"s, "-"s, and digits
    // Must contain at least one non number char
    else if ((idLen = isID(line, i)) != 0) {
      string text = line.substr(i, idLen);
      if (isKeyWord(text, keywords)) {
        sendToken("KEYW", text);
      } else {
        sendToken("ID  ", text);
      }
      i += idLen - 1;
    }

    // Numbers contain only digits and can not be touching an ID specified char
    else if ((numLen = isNum(line, i)) != 0) {
      sendToken("NUM ", line.substr(i, numLen));
      i += numLen - 1;
    }

    //Operations are 1 or 2 chars and activate any time they appear
    else if ((oppLen = isOpp(line, i)) != 0) {
      sendToken("OPP ", line.substr(i, oppLen));
      i += oppLen - 1;
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
  if (file.is_open()) {
    std::cout << "Found file: " << fileName << "\n";
  } else {
    std::cout << "ERROR; File " << fileName << "not found \n";
  }

  std::string line;
  keywords = {"for", "if", "else", "while", "int", "bool", "double", "char"};

  // Lexes each line
  while (std::getline(file, line)) {
    lexLine(line);
  }

  if (tokens.size() == 0) {
    std::cout << "No Tokens read\n\n";
  }

  sendToken("ENDL", "");
  
  std::cout << "TOKENS: \n\n";

  // Prints out the token info
  for (Token token : tokens) {
    std::cout << "[" << token.getType() << ": " << token.getText() << "]\n";
  }
  std::cout << "\n";

  return tokens;
}

