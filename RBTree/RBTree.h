#include <vector>
#include <concepts>

enum COLOR {
  RED = 0,
  BLACK = 1,
};;

template <std::totally_ordered T> class TreeNode {
public:
  T value;
  TreeNode *parent;
  TreeNode *left;
  TreeNode *right;
  enum COLOR color;
  TreeNode();
  TreeNode(TreeNode *p, TreeNode *left, TreeNode *right, T value,
           enum COLOR color);
  ~TreeNode() ;


private:
};

template <std::totally_ordered T> class RBTree {
public:
  RBTree();
  RBTree(T value);
  ~RBTree();
  TreeNode<T>* getTree();
  TreeNode<T>* getNil();
  void addNode(T v);
  void removeNode(T v);
  TreeNode<T>* search(T v);
  TreeNode<T>* getSuccessor(TreeNode<T>* n);
  TreeNode<T>* getPredecessor(TreeNode<T>* n);
  int size();
  T minimum();
  T maximum();
  std::vector<T>* getKeys();
private:
  TreeNode<T>* root;
  TreeNode<T>* NIL;


  void fixInsertionColor(TreeNode<T>* novo);
  void rightRotate(TreeNode<T> *n);
  void leftRotate(TreeNode<T> *n);
  void remove(TreeNode<T>* n);
  void fixDeletionColor(TreeNode<T>* n);
  int sizeHelper(TreeNode<T>* n);
};

