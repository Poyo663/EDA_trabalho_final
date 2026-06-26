#include <algorithm>
#include <iostream>
#include <stack>
#include <string>

enum Color { RED, BLACK };

template <typename T> struct Node {
  T data;
  Color color;
  Node *left;
  Node *right;

  Node(T val) {
    this->data = val;
    this->color = RED;
    this->left = nullptr;
    this->right = nullptr;
  }
};

template <typename T> class RedBlackTree {
private:
  Node<T> *root;

  Color getColor(Node<T> *n);
  void setColor(Node<T> *n, Color c);
  Node<T> *rotateLeft(Node<T> *x);
  Node<T> *rotateRight(Node<T> *y);
  void connectToParent(Node<T> *parent, Node<T> *oldChild, Node<T> *newChild);

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
  bool search(T key);
  void print();
};
