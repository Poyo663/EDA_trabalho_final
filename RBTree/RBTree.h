#include <vector>

enum COLOR {
  RED = 0,
  BLACK = 1,
};;

class TreeNode {
public:
  TreeNode();
  TreeNode(TreeNode *p, TreeNode *left, TreeNode *right, int value,
           enum COLOR color);
  ~TreeNode() ;

  TreeNode *parent;
  TreeNode *left;
  TreeNode *right;
  int value;
  enum COLOR color;

private:
};

class RBTree {
public:
  RBTree();
  RBTree(int value);
  ~RBTree();
  TreeNode* getTree();
  TreeNode* getNil();
  void addNode(int v);
  void removeNode(int v);
  TreeNode* search(int v);
  TreeNode* getSuccessor(TreeNode* n);
  TreeNode* getPredecessor(TreeNode* n);
  int size();
  int minimum();
  int maximum();
  std::vector<int>* getKeys();
private:
  TreeNode* root;
  TreeNode* NIL;


  void fixInsertionColor(TreeNode* novo);
  void rightRotate(TreeNode *n);
  void leftRotate(TreeNode *n);
  void remove(TreeNode* n);
  void fixDeletionColor(TreeNode* n);
  int sizeHelper(TreeNode* n);
};

