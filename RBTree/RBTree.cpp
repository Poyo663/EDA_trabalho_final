#include "RBTree.h"
#include <vector>

template <typename T> Color RedBlackTree<T>::getColor(RBNode<T> *n) {
  return n ? n->color : BLACK;
}

template <typename T> void RedBlackTree<T>::setColor(RBNode<T> *n, Color c) {
  if (n) {
    this->color_changes++;
    n->color = c;
  }
}

template <typename T> RBNode<T> *RedBlackTree<T>::rotateLeft(RBNode<T> *x) {
  this->rotations++;
  RBNode<T> *y = x->right;
  x->right = y->left;
  y->left = x;
  return y;
}

template <typename T> RBNode<T> *RedBlackTree<T>::rotateRight(RBNode<T> *y) {
  this->rotations++;
  RBNode<T> *x = y->left;
  y->left = x->right;
  x->right = y;
  return x;
}

template <typename T>
void RedBlackTree<T>::connectToParent(RBNode<T> *parent, RBNode<T> *oldChild,
                                      RBNode<T> *newChild) {
  if (!parent) {
    root = newChild;
  } else if (parent->left == oldChild) {
    parent->left = newChild;
  } else {
    parent->right = newChild;
  }
}

template <typename T> RedBlackTree<T>::~RedBlackTree() {
  if (!root)
    return;
  std::stack<RBNode<T> *> s;
  s.push(root);
  while (!s.empty()) {
    RBNode<T> *curr = s.top();
    s.pop();
    if (curr->left)
      s.push(curr->left);
    if (curr->right)
      s.push(curr->right);
    delete curr;
  }
}

template <typename T> void RedBlackTree<T>::insert(T key) {
  RBNode<T> *newRBNode = new RBNode<T>(key);
  if (!root) {
    root = newRBNode;
    root->color = BLACK;
    return;
  }

  std::stack<RBNode<T> *> path;
  RBNode<T> *curr = root;

  while (curr) {
    path.push(curr);
    this->comparisons += 1;
    if (key < curr->data) {
      if (!curr->left) {
        curr->left = newRBNode;
        break;
      }
      curr = curr->left;
    } else if (key > curr->data) {
      this->comparisons += 2;
      if (!curr->right) {
        curr->right = newRBNode;
        break;
      }
      curr = curr->right;
    } else {
      this->comparisons += 2;
      curr->n++;
      delete newRBNode;
      return;
    }
  }

  RBNode<T> *z = newRBNode;

  while (!path.empty() && getColor(path.top()) == RED) {
    RBNode<T> *p = path.top();
    path.pop();

    if (path.empty())
      break;
    RBNode<T> *g = path.top();
    path.pop();

    RBNode<T> *gg = path.empty() ? nullptr : path.top();

    if (p == g->left) {
      RBNode<T> *u = g->right;

      if (getColor(u) == RED) {
        setColor(p, BLACK);
        setColor(u, BLACK);
        setColor(g, RED);
        z = g;
        if (gg)
          path.push(gg);
      } else {
        if (z == p->right) {
          p = rotateLeft(p);
          g->left = p;
        }
        setColor(p, BLACK);
        setColor(g, RED);
        RBNode<T> *newSubRoot = rotateRight(g);
        connectToParent(gg, g, newSubRoot);
        break;
      }
    } else {
      RBNode<T> *u = g->left;

      if (getColor(u) == RED) {
        setColor(p, BLACK);
        setColor(u, BLACK);
        setColor(g, RED);
        z = g;
        if (gg)
          path.push(gg);
      } else {
        if (z == p->left) {
          p = rotateRight(p);
          g->right = p;
        }
        setColor(p, BLACK);
        setColor(g, RED);
        RBNode<T> *newSubRoot = rotateLeft(g);
        connectToParent(gg, g, newSubRoot);
        break;
      }
    }
  }
  root->color = BLACK;
}

template <typename T> RBNode<T>* RedBlackTree<T>::search(T key) {
  RBNode<T> *curr = root;
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

template <typename T> void RedBlackTree<T>::print() {
  if (!root)
    return;
  std::stack<RBNode<T> *> s;
  RBNode<T> *curr = root;

  while (curr || !s.empty()) {
    while (curr) {
      s.push(curr);
      curr = curr->left;
    }
    curr = s.top();
    s.pop();
    std::cout << curr->data << ": " << curr->n << std::endl;
    curr = curr->right;
  }
  std::cout << std::endl;
}

template <typename T> std::vector<T> RedBlackTree<T>::getKeys() {
  std::stack<RBNode<T> *> s;
  std::vector<T> v;
  if (!root)
    return v;

  RBNode<T> *curr = root;

  while (curr || !s.empty()) {
    while (curr) {
      s.push(curr);
      curr = curr->left;
    }
    curr = s.top();
    s.pop();
    v.push_back(curr->data);
    curr = curr->right;
  }
  return v;
}

// int main() {
//   std::cout << "--- RBT de Inteiros ---" << std::endl;
//   RedBlackTree<int> rbtInt;
//
//   int arr[] = {10, 20, 30, 15, 25, 5};
//   for (int x : arr) {
//     rbtInt.insert(x);
//   }
//
//   std::cout << "Árvore em ordem [Valor(Cor: V=Vermelho, P=Preto)]:"
//             << std::endl;
//   rbtInt.print();
//
//   std::cout << "\n--- RBT de Strings ---" << std::endl;
//   RedBlackTree<std::string> rbtString;
//   rbtString.insert("Estruturas");
//   rbtString.insert("De");
//   rbtString.insert("Dados");
//   rbtString.insert("Avancadas");
//
//   rbtString.print();
//
//   std::cout << "Buscar 'Dados': "
//             << (rbtString.search("Dados") ? "Encontrado" : "Não encontrado")
//             << std::endl;
//
//   return 0;
// }
