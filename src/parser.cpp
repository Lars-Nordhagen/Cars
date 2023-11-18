#include "../h/ParseNode.h"
#include "../h/Token.h"
#include "../h/Constants.h"

#include <iostream>

std::vector<Class_PN*> classNodes = {};
std::vector<Token> tokensp;

int current = 0;
string currType = "";
string currText = "";


bool Parse_Class();
ParseNode* Parse_Value();
ParseNode* Parse_Value1();
ParseNode* Parse_Value2();
ParseNode* Parse_Value3();
bool Parse_Body(NodeList*);
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
  if (text != "") {
    yes &= token.getText() == text;
  }

  if (yes) {
    current++;
  }
  if (text != "") {
    yes = yes && token.getText() == text;
  }
  std::cout << token.getType() << " " << token.getText() << "\n";
  if (!yes) {
    std::cout << "    expected: " << type << " " << text << "\n";
  }
  currType = type;
  currText = text;
  return yes;
}

bool Type (string type) {
  return Type(type, "");
}

ParseNode* Parse_DT() {
  int save = current;
  if (Type(types::NUMBER)) {
  } else if (Type(types::QUOTE)) {
  } else if (Type(types::BOOL)) {
  } else if (Type(types::ID)) {
  } else {
    current = save;
    return nullptr;
  }
  
  Dual_PN* newNode = new Dual_PN();
  newNode->setText(tokensp.at(current-1).getText());
  newNode->setType(tokensp.at(current-1).getType());
  return newNode; 
}


ParseNode* Parse_Value4_1() {
  return Parse_DT();
}

ParseNode* Parse_Value4_2() {
  bool yes = Type(types::OPEN);

  if (!yes) { return nullptr; }

  ParseNode* node = Parse_Value();

  yes = Type(types::CLOSE);
  if (!yes) { return nullptr; }

  return node;
}

ParseNode* Parse_Value4() {
  int save = current;

  ParseNode* node = Parse_Value4_1();
  if (node != nullptr) { return node; }

  ParseNode* node2 = Parse_Value4_2();
  if (node2 != nullptr) { return node2; }

  current = save;
  return nullptr;
}

ParseNode* Parse_Value3() {
  int save = current;

  ParseNode* node = Parse_Value4();
  if (node == nullptr) {
    current = save;
    return nullptr;
  }

  bool validOpp = (Type(types::OPP, SYN::TIMES) ||
                   Type(types::OPP, SYN::DIVIDE));
  if (!validOpp) {return node;}

  Dual_PN* newNode = new Dual_PN();
  newNode->setText(currText);
  newNode->setType(currType);

  ParseNode* node2 = Parse_Value3();
  if (node2 == nullptr) {
    current = save;
    return nullptr;
  }
  newNode->setLeft(node);
  newNode->setRight(node2);
  return newNode;
}
  
ParseNode* Parse_Value2() {
  int save = current;

  ParseNode* node = Parse_Value3();
  if (node == nullptr) {
    current = save;
    return nullptr;
  }

  bool validOpp = (Type(types::OPP, SYN::PLUS) ||
                   Type(types::OPP, SYN::MINUS));
  if (!validOpp) { return node; }
  Dual_PN* newNode = new Dual_PN();
  newNode->setText(currText);
  newNode->setType(currType);

  ParseNode* node2 = Parse_Value2();
  if (node2 == nullptr) {
    current = save;
    return nullptr;
  }

  
  newNode->setLeft(node);
  newNode->setRight(node2);
  return newNode;
}


ParseNode* Parse_Value1() {
  int save = current;

  ParseNode* node = Parse_Value2();
  if (node == nullptr) {
    current = save;
    return nullptr;
  }

  bool validOpp = (Type(types::OPP, SYN::EQUAL) ||
                   Type(types::OPP, SYN::NEQUAL) ||
                   Type(types::OPP, SYN::LESS) ||
                   Type(types::OPP, SYN::GREAT) ||
                   Type(types::OPP, SYN::LESSE) ||
                   Type(types::OPP, SYN::GREAE));
  if (!validOpp) {return node;}
  
  Dual_PN* newNode = new Dual_PN();
  newNode->setText(currText);
  newNode->setType(currType);

  ParseNode* node2 = Parse_Value1();
  if (node2 == nullptr) {
    current = save;
    return nullptr;
  }

  newNode->setLeft(node);
  newNode->setRight(node2);
  return newNode;
}



ParseNode* Parse_Value() {
  int save = current;

  ParseNode* node = Parse_Value1();
  if (node == nullptr) {
    current = save;
    return nullptr;
  }
  bool validOpp = (Type(types::OPP, SYN::AND) ||
                   Type(types::OPP, SYN::OR));
  if (!validOpp) {return node;}

  Dual_PN* newNode = new Dual_PN(currType);
  newNode->setText(currText);
  newNode->setType(currType);

  
  ParseNode* node2 = Parse_Value();
  if (node2 == nullptr) {
      current = save;
      return nullptr;
  }

  newNode->setLeft(node);
  newNode->setRight(node2);
  return newNode; 
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
bool Parse_Body_4(NodeList* body){

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
bool Parse_Body_2(NodeList* body) {
  int save = current;

  ParseNode* node = Parse_Value();

  if (node == nullptr) {
    current = save;
    return false;
  }
  body->addNode(node);
  
  

  return true;
}
bool Parse_Body(NodeList* body) {
  int save = current;
  bool yes = (current = save, Parse_Body_1(body)) ||
    (current = save, Parse_Body_2(body)) ||
    (current = save, Parse_Body_3(body));


  return yes;
}





void PrintTree() {
  //std::cout<< "Thing: " << classNodes.front()->getList()->getNode(0)->getText() << "\n";
  for (int i = 0; i < classNodes.size(); i++) {
    ParseNode* classNode = classNodes.at(i);
    std::vector<ParseNode*> depthList = {classNode};

    ParseNode* currentNode = classNode;
    classNode->begin();

    std::cout << classNode->getText() << "\n";

    while (depthList.size() != 0) {
      //std::cout<< "1\n";
      ParseNode* possibleNext = currentNode->next();
      //std::cout<< "2 " << currentNode->getText() << "\n";
      if (possibleNext != nullptr) {
        depthList.push_back(currentNode);
        currentNode = possibleNext;
        currentNode->begin();
        for (int i = 0; i < depthList.size() - 1; i++) {
          std::cout << "--";
        }
        std::cout << currentNode->getType() << " " << currentNode->getText() << "\n";
      } else {
        currentNode = depthList.back();
        depthList.pop_back();
      }
    }
  }
}
