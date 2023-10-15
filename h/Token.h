#ifndef TOKEN_H
#define TOKEN_H

#include <string>
using std::string;

class Token {
public:
  Token() {}

  Token(string p_type, string p_text) {
    type = p_type;
    text = p_text;
  }

  string getType() const {
    return type;
  }

  string getText() const {
    return text;
  }

private:
  string type;
  string text;
};

#endif // TOKEN_H
