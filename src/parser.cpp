#include "../h/ParseNode.h"
#include "../h/Token.h"

#include <iostream>

std::vector<Class_PN*> classNodes = {};
std::vector<Token> tokensp;

int current = 0;
bool Parse_Class();
bool Parse_Body(NodeList* body);
void PrintTree();
std::vector<Class_PN*> parseTokens (std::vector<Token> p_tokens) {
  tokensp = p_tokens;
  int tokenspLength = tokensp.size();
  std::cout << "PARSING\n";
  
  while (current < tokenspLength) {
    Parse_Class();
  }
  std::cout << "DONEPARSE\n";

  PrintTree();

  return classNodes;
}

bool Type(string type, string text) {
  Token token = tokensp.at(current);
  bool yes = token.getType() == type;
  current++;
  if (text != "") {
    yes = yes && token.getText() == text;
  }
  std::cout << token.getType() << " " << token.getText() << " " << yes << "\n";
  if (!yes) {
    std::cout << "     expected: " << type << " " << text << "\n";
  }
  return yes;
}

bool Type (string type) {
  return Type(type, "");
}



bool Parse_Class_1(NodeList* body) {
  return Type("KEYW", "class") && Type("CLSS") && Type("ENDL") && Parse_Body(body);
}
bool Parse_Class() {
  int save = current;

  NodeList* body = new NodeList();
  
  bool yes = Parse_Class_1(body);

  if (!yes) {
    current = save + 1;
  } else {
    Class_PN* cn = new Class_PN();
    cn->setType("KEYW");
    cn->setText("class");

    cn->setBody(body);
    
    ParseNode* classNode = new ParseNode();
    classNode->setType("CLSS");
    classNode->setText(tokensp.at(save + 1).getText());

    cn->setClassNode(classNode);
    //cn

    classNodes.push_back(cn);
  }
  return yes;
}





bool Parse_Body(NodeList* body);
bool Parse_Body_1(NodeList* body) {
  bool yes = Type("ID  ") && Type("EQUA") && Type("NUM ") && Type("ENDL") && Parse_Body(body);
  if (yes) {
    Dual_PN* node = new Dual_PN();
    node->setType("EQUA");
    node->setText("=");

    ParseNode* lNode = new ParseNode("ID  ", "", "");
    ParseNode* rNode = new ParseNode("NUM ", "", "");
    node->setLeft(lNode);
    node->setRight(rNode);

    body->addNode(node, 0);
  }
  return yes;
}
bool Parse_Body_2(NodeList* body) {
  bool yes = Type("ID  ") && Type("EQUA") && Type("QUO ") && Type("ENDL") && Parse_Body(body);
  if (yes) {
    Dual_PN* node = new Dual_PN();
    node->setType("EQUA");
    node->setText("=");

    ParseNode* lNode = new ParseNode("ID  ", "");
    ParseNode* rNode = new ParseNode("QUO ", "");
    node->setLeft(lNode);
    node->setRight(rNode);

    body->addNode(node, 0);  }
  return yes;
}
bool Parse_Body_3(NodeList* body) {
  return Type("FUNE");
}
bool Parse_Body(NodeList* body) {
  int save = current;
  bool yes = (current = save, Parse_Body_1(body)) || (current = save, Parse_Body_2(body)) || (current = save, Parse_Body_3(body));


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
