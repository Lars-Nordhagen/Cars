#include <string>
#include <vector>
using std::string;

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

  virtual ParseNode* getLeft() {return nullptr;}
  virtual ParseNode* getRight() {return nullptr;}

private:
  string type;
  string specType;
  string text;

};




class NodeList {
public:
  NodeList () {
    nodes = {};
  }

  ParseNode getNode(int index) {
    return nodes.at(index);
  }
  void addNode(ParseNode node) {
    nodes.push_back(node);
  }
  void addNode(ParseNode node, int index) {
    nodes.insert(nodes.begin() + index, node);
  }

private:
  std::vector<ParseNode> nodes;
};



// ClassNode -> body Nodes
class Class_PN : public ParseNode {
public: 
  Class_PN(){}

  void setClassNode(ParseNode node) {
    classNode = node;
  }

  ParseNode* getClassNode() {
    return &classNode;
  }

  ParseNode* getLeft() {
    return getClassNode();
  }

  NodeList* body = new NodeList(); 
private:
  ParseNode classNode;
};


// CondNode -> IfBody Nodes -> ElseBody Nodes
class If_PN : public ParseNode {
public:
  If_PN(){}

  void setCondNode(ParseNode node) {
    condNode = node;
  }
  ParseNode* getCondNode() {
    return &condNode;
  }

  ParseNode* getLeft() {
    return getCondNode();
  }

  NodeList* ifBody = new NodeList();
  NodeList* elseBody = new NodeList();
private:
  ParseNode condNode;
};


// CondNode -> Body Nodes
class While_PN : public ParseNode {
public:
  While_PN(){}

  void setCondNode(ParseNode node) {
    condNode = node;
  }
  ParseNode* getCondNode() {
    return &condNode;
  }

  ParseNode* getLeft() {
    return getCondNode();
  }

  NodeList* body = new NodeList();
private:
  ParseNode condNode; 
};


// ParamNode -> Body Nodes
class Func_PN : public ParseNode {
public:
  Func_PN(){}

  void setParamNode(ParseNode node) {
    paramNode = node;
  }
  ParseNode* getParamNode() {
    return &paramNode;
  }

  ParseNode* getLeft() {
    return getParamNode();
  }

  NodeList* body = new NodeList();
private:
  ParseNode paramNode;
};


// LeftNode -> RightNode
class Dual_PN : public ParseNode {
public:
  Dual_PN(){}

  void setLeft(ParseNode node) {
    leftNode = node;
  }
  void setRight(ParseNode node) {
    rightNode = node;
  }

  ParseNode* getLeft() {
    return &leftNode;
  }
  ParseNode* getRight() {
    return &rightNode;
  }
private:
  ParseNode leftNode;
  ParseNode rightNode;
};
