class AVLNode {
public:
  int value;
  AVLNode(int v);
  int height();
  int balance();
  int isBalanced();

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
  AVLNode* addNode(int v);
  AVLNode *deleteNode(int v);
};
