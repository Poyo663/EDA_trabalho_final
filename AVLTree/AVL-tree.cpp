#include "AVL-tree.h"

template <typename T> int AVLTree<T>::height(Node<T> *n) {
  return n ? n->height : 0;
}

template <typename T> void AVLTree<T>::updateHeight(Node<T> *n) {
  if (n) {
    n->height = 1 + std::max(height(n->left), height(n->right));
  }
}

template <typename T> int AVLTree<T>::getBalance(Node<T> *n) {
  return n ? height(n->left) - height(n->right) : 0;
}

template <typename T> Node<T> *AVLTree<T>::rotateRight(Node<T> *y) {
  this->rotations++;
  Node<T> *x = y->left;
  Node<T> *T2 = x->right;

  x->right = y;
  y->left = T2;

  updateHeight(y);
  updateHeight(x);

  return x;
}

template <typename T> Node<T> *AVLTree<T>::rotateLeft(Node<T> *x) {
  this->rotations++;
  Node<T> *y = x->right;
  Node<T> *T2 = y->left;

  y->left = x;
  x->right = T2;

  updateHeight(x);
  updateHeight(y);

  return y;
}

template <typename T> Node<T> *AVLTree<T>::balanceNode(Node<T> *subRoot) {
  updateHeight(subRoot);
  int balance = getBalance(subRoot);

  this->comparisons += 2;
  if (balance > 1 && getBalance(subRoot->left) >= 0) {
    return rotateRight(subRoot);
  }

  this->comparisons += 2;
  if (balance > 1 && getBalance(subRoot->left) < 0) {
    subRoot->left = rotateLeft(subRoot->left);
    return rotateRight(subRoot);
  }

  this->comparisons += 2;
  if (balance < -1 && getBalance(subRoot->right) <= 0) {
    return rotateLeft(subRoot);
  }

  this->comparisons += 2;
  if (balance < -1 && getBalance(subRoot->right) > 0) {
    subRoot->right = rotateRight(subRoot->right);
    return rotateLeft(subRoot);
  }

  return subRoot;
}

template <typename T>
void AVLTree<T>::connectToParent(Node<T> *parent, Node<T> *oldChild,
                                 Node<T> *newChild) {
  if (!parent) {
    this->comparisons += 1;
    root = newChild;
  } else if (parent->left == oldChild) {
    this->comparisons += 2;
    parent->left = newChild;
  } else {
    this->comparisons += 2;
    parent->right = newChild;
  }
}

template <typename T> AVLTree<T>::~AVLTree() {
  if (!root)
    return;
  std::stack<Node<T> *> s;
  s.push(root);
  while (!s.empty()) {
    Node<T> *curr = s.top();
    s.pop();
    if (curr->left)
      s.push(curr->left);
    if (curr->right)
      s.push(curr->right);
    delete curr;
  }
}

template <typename T> void AVLTree<T>::insert(T key) {
  Node<T> *newNode = new Node<T>(key);
  this->comparisons += 1;
  if (!root) {
    root = newNode;
    return;
  }

  std::stack<Node<T> *> path;
  Node<T> *curr = root;

  while (curr) {
    this->comparisons += 2;
    path.push(curr);
    if (key < curr->data) {
      this->comparisons += 1;
      if (!curr->left) {
        curr->left = newNode;
        break;
      }
      curr = curr->left;
    } else if (key > curr->data) {
      this->comparisons += 2;
      if (!curr->right) {
        curr->right = newNode;
        break;
      }
      curr = curr->right;
    } else {
      delete newNode;
      return;
    }
  }
  // última comparação do while que não foi contada
  this->comparisons += 1;

  while (!path.empty()) {
    this->comparisons += 1;
    curr = path.top();
    path.pop();

    Node<T> *parent = path.empty() ? nullptr : path.top();
    Node<T> *balanced = balanceNode(curr);

    connectToParent(parent, curr, balanced);
  }
  // última comparação do while que não foi contada
  this->comparisons += 1;
}

template <typename T> void AVLTree<T>::remove(T key) {
  this->comparisons += 1;
  if (!root)
    return;

  std::stack<Node<T> *> path;
  Node<T> *curr = root;

  while (curr && curr->data != key) {
    this->comparisons += 2;
    path.push(curr);
    if (key < curr->data)
      curr = curr->left;
    else
      curr = curr->right;
  }
  // última comparação do while que não foi contada
  this->comparisons += 3;

  if (!curr)
    return;

  Node<T> *target = curr;
  Node<T> *child = nullptr;

  if (!target->left || !target->right) {
    child = target->left ? target->left : target->right;
    Node<T> *parent = path.empty() ? nullptr : path.top();
    connectToParent(parent, target, child);
    delete target;
  } else {
    path.push(target);
    Node<T> *successor = target->right;

    while (successor->left) {
      this->comparisons += 1;
      path.push(successor);
      successor = successor->left;
    }
    // última comparação do while que não foi contada
    this->comparisons += 2;

    target->data = successor->data;

    Node<T> *succParent = path.top();
    if (succParent == target) {
      succParent->right = successor->right;
    } else {
      succParent->left = successor->right;
    }

    delete successor;
  }

  while (!path.empty()) {
    this->comparisons += 1;
    curr = path.top();
    path.pop();

    Node<T> *parent = path.empty() ? nullptr : path.top();
    Node<T> *balanced = balanceNode(curr);

    connectToParent(parent, curr, balanced);
  }
  // última comparação do while que não foi contada
  this->comparisons += 1;
}

template <typename T> AVLTree<T> *AVLTree<T>::search(T key) {
  Node<T> *curr = root;
  while (curr) {
    if (key == curr->data)
      return curr;
    else if (key < curr->data)
      curr = curr->left;
    else
      curr = curr->right;
  }
  return nullptr;
}

template <typename T> void AVLTree<T>::print() {
  if (!root)
    return;
  std::stack<Node<T> *> s;
  Node<T> *curr = root;

  while (curr || !s.empty()) {
    while (curr) {
      s.push(curr);
      curr = curr->left;
    }
    curr = s.top();
    s.pop();
    std::cout << curr->data << " ";
    curr = curr->right;
  }
  std::cout << std::endl;
}

// int main() {
//   std::cout << "--- AVL de Inteiros ---" << std::endl;
//   AVLTree<int> intTree;
//   intTree.insert(15);
//   intTree.insert(10);
//   intTree.insert(20);
//   intTree.insert(5);
//   intTree.print();
//
//   std::cout << "\n--- AVL de Strings ---" << std::endl;
//   AVLTree<std::string> stringTree;
//   stringTree.insert("C++");
//   stringTree.insert("Java");
//   stringTree.insert("Python");
//   stringTree.insert("Ada");
//
//   stringTree.print();
//
//   std::cout << "Buscar 'Java': " << (stringTree.search("Java") ? "Sim" :
//   "Não")
//             << std::endl;
//
//   return 0;
// }
