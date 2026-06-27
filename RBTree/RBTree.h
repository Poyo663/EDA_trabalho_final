#ifndef RB
#define RB

#include <algorithm>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

enum Color { RED, BLACK };

template <typename T> struct RBNode {
  T data;
  unsigned short n;
  Color color;
  RBNode *left;
  RBNode *right;

  RBNode(T val) {
    this->data = val;
    this->n = 1;
    this->color = RED;
    this->left = nullptr;
    this->right = nullptr;
  }
};

template <typename T> class RedBlackTree {
private:
  RBNode<T> *root;

  Color getColor(RBNode<T> *n);
  void setColor(RBNode<T> *n, Color c);
  RBNode<T> *rotateLeft(RBNode<T> *x);
  RBNode<T> *rotateRight(RBNode<T> *y);
  void connectToParent(RBNode<T> *parent, RBNode<T> *oldChild, RBNode<T> *newChild);

public:
  unsigned int comparisons;
  unsigned int rotations;
  unsigned int color_changes;

  RedBlackTree() {
    this->root = nullptr;
    this->comparisons = 0;
    this->rotations = 0;
    this->color_changes = 0;
  }
  ~RedBlackTree();
  void insert(T key);
  RBNode<T>* search(T key);
  void print();
  std::vector<T> getKeys();
};

#include "RBTree.cpp"
#endif 
