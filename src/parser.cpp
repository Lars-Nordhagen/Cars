#include "../h/ParseNode.h"
#include "../h/Token.h"
#include "../h/Constants.h"

#include <iostream>

std::vector<Class_PN*> classNodes = {};
std::vector<Token> tokensp;

int current = 0;


bool Parse_Class();
bool Parse_Value(ParseNode*);
bool Parse_Value1();
bool Parse_Value2();
bool Parse_Value3();
bool Parse_Body(NodeList* body);
void PrintTree();



std::vector<Class_PN*> parseTokens (std::vector<Token> p_tokens) {
  tokensp = p_tokens;
  int tokenspLength = tokensp.size();
  std::cout << "PARSING\n";
  
  while (current < tokenspLength) {
    Parse_Class();
  }
  std::cout << "DONE PARSING\n";

  PrintTree();

  return classNodes;
}

bool Type(string type, string text) {
  Token token = tokensp.at(current);
  bool yes = token.getType() == type;

  if (yes) {
    current++;
  }
  if (text != "") {
    yes = yes && token.getText() == text;
  }
  std::cout << token.getType() << " " << token.getText() << " " << yes << "\n";
  if (!yes) {
    std::cout << "    expected: " << type << " " << text << "\n";
  }
  return yes;
}

bool Type (string type) {
  return Type(type, "");
}

bool Parse_DT(ParseNode* node) {
  int save = current;

  if (Type(types::NUMBER)) {
    return true;
  } else if (Type(types::QUOTE)) {
    return true;
  } else if (Type(types::BOOL)) {
    return true;
  } else if (Type(types::ID)) {
    return true;
  }
  
  current = save;
  return false;
}



bool Parse_Value4_1() {
  return Parse_DT(nullptr);
}

bool Parse_Value4_2() {
  bool yes = (Type(types::OPEN) &&
              Parse_Value(nullptr) &&
              Type(types::CLOSE));

  return yes;
}

bool Parse_Value4(){
  int save = current;

  bool yes = ((current = save, Parse_Value4_1()) ||
              (current = save, Parse_Value4_2()));


  return yes;
}

bool Parse_Value3() {
  int save = current;

  bool yes = Parse_Value4();
  if (!yes) {
    current = save;
    return false;
  }
  yes &= (Type(types::OPP, SYN::TIMES) ||
          Type(types::OPP, SYN::DIVIDE));
  if (!yes) {return true;}
  yes &= Parse_Value3();
  if (!yes) {current = save;}
  return yes; 
}

bool Parse_Value2() {
  int save = current;

  bool yes = Parse_Value3();
  if (!yes) {
    current = save;
    return false;
  }
  yes &= (Type(types::OPP, SYN::PLUS) ||
          Type(types::OPP, SYN::MINUS));
  if (!yes) {return true;}
  yes &= Parse_Value2();
  if (!yes) {current = save;}
  return yes;
}



bool Parse_Value1() {
  int save = current;

  bool yes = Parse_Value2();
  if (!yes) {
    current = save;
    return false;
  }
  yes &= (Type(types::OPP, SYN::EQUAL) ||
          Type(types::OPP, SYN::NEQUAL) ||
          Type(types::OPP, SYN::LESS) ||
          Type(types::OPP, SYN::GREAT) ||
          Type(types::OPP, SYN::LESSE) ||
          Type(types::OPP, SYN::GREAE));
  if (!yes) {return true;}
  yes &= Parse_Value1();
  if (!yes) {current = save;}
  return yes;
}



bool Parse_Value(ParseNode* parent) {
  int save = current;

  bool yes = Parse_Value1();
  if (!yes) {
    current = save;
    return false;
  }
  yes &= (Type(types::OPP, SYN::AND) ||
          Type(types::OPP, SYN::OR));
  if (!yes) {return true;}
  yes &= Parse_Value(nullptr);
  if (!yes) {current = save;}
  return yes; 
}



bool Parse_Class_1(NodeList* body) {
  return Type(types::KEYW, KW::CLASS) &&
    Type(types::CLASS) &&
    Type(types::ENDL) &&
    Parse_Body(body);
}
bool Parse_Class() {
  int save = current;

  NodeList* body = new NodeList();
  
  bool yes = Parse_Class_1(body);

  if (!yes) {
    current = save + 1;
  } else {
    Class_PN* cn = new Class_PN();
    cn->setType(types::KEYW);
    cn->setText(KW::CLASS);

    cn->setBody(body);
    
    ParseNode* classNode = new ParseNode();
    classNode->setType(types::CLASS);
    classNode->setText(tokensp.at(save + 1).getText());

    cn->setClassNode(classNode);

    classNodes.push_back(cn);
  }
  return yes;
}





bool Parse_Body_1(NodeList* body) {

  bool yes = Type(types::ID) &&
    Type(types::EQUAL) &&
    Type(types::NUMBER) &&
    Type(types::ENDL) &&
    Parse_Body(body);

  if (yes) {
    Dual_PN* node = new Dual_PN();
    node->setType(types::EQUAL);
    node->setText("=");

    ParseNode* lNode = new ParseNode(types::ID, "", "");
    ParseNode* rNode = new ParseNode(types::NUMBER, "", "");
    node->setLeft(lNode);
    node->setRight(rNode);

    body->addNode(node, 0);
  }
  return yes;
}
bool Parse_Body_2(NodeList* body){

  bool yes = Type(types::ID) &&
    Type(types::EQUAL) &&
    Type(types::QUOTE) &&
    Type(types::ENDL) &&
    Parse_Body(body);

  if (yes) {
    Dual_PN* node = new Dual_PN();
    node->setType(types::EQUAL);
    node->setText("=");

    ParseNode* lNode = new ParseNode(types::ID, "");
    ParseNode* rNode = new ParseNode(types::QUOTE, "");
    node->setLeft(lNode);
    node->setRight(rNode);

    body->addNode(node, 0);
  }
  return yes;
}
bool Parse_Body_3(NodeList* body) {
  return Type(types::FUNE);
}
bool Parse_Body(NodeList* body) {
  int save = current;
  bool yes = (current = save, Parse_Body_1(body)) ||
    (current = save, Parse_Body_2(body)) ||
    (current = save, Parse_Body_3(body));


  return yes;
}





void PrintTree() {
  for (int i = 0; i < classNodes.size(); i++) {
    ParseNode* classNode = classNodes.at(i);
    std::vector<ParseNode*> depthList = {classNode};

    ParseNode* currentNode = classNode;
    classNode->begin();

    std::cout << classNode->getText() << "\n";

    while (depthList.size() != 0) {
      ParseNode* possibleNext = currentNode->next();
      if (possibleNext != nullptr) {
        depthList.push_back(currentNode);
        currentNode = possibleNext;
        currentNode->begin();
        for (int i = 0; i < depthList.size() - 1; i++) {
          std::cout << "  ";
        }
        std::cout << currentNode->getType() << "\n";
      } else {
        currentNode = depthList.back();
        depthList.pop_back();
      }
    }
  }
}
