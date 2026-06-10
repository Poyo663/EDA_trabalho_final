#include "RBTree.h"
#include <stack>
#include <stdlib.h>
#include <vector>

template <std::totally_ordered T> TreeNode<T>::TreeNode() {
  this->parent = nullptr;
  this->left = nullptr;
  this->right = nullptr;
  this->value = 0;
  this->color = BLACK;
}

template <std::totally_ordered T>
TreeNode<T>::TreeNode(TreeNode<T> *p, TreeNode<T> *left, TreeNode<T> *right,
                      T value, enum COLOR color) {
  this->parent = p;
  this->left = left;
  this->right = right;
  this->value = value;
  this->color = color;
}

template <std::totally_ordered T> TreeNode<T>::~TreeNode() {
  delete this->left;
  delete this->right;
}

template <std::totally_ordered T> RBTree<T>::RBTree() {
  this->NIL = new TreeNode<T>(nullptr, nullptr, nullptr, -1, BLACK);
  this->root = this->NIL;
}

template <std::totally_ordered T> RBTree<T>::RBTree(T value) {
  this->NIL = new TreeNode<T>(nullptr, nullptr, nullptr, -1, BLACK);
  this->root = new TreeNode<T>(this->NIL, this->NIL, this->NIL, value, BLACK);
}

template <std::totally_ordered T> RBTree<T>::~RBTree() {
  delete this->root;
  this->NIL = new TreeNode<T>(nullptr, nullptr, nullptr, -1, BLACK);
}

template <std::totally_ordered T> TreeNode<T> *RBTree<T>::getTree() {
  return this->root;
}

template <std::totally_ordered T> TreeNode<T> *RBTree<T>::getNil() {
  return this->NIL;
}

template <std::totally_ordered T> void RBTree<T>::rightRotate(TreeNode<T> *n) {
  TreeNode<T> *right_child_of_left_child = n->left;
  n->left = right_child_of_left_child->right;
  if (right_child_of_left_child->right != this->NIL)
    right_child_of_left_child->left->parent = n;
  right_child_of_left_child->parent = n->parent;
  if (n->parent == this->NIL)
    this->root = right_child_of_left_child;
  else if (n == n->parent->left)
    n->parent->left = right_child_of_left_child;
  else
    n->parent->left = right_child_of_left_child;
  right_child_of_left_child->left = n;
  n->parent = right_child_of_left_child;
}

template <std::totally_ordered T> void RBTree<T>::leftRotate(TreeNode<T> *n) {
  TreeNode<T> *right_child_of_right_child = n->right;
  n->right = right_child_of_right_child->right;
  if (right_child_of_right_child->right != this->NIL)
    right_child_of_right_child->right->parent = n;
  right_child_of_right_child->parent = n->parent;
  if (n->parent == this->NIL)
    this->root = right_child_of_right_child;
  else if (n == n->parent->right)
    n->parent->right = right_child_of_right_child;
  else
    n->parent->right = right_child_of_right_child;
  right_child_of_right_child->right = n;
  n->parent = right_child_of_right_child;
}

template <std::totally_ordered T>
void RBTree<T>::fixInsertionColor(TreeNode<T> *novo) {
  TreeNode<T> *parent = nullptr;
  TreeNode<T> *grandparent = nullptr;

  while ((novo != root) && (novo->color != BLACK) &&
         (novo->parent->color == RED)) {
    parent = novo->parent;
    grandparent = novo->parent->parent;
    if (parent == grandparent->left) {
      TreeNode<T> *uncle = grandparent->right;

      if (uncle != nullptr && uncle->color == RED) {
        grandparent->color = RED;
        parent->color = BLACK;
        uncle->color = BLACK;
        novo = grandparent;
      } else {
        if (novo == parent->right) {
          leftRotate(parent);
          novo = parent;
          parent = novo->parent;
        }
        rightRotate(grandparent);
        std::swap(parent->color, grandparent->color);
        novo = parent;
      }
    } else {
      TreeNode<T> *uncle = grandparent->left;

      if (uncle != nullptr && uncle->color == RED) {
        grandparent->color = RED;
        parent->color = BLACK;
        uncle->color = BLACK;
        novo = grandparent;
      } else {
        if (novo == parent->left) {
          rightRotate(parent);
          novo = parent;
          parent = novo->parent;
        }
        leftRotate(grandparent);
        std::swap(parent->color, grandparent->color);
        novo = parent;
      }
    }
  }
  root->color = BLACK;
}

template <std::totally_ordered T> TreeNode<T> *RBTree<T>::search(T v) {
  TreeNode<T> *n = this->root;
  while (n != this->NIL) {
    if (v < n->value)
      n = n->left;
    else if (v > n->value)
      n = n->right;
    else
      return n;
  }
  return nullptr;
}

