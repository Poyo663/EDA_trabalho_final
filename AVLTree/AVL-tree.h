#include <concepts>

template <std::totally_ordered T> class AVLNode {
public:
  T value;
  AVLNode(T v);
  int height();
  int balance();
  bool isBalanced();

private:
  AVLNode *left;
  AVLNode *right;
  void fixNode();
  AVLNode *rightRotate();
  AVLNode *leftRotate();
  AVLNode *doubleRightRotate();
  AVLNode *doubleLeftRotate();
  AVLNode *fixupDeletion();
  AVLNode *removeSuccessor();
  AVLNode *addNode(int v);
  AVLNode *deleteNode(int v);
};
