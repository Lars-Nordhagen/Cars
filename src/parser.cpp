#include "../h/ParseNode.h"
#include "../h/Token.h"
#include "../h/Constants.h"

#include <iostream>

std::vector<Class_PN*> classNodes = {};
std::vector<Token> tokensp;

// Keeps track of the current token the parser is analyzing
int current = 0;

// To be able to access the token data from outside Type function
// --While still being able to keep Type function return type as bool
string currType = "";
string currText = "";


bool Parse_Class();
ParseNode* Parse_Value();
ParseNode* Parse_Value1();
ParseNode* Parse_Value2();
ParseNode* Parse_Value3();
NodeList* Parse_Body();
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




// Returns true if the specified type and text matches the current Token
// If text for the type checker is unspecified, text is ignored (func below)
bool Type(string type, string text) {
  Token token = tokensp.at(current);
  bool yes = token.getType() == type;
  // If no text specified, text ignored
  if (text != "") {
    yes &= token.getText() == text;
  }

  // If token matches, token advances; if not, current token remains
  if (yes) {
    current++;
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





// Returns a node of the datatype if exists
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
  
  ParseNode* newNode = new ParseNode();
  newNode->setText(tokensp.at(current-1).getText());
  newNode->setType(tokensp.at(current-1).getType());
  return newNode; 
}




// PN for data types
ParseNode* Parse_Value4_1() {
  return Parse_DT();
}

// PN for (Value)
ParseNode* Parse_Value4_2() {
  bool yes = Type(types::OPEN);

  if (!yes) { return nullptr; }

  ParseNode* node = Parse_Value();

  if (node == nullptr) { return nullptr; }

  yes = Type(types::CLOSE);
  if (!yes) { return nullptr; }

  return node;
}

// PN for DataTypes or Values in Parenthasis
ParseNode* Parse_Value4() {
  int save = current;

  ParseNode* node = Parse_Value4_1();
  if (node != nullptr) { return node; }

  ParseNode* node2 = Parse_Value4_2();
  if (node2 != nullptr) { return node2; }

  current = save;
  return nullptr;
}



// PN for Values next to * or /
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
  


// PN for values next to + or -
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



// PN for values next to comparators
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



// PN for values next to && or ||
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


// PN for declaring a variable (eg: var int numberVariable)
ParseNode* Parse_Declaration() {
  int save = current;

  bool valid = (Type(types::KEYW, KW::VAR) &&
                Type(types::TYPE) &&
                Type(types::ID));

  if (!valid) {
    current = save;
    return nullptr;
  }

  ParseNode* typeNode = new ParseNode();
  typeNode->setText(tokensp.at(current-3).getText());
  typeNode->setType(tokensp.at(current-3).getType());

  ParseNode* idNode = new ParseNode();
  idNode->setText(tokensp.at(current-1).getText());
  idNode->setType(tokensp.at(current-1).getType());

  Dual_PN* newNode = new Dual_PN();
  newNode->setText(tokensp.at(current-2).getText());
  newNode->setType(tokensp.at(current-2).getText());

  newNode->setLeft(typeNode);
  newNode->setRight(idNode);

  return newNode;
}

// PN for an id being set to a value (eg: var1 += 7 * 3)
ParseNode* Parse_Assignment() {
  int save = current;

  bool valid = (Type(types::ID) &&
                (Type(types::EQUAL) || Type(types::EQUALMOD)));

  if (!valid) {
    current = save;
    return nullptr;
  }

  ParseNode* valNode = Parse_Value();

  if (valNode == nullptr) {
    current = save;
    return nullptr;
  }

  ParseNode* idNode = new Dual_PN();
  idNode->setText(tokensp.at(save).getText());
  idNode->setType(tokensp.at(save).getType());

  Dual_PN* newNode = new Dual_PN();
  newNode->setText(tokensp.at(save+1).getText());
  newNode->setType(tokensp.at(save+1).getType());
  
  newNode->setLeft(idNode);
  newNode->setRight(valNode);

  return newNode;
}



// Returns true if a valid class structure is found
// Adds the class node to the outside list
bool Parse_Class() {
  int save = current;

  NodeList* body = new NodeList();

  bool valid = Type(types::KEYW, KW::CLASS) &&
              Type(types::CLASS) &&
              Type(types::ENDL);

  if (!valid) {
    current = save + 1;
    return false;
  }

  NodeList* bodyList = Parse_Body();
  if (bodyList == nullptr) {
    current = save + 1;
    return false;
  }
  
  Class_PN* classNode = new Class_PN();
  classNode->setType(types::KEYW);
  classNode->setText(KW::CLASS);

  classNode->setBody(bodyList);

  ParseNode* cNameNode = new ParseNode();
  cNameNode->setType(types::CLASS);
  cNameNode->setText(tokensp.at(save + 1).getText());

  classNode->setClassNode(cNameNode);

  classNodes.push_back(classNode);

  return true;
}




// PN for assignment lines (eg: var2 = 3 \n)
ParseNode* Parse_Body_1() {
  int save = current;
  
  ParseNode* asNode = Parse_Assignment();
  if (asNode == nullptr) {
    current = save;
    return nullptr;
  }
  std::cout << "---------Valid Assignment\n";

  if (!Type(types::ENDL)) {
    current = save;
    return nullptr;
  }
  std::cout << "---------Valid ENDL\n";

  return asNode;
}

// PN for lines of a value (eg: 5 * 3 \n)
ParseNode* Parse_Body_2() {
  return nullptr;

  int save = current;

  ParseNode* node = Parse_Value();

  if (node == nullptr) {
    current = save;
    return nullptr;
  }

  if (!Type(types::ENDL)) {
    current = save;
    return nullptr;
  }

  return node;
}

// PlaceHolder
ParseNode* Parse_Body_3() {
  return nullptr;
  int save = current;

  ParseNode* node = Parse_Declaration();

  if (node == nullptr) {
    current = save;
    return nullptr;
  }

  if (!Type(types::ENDL)) {
    current = save;
    return nullptr;
  }

  return node;
}


NodeList* Parse_Body() {
  int save = current;
  
  NodeList* bodyList = new NodeList();

  bool cont = true;

  // Will repeat until no new valid body element is found
  while (cont) {
    if (Type(types::FUNE)) {
      break;
    }

    cont = false;

    // For loop exists so break command wont exit while loop
    for (int i = 0; i < 1; i++) {
    //

    ParseNode* nextNode = Parse_Body_1();
    if (nextNode != nullptr) {
      bodyList->addNode(nextNode);
      cont = true;
      break;
    }

    nextNode = Parse_Body_2();
    if (nextNode != nullptr) {
      bodyList->addNode(nextNode);
      cont = true;
      break;
    }

    nextNode = Parse_Body_3();
    if (nextNode != nullptr) {
      bodyList->addNode(nextNode);
      cont = true;
      break;
    }

    // This bracket goes to the for loop
    }
    //
  }

  return bodyList;
}




// Depth first Parse Tree search
void PrintTree() {
  for (int i = 0; i < classNodes.size(); i++) {
    ParseNode* classNode = classNodes.at(i);
    std::vector<ParseNode*> depthList = {classNode};

    ParseNode* currentNode = classNode;
    classNode->begin();

    std::cout << classNode->getText() << "\n";

    while (depthList.size() != 0) {
      if (depthList.size() > 30) {
        std::cout << "----------NODE DEPTH LIMIT REACHED----------\n";
        return;
      }
      ParseNode* possibleNext = currentNode->next();
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