template <std::totally_ordered T> void RBTree<T>::addNode(T v) {
  if (this->root == this->NIL) {
    this->root = new TreeNode<T>(this->NIL, this->NIL, this->NIL, v, BLACK);
  }
  TreeNode<T> *n = this->root;
  TreeNode<T> *y = this->NIL;
  while (n != this->NIL) {
    y = n;
    if (v < n->value)
      n = n->left;
    else if (v > n->value)
      n = n->right;
    else
      return;
  }
  TreeNode<T> *ans = new TreeNode<T>();
  ans->parent = y;
  ans->value = v;
  if (y == this->NIL)
    this->root = ans;
  else if (v < y->value)
    y->left = ans;
  else
    y->right = ans;
  ans->left = this->NIL;
  ans->right = this->NIL;
  ans->color = RED;
  if (y->color == RED)
    this->fixInsertionColor(ans);
}

template <std::totally_ordered T> void RBTree<T>::removeNode(T v) {
  if (this->root == this->NIL) {
    return;
  }
  TreeNode<T> *n = this->search(v);
  if (n == nullptr)
    return;
  this->remove(n);
}

template <std::totally_ordered T> void RBTree<T>::remove(TreeNode<T> *n) {
  if (n->right != this->NIL && n->left != this->NIL) {
    TreeNode<T> *m = n;
    n = this->getSuccessor(n);
    T a = m->value;
    m->value = n->value;
    n->value = a;
  }

  TreeNode<T> *child = n->left != this->NIL ? n->left : n->right;
  child->parent = n->parent;
  n == n->parent->right ? n->parent->right = child : n->parent->left = child;
  if (n->color == BLACK && child->color == RED)
    child->color = BLACK;
  n->left = nullptr;
  n->right = nullptr;
  if (!(n->color == BLACK && child->color == BLACK))
    return delete n;
  delete n;
  this->fixDeletionColor(child);
}

template <std::totally_ordered T>
TreeNode<T> *RBTree<T>::getSuccessor(TreeNode<T> *n) {
  TreeNode<T> *current = n->right;
  while (current->left != this->NIL) {
    current = current->left;
  }
  return current;
}

template <std::totally_ordered T>
TreeNode<T> *RBTree<T>::getPredecessor(TreeNode<T> *n) {
  TreeNode<T> *current = n->left;
  while (current->right != this->NIL) {
    current = current->right;
  }
  return current;
}

template <std::totally_ordered T>
void RBTree<T>::fixDeletionColor(TreeNode<T> *n) {
  while (n != root && n->color == BLACK) {
    if (n == n->parent->left) {
      TreeNode<T> *sibling = n->parent->right;
      if (sibling->color == RED) {
        sibling->color = BLACK;
        n->parent->color = RED;
        this->leftRotate(n->parent);
        sibling = n->parent->right;
      }

      if (sibling->left->color == BLACK && sibling->right->color == BLACK) {
        sibling->color = RED;
        n = n->parent;
      } else {
        if (sibling->right->color == BLACK) {
          sibling->left->color = BLACK;
          sibling->color = RED;
          this->rightRotate(sibling);
          sibling = n->parent->right;
        }
        sibling->color = n->parent->color;
        n->parent->color = BLACK;
        sibling->right->color = BLACK;
        this->leftRotate(n->parent);
        n = root;
      }
    } else {
      TreeNode<T> *sibling = n->parent->left;
      if (sibling->color == RED) {
        sibling->color = BLACK;
        n->parent->color = RED;
        this->rightRotate(n->parent);
        sibling = n->parent->left;
      }

      if (sibling->right->color == BLACK && sibling->left->color == BLACK) {
        sibling->color = RED;
        n = n->parent;
      }
      else {
        if (sibling->left->color == BLACK) {
          sibling->right->color = BLACK;
          sibling->color = RED;
          this->leftRotate(sibling);
          sibling = n->parent->left;
        }
        sibling->color = n->parent->color;
        n->parent->color = BLACK;
        sibling->left->color = BLACK;
        this->rightRotate(n->parent);
        n = root;
      }
    }
  }
  n->color = BLACK;
}

template <std::totally_ordered T> T RBTree<T>::minimum() {
  TreeNode<T> *current = this->root;
  while (current->left != this->NIL) {
    current = current->left;
  }
  return current->value;
}

template <std::totally_ordered T> T RBTree<T>::maximum() {
  TreeNode<T> *current = this->root;
  while (current->right != this->NIL) {
    current = current->right;
  }
  return current->value;
}

template <std::totally_ordered T> int RBTree<T>::size() {
  return this->sizeHelper(this->root);
}

template <std::totally_ordered T> int RBTree<T>::sizeHelper(TreeNode<T> *n) {
  int resp = 0;
  std::stack<TreeNode<T> *> next;
  next.push(this->root);
  TreeNode<T> *current = next.top();
  while (!next.empty()) {
    resp++;
    next.pop();
    if (current->right != this->getNil())
      next.push(current->right);
    if (current->left != this->getNil())
      next.push(current->left);
    TreeNode<T> *current = next.top();
  }
  return resp;
}

template <std::totally_ordered T> std::vector<T> *RBTree<T>::getKeys() {
  std::vector<T> *v = new std::vector<T>();
  if (this->root == this->NIL)
    return v;
  std::stack<TreeNode<T> *> s;
  s.push(this->root);
  while (!s.empty()) {
    if (s.top() == this->NIL) {
      s.pop();
      continue;
    }
    TreeNode<T> *temp = s.top();
    v->push_back(temp->value);
    s.pop();
    s.push(temp->right);
    s.push(temp->left);
  }
  return v;
}
