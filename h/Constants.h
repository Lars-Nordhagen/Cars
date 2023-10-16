#include <string>

using std::string;

#ifndef CONST_H
#define CONST_H

namespace types {
  const string ID = "ID  ";
  const string QUOTE = "QUO ";
  const string NUMBER = "NUM ";
  const string BOOL = "BOOL";
  const string OPP = "OPP ";
  const string KEYW = "KEYW";
  const string CLASS = "CLSS";
  const string FUNE = "FUNE";
  const string ENDL = "ENDL";

  const string OPEN = "OPEN";
  const string CLOSE = "CLOS";
  const string DOT = "DOT ";
  const string COMMA = "COMM";
  const string EQUAL = "EQUA";

  const string UNKNOWN = "UNKW";
}

namespace KW {
  const string FOR = "for";
  const string IF = "if";
  const string ELSE = "else";
  const string WHILE = "while";

  const string CLASS = "class";
  const string FUNC = "func";

  const string INCLUDE = "include";
  const string NEW = "new";
  const string CONST = "const";
  const string PACKAGE = "package";

  const string KW_ARR[] = {FOR, IF, ELSE, WHILE, CLASS, FUNC, INCLUDE, NEW, CONST, PACKAGE};
}

namespace SYN {
  const string DOT = ".";
  const string COMMA = ",";
  const string OPEN = "(";
  const string CLOSE = ")";
  const string SET = "=";
  
  const string PLUS = "+";
  const string MINUS = "-";
  const string TIMES = "*";
  const string DIVIDE = "/";

  const string AND = "&&";
  const string OR = "||";
  const string LESS = "<";
  const string GREAT = ">";
  const string EQUAL = "==";
  const string NEQUAL = "!=";
  const string LESSE = "<=";
  const string GREAE = ">=";

  const string PLUSE = "+=";
  const string MINUE = "-=";
  const string TIMEE = "*=";
  const string DIVIE = "/=";

  const string INCR = "++";
  const string DECR = "--";

  const string OPP_ARR[] = {PLUS, MINUS, TIMES, DIVIDE, AND, OR, LESS, GREAT, EQUAL, LESSE, GREAE, PLUSE, MINUE, TIMEE, DIVIE, INCR, DECR};
}

#endif // CONST_H
