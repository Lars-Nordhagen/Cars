#include "../h/Token.h"
#include "../src/tokenDetermine.cpp"

#include <fstream>
#include <string>
#include <unordered_set>
#include <iostream>
#include <vector>

std::vector<Token> tokens = {};
std::unordered_set<string> keywords;
std::unordered_set<string> classes;
std::unordered_set<string> typesUS;

// Adds a token to the list
void sendToken(std::string type, std::string text) {
  Token token(type, text);
  tokens.push_back(token);
}

void lexLine(string line) {
  line += " "; // Avoid index errors if at end of line
  // Checks if the line is blank or if it is an end line
  bool blankLine = true;
  for (char c : line) {
    if (c != ' ') {
      blankLine = false;
    }

    // Checks if the line is a function terminator
    if (c == '!') {
      sendToken(types::FUNE, "");
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
    int numLen, oppLen, idLen, quoLen;
    string synTyp;
    char currChar = line[i];

    // Checks if the char is blank
    if (currChar == ' ') {

    }
    else if (currChar == '/' && line[i+1] == '/') {
      return;
    }
    else if ((quoLen = isQuo(line, i)) != 0) {
      sendToken(types::QUOTE, line.substr(i, quoLen));
      i += quoLen - 1;
    }

    // Syntax is a single char and activates any time a syntax char shows up
    else if ((synTyp = isSyn(line[i], line[i+1])) != "") {
      if (synTyp == types::EQUALMOD) {
        sendToken(synTyp, line.substr(i, 2));
      } else {
        sendToken(synTyp, line.substr(i, 1));
      }
    }

    // IDs can contain letters, "_"s, "-"s, and digits
    // Must contain at least one non number char
    else if ((idLen = isID(line, i)) != 0) {
      string text = line.substr(i, idLen);
      if (isKeyWord(text, keywords)) {
        sendToken(types::KEYW, text);
      } else if (isClass(text, classes)) {
        sendToken(types::CLASS, text);
      } else if (isType(text, typesUS)) {
        sendToken(types::TYPE, text);
      } else {
        sendToken(types::ID, text);
      }
      i += idLen - 1;
    }

    // Numbers contain only digits and can not be touching an ID specified char
    else if ((numLen = isNum(line, i)) != 0) {
      sendToken(types::NUMBER, line.substr(i, numLen));
      i += numLen - 1;
    }

    //Operations are 1 or 2 chars and activate any time they appear
    else if ((oppLen = isOpp(line, i)) != 0) {
      sendToken(types::OPP, line.substr(i, oppLen));
      i += oppLen - 1;
    }

    // Sends an unknown token when no other token types are found
    else {
      sendToken(types::UNKNOWN, std::string(1, line[i]));
    }
  }

  //----End of For Loop----//
  
  //line is over
  sendToken(types::ENDL, "");
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
  keywords = {KW::FOR, KW::IF, KW::ELSE, KW::WHILE, KW::ELSE, KW::CLASS, KW::INCLUDE, KW::NEW, KW::CONST, KW::PACKAGE};
  classes = {"Main", "int", "bool", "double", "char", "Class1", "Class2", "Class3"};
  typesUS = {SYN::INT, SYN::BOOL, SYN::DOUBLE, SYN::CHAR};

  // Lexes each line
  while (std::getline(file, line)) {
    lexLine(line);
  }

  if (tokens.size() == 0) {
    std::cout << "No Tokens read\n\n";
  }

  sendToken(types::ENDL, "");
  
  std::cout << "TOKENS: \n\n";

  // Prints out the token info
  for (Token token : tokens) {
    std::cout << "[" << token.getType() << ": " << token.getText() << "]\n";
  }
  std::cout << "\n";

  return tokens;
}

