#include "RBTree.h"
#include <stack>
#include <stdlib.h>
#include <vector>

TreeNode::TreeNode() {
  this->parent = nullptr;
  this->left = nullptr;
  this->right = nullptr;
  this->value = 0;
  this->color = BLACK;
}

TreeNode::TreeNode(TreeNode *p, TreeNode *left, TreeNode *right, int value,
                   enum COLOR color) {
  this->parent = p;
  this->left = left;
  this->right = right;
  this->value = value;
  this->color = color;
}

TreeNode::~TreeNode() {
  delete this->left;
  delete this->right;
}

RBTree::RBTree() {
  this->NIL = new TreeNode(nullptr, nullptr, nullptr, -1, BLACK);
  this->root = this->NIL;
}

RBTree::RBTree(int value) {
  this->NIL = new TreeNode(nullptr, nullptr, nullptr, -1, BLACK);
  this->root = new TreeNode(this->NIL, this->NIL, this->NIL, value, BLACK);
}
RBTree::~RBTree() {
  delete this->root;
  this->NIL = new TreeNode(nullptr, nullptr, nullptr, -1, BLACK);
}

TreeNode *RBTree::getTree() { return this->root; }

TreeNode *RBTree::getNil() { return this->NIL; }

void RBTree::rightRotate(TreeNode *n) {
  TreeNode *right_child_of_left_child = n->left;
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

void RBTree::leftRotate(TreeNode *n) {
  TreeNode *right_child_of_right_child = n->right;
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

void RBTree::fixInsertionColor(TreeNode *novo) {
  TreeNode *parent = nullptr;
  TreeNode *grandparent = nullptr;

  while ((novo != root) && (novo->color != BLACK) &&
         (novo->parent->color == RED)) {
    parent = novo->parent;
    grandparent = novo->parent->parent;

    if (parent == grandparent->left) {
      TreeNode *uncle = grandparent->right;

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
      TreeNode *uncle = grandparent->left;

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

TreeNode *RBTree::search(int v) {
  TreeNode *n = this->root;
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

void RBTree::addNode(int v) {
  if (this->root == this->NIL) {
    this->root = new TreeNode(this->NIL, this->NIL, this->NIL, v, BLACK);
  }

  TreeNode *n = this->root;
  TreeNode *y = this->NIL;
  while (n != this->NIL) {
    y = n;
    if (v < n->value)
      n = n->left;
    else if (v > n->value)
      n = n->right;
    else
      return;
  }

  TreeNode *ans = new TreeNode();
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

void RBTree::removeNode(int v) {
  if (this->root == this->NIL) {
    return;
  }

  TreeNode *n = this->search(v);
  if (n == nullptr)
    return;
  this->remove(n);
}

void RBTree::remove(TreeNode *n) {
  if (n->right != this->NIL && n->left != this->NIL) {
    TreeNode *m = n;
    n = this->getSuccessor(n);
    int a = m->value;
    m->value = n->value;
    n->value = a;
  }

  TreeNode *child = n->left != this->NIL ? n->left : n->right;

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

TreeNode *RBTree::getSuccessor(TreeNode *n) {
  TreeNode *current = n->right;

  while (current->left != this->NIL) {
    current = current->left;
  }

  return current;
}

TreeNode *RBTree::getPredecessor(TreeNode *n) {
  TreeNode *current = n->left;

  while (current->right != this->NIL) {
    current = current->right;
  }

  return current;
}

void RBTree::fixDeletionColor(TreeNode *n) {
  while (n != root && n->color == BLACK) {
    if (n == n->parent->left) {
      TreeNode *sibling = n->parent->right;

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
      TreeNode *sibling = n->parent->left;

      if (sibling->color == RED) {
        sibling->color = BLACK;
        n->parent->color = RED;
        this->rightRotate(n->parent);
        sibling = n->parent->left;
      }

      if (sibling->right->color == BLACK && sibling->left->color == BLACK) {
        sibling->color = RED;
        n = n->parent;
      } else {
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

int RBTree::minimum() {
  TreeNode *current = this->root;
  while (current->left != this->NIL) {
    current = current->left;
  }
  return current->value;
}

int RBTree::maximum() {
  TreeNode *current = this->root;
  while (current->right != this->NIL) {
    current = current->right;
  }
  return current->value;
}

int RBTree::size() { return this->sizeHelper(this->root); }

int RBTree::sizeHelper(TreeNode *n) {
  int resp = 0;
  std::stack<TreeNode *> next;
  next.push(this->root);
  TreeNode *current = next.top();
  while (!next.empty()) {
    resp++;
    next.pop();
    if(current->right != this->getNil())
      next.push(current->right);
    if(current->left != this->getNil())
      next.push(current->left);
    TreeNode *current = next.top();
  }
  return resp;
}

std::vector<int> *RBTree::getKeys() {
  std::vector<int> *v = new std::vector<int>();
  if (this->root == this->NIL)
    return v;
  std::stack<TreeNode *> s;
  s.push(this->root);
  while (!s.empty()) {
    if (s.top() == this->NIL) {
      s.pop();
      continue;
    }
    TreeNode *temp = s.top();
    v->push_back(temp->value);
    s.pop();
    s.push(temp->right);
    s.push(temp->left);
  }
  return v;
}
