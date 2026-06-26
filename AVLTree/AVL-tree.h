#include <algorithm>
#include <iostream>
#include <stack>

template <typename T> struct Node {
  T data;
  int height;
  Node *left;
  Node *right;

  Node(T val) {
    this->data = val;
    this->height = 1;
    this->left = nullptr;
    this->right = nullptr;
  }
};

template <typename T> class AVLTree {
private:
  Node<T> *root;

  int height(Node<T> *n);
  void updateHeight(Node<T> *n);
  int getBalance(Node<T> *n);
  Node<T> *rotateRight(Node<T> *y);
  Node<T> *rotateLeft(Node<T> *x);
  Node<T> *balanceNode(Node<T> *subRoot);
  void connectToParent(Node<T> *parent, Node<T> *oldChild, Node<T> *newChild);

public:
  unsigned int comparisons;
  unsigned int rotations;

  AVLTree() {
    this->root = nullptr;
    this->comparisons = 0;
    this->rotations = 0;
  }
  ~AVLTree();
  void insert(T key);
  void remove(T key);
  AVLTree<T>* search(T key);
  void print();
};
