#include <queue>
#include <stack>
#include <stdlib.h>

#include "AVL-tree.h"

AVLNode::AVLNode(int v) {
  this->value = v;
  this->left = nullptr;
  this->right = nullptr;
}

int AVLNode::height() {
  if (nullptr == this)
    return 0;
  std::queue<AVLNode *> q;
  q.push(this);
  int height = 0;
  while (!q.empty()) {
    int nodeCount = q.size();
    height++;
    while (nodeCount > 0) {
      AVLNode *front = q.front();
      q.pop();
      if (front->left != nullptr)
        q.push(front->left);
      if (front->right != nullptr)
        q.push(front->right);
      nodeCount--;
    }
  }
  return height;
}

int AVLNode::balance() { return this->right->height() - this->left->height(); }

// return 1 if node is balanced
// return 0 otherwise
int AVLNode::isBalanced() { return abs(this->balance()) <= 1; }

// fixes a node if it is not balanced
// does nothing otherwise
void AVLNode::fixNode() {
  if (this->isBalanced())
    return;
  int b = this->balance();
}

AVLNode *AVLNode::rightRotate() {
  AVLNode *right_child_of_left_child = this->left;
  this->left = right_child_of_left_child->right;
  right_child_of_left_child->right = this;
  return right_child_of_left_child;
}

AVLNode *AVLNode::leftRotate() {
  AVLNode *left_child_of_right_child = this->right;
  this->right = left_child_of_right_child->left;
  left_child_of_right_child->left = this;
  return left_child_of_right_child;
}

AVLNode *AVLNode::doubleRightRotate() {
  this->right->leftRotate();
  return this->rightRotate();
}

AVLNode *AVLNode::doubleLeftRotate() {
  this->left->rightRotate();
  return this->leftRotate();
}

AVLNode *AVLNode::fixupDeletion() {
  int bal = this->balance();

  if (bal > 1 && this->right->balance() >= 0)
    return this->leftRotate();
  else if (bal > 1 && this->balance() < 0) {
    this->right = this->right->rightRotate();
    return this->leftRotate();
  } else if (bal < -1 && this->left->balance() <= 0)
    return this->rightRotate();
  else if (bal < -1 && this->left->balance() > 0) {
    this->left = this->left->leftRotate();
    return this->rightRotate();
  }

  return this;
}

AVLNode *AVLNode::removeSuccessor() {
  std::stack<AVLNode *> s;
  AVLNode *current = this->right;
  while (current->left != nullptr) {
    s.push(current);
    current = current->left;
  }
  this->value = current->value;
  AVLNode *replacement = current->right;
  delete current;
  if (s.empty())
    return replacement;
  AVLNode *parent = s.top();
  parent->left = replacement;
  while (!s.empty()) {
    current = s.top();
    s.pop();
    AVLNode *balancedNode = current->fixupDeletion();
    if (!s.empty()) {
      parent = s.top();
      if (parent->left == current)
        parent->left = balancedNode;
      else
        parent->right = balancedNode;
    } else
      return balancedNode;
  }

  return nullptr;
}

// TODO: make this code more readable
AVLNode *AVLNode::addNode(int v) {
  std::stack<AVLNode *> s;
  AVLNode *current = this;
  while (current != nullptr) {
    s.push(current);
    if (current->value == v)
      return this;
    else if (current->value < v)
      current = current->right;
    else
      current = current->left;
  }
  current = s.top();
  AVLNode *newNode = new AVLNode(v);
  if (current->value < v)
    current->right = newNode;
  else
    current->left = newNode;
  while (!s.empty()) {
    current = s.top();
    s.pop();
    int balance = current->balance();
    AVLNode *rotatedRoot = current;
    if (balance > 1 && v < current->left->value) {
      rotatedRoot = current->rightRotate();
    } else if (balance < -1 && v > current->right->value) {
      rotatedRoot = current->leftRotate();
    } else if (balance > 1 && v > current->left->value) {
      current->left = current->left->leftRotate();
      rotatedRoot = current->rightRotate();
    } else if (balance < -1 && v < current->right->value) {
      current->right = current->right->rightRotate();
      rotatedRoot = current->leftRotate();
    }
    if (!s.empty()) {
      AVLNode *parent = s.top();
      if (parent->left == current) {
        parent->left = rotatedRoot;
      } else {
        parent->right = rotatedRoot;
      }
    } else {
      return rotatedRoot;
    }
  }
  return nullptr;
}

AVLNode *AVLNode::deleteNode(int v) {
  AVLNode *current = this;
  std::stack<AVLNode *> s;
  s.push(current);
  while (current != nullptr && current->value != v) {
    s.push(current);
    if (v < current->value) {
      current = current->left;
    } else {
      current = current->right;
    }
  }
  if (current == nullptr)
    return this;
  AVLNode *nodeToDelete = current;
  AVLNode *replacement = nullptr;
  if (nodeToDelete->left != nullptr && nodeToDelete->right != nullptr) {
    nodeToDelete->right = nodeToDelete->removeSuccessor();
    nodeToDelete = nullptr;
  }
  if (nodeToDelete != nullptr) {
    if (nodeToDelete->left != nullptr) {
      replacement = nodeToDelete->left;
    } else {
      replacement = nodeToDelete->right;
    }
    if (!s.empty()) {
      AVLNode *parent = s.top();
      if (parent->left == nodeToDelete) {
        parent->left = replacement;
      } else {
        parent->right = replacement;
      }
    } else {
      delete nodeToDelete;
      return replacement;
    }
    delete nodeToDelete;
  }
  while (!s.empty()) {
    current = s.top();
    s.pop();
    int balance = current->balance();
    AVLNode *rotatedRoot = current;

    if (balance > 1) {
      if (current->left->balance() >= 0)
        rotatedRoot = current->rightRotate(); // LL
      else
        rotatedRoot = current->doubleLeftRotate();

    } else if (balance < -1) {
      if (current->right->balance() <= 0)
        rotatedRoot = current->leftRotate();
      else
        rotatedRoot = current->doubleRightRotate();
    }

    if (!s.empty()) {
      AVLNode *parent = s.top();
      if (parent->left == current) {
        parent->left = rotatedRoot;
      } else {
        parent->right = rotatedRoot;
      }
    } else {
      return rotatedRoot;
    }
  }
  return this;
}
