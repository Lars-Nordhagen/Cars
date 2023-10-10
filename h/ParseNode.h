#include <string>
using std::string;

class ParseNode;

class ParseNode {
public:
  ParseNode(){}
  
  ParseNode(string p_type, string p_text) {
    ParseNode(p_type, "", p_text);
  }
  ParseNode(string p_type, string p_specType, string p_text) {
    type = p_type;
    specType = p_specType;
    text = p_text;
  }

  void setLeft(ParseNode node) {
    leftNode = node;
  }

  void setRight(ParseNode node) {
    rightNode = node;
  }

  ParseNode getLeft() {
    return leftNode;
  }

  ParseNode getRight() {
    return rightNode;
  }

private:
  string type;
  string specType;
  string text;

  ParseNode leftNode;
  ParseNode rightNode;
};


class Class_PN : public ParseNode {
public: 
  Class_PN(){}

  void setClassNode(ParseNode node) {
    setLeft(node);
  }

  ParseNode getClassNode() {
    return getLeft();
  }

  void addBodyNode(ParseNode node) {
    //bodyNodes.add(node);
  }

  ParseNode getBodyNode(int index) {
    return bodyNodes[index];
  }
private:
  ParseNode bodyNodes[];
};

class If_PN : public ParseNode {

};

class While_PN : public ParseNode {

};

class Dual_PN : public ParseNode {

};
