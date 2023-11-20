#include "../h/Constants.h"

#include <string>
#include <unordered_set>
#include <iostream>

using std::string;

// Checks if a char is a char used in IDs
static bool isIDChar(char testChar) {
  // True if char is from alphabet
  if (isalpha(testChar)) {
    return true;
  }

  const char otherChars[] = {'_'};

  // True if char is one of predfined chars
  for (char c : otherChars) {
    if (testChar == c) {
      return true;
    }
  }

  return false;
}

static int isID(string str, int startingIndex) {

  // Bool to make sure the token contains at least 1 id char and not just numbers
  bool contIDChar = false;

  for (int i = startingIndex; i < str.length(); i++) {
    // Sets IDChar Var if it contains one
    // Returns the length of token if char is neither id char or digit
    if (isIDChar(str[i])) {
      contIDChar = true;
    } else if (!isdigit(str[i])) {
      if (contIDChar) {
        return i - startingIndex;
      } else {
        return 0;
      }
    }
  }

  // Returns the length if it contains an ID Char and it reached the end of the line
  if (contIDChar) {
    return str.length() - startingIndex;
  }

  return 0;
}


// Checks if the string from the index to tht next blank is a number
// Returns 0 if it is not a number
static int isNum(string str, int startingIndex) {
  
  // Stops if the preceding char is a letter
  if (startingIndex != 0) {
    if (isIDChar(str[startingIndex - 1])) {
      return 0;
    }
  }

  // Checks if it is not directly connected to an ID Char
  // Returns when the number is over
  for (int i = startingIndex; i < str.length(); i++) {
    if (isIDChar(str[i])) {
      return 0;
    }

    if (!isdigit(str[i])) {
      return i - startingIndex;
    }
  }

  // Returns if the number lasted to the end of the line
  return str.length() - startingIndex;
}


static int isOpp(string str, int startingIndex) {
  char testChar = str[startingIndex];
  char nextChar = 0x00;

  // Sets the next char to a value if the current char isn't the last
  if (str.length() > startingIndex + 1) {
    nextChar = str[startingIndex + 1];
  }

  bool shortMatch = false;

  for (int i = 0; i < std::size(SYN::OPP_ARR); i++) {
    if (SYN::OPP_ARR[i][0] == testChar) {
      std::cout << SYN::OPP_ARR[i]<< SYN::OPP_ARR[i].length() << nextChar << "\n";

      if (SYN::OPP_ARR[i].length() > 1 && SYN::OPP_ARR[i][1] == nextChar) {
        return 2;
      }
      shortMatch = true;
    }
  }
  return shortMatch;
}

static bool isKeyWord(string str, std::unordered_set<string> keywordSet) {
  for (int i = 0; i < std::size(KW::KW_ARR); i++) {
    if (str == KW::KW_ARR[i]) {
      return true;
    }
  }

  return false;
}
static bool isClass(string str, std::unordered_set<string> classSet) {
  if (classSet.find(str) != classSet.end()) {
    return true;
  }

  return false;
}
static bool isType(string str, std::unordered_set<string> typeSet) {
  if (typeSet.find(str) != typeSet.end()) {
    return true;
  }

  return false;
}

static string isSyn(char cha, char nextCha) {
  // Checks if the char is a syntax char
  if (cha == SYN::OPEN[0]) {
    return types::OPEN;
  }
  if (cha == SYN::CLOSE[0]) {
    return types::CLOSE;
  }
  if (cha == SYN::DOT[0]) {
    return types::DOT;
  }
  if (cha == SYN::COMMA[0]) {
    return types::COMMA;
  }
  if (cha == SYN::SET[0] && nextCha != SYN::SET[0]) {
    return types::EQUAL;
  }

  for (int i = 0; i < std::size(SYN::SET_ARR); i++) {
    if (SYN::SET_ARR[i][0] == cha && SYN::SET_ARR[i][1] == nextCha) {
      return types::EQUALMOD;
    }
  }

  return "";
}

static int isQuo(string line, int startingIndex) {
  if (line[startingIndex] != '"') {
    return 0;
  }
  
  int ct = 0;
  for (int i = startingIndex + 1; i < line.length(); i++) {
    if (line[i] != '"') {
      ct++;
    } else {
      break;
    }
  }
  return ct + 2;
}
