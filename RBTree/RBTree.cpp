#include "RBTree.h"

template <typename T> Color RedBlackTree<T>::getColor(Node<T> *n) {
  return n ? n->color : BLACK;
}

template <typename T> void RedBlackTree<T>::setColor(Node<T> *n, Color c) {
  if (n) {
    this->color_changes++;
    n->color = c;
  }
}

template <typename T> Node<T> *RedBlackTree<T>::rotateLeft(Node<T> *x) {
  this->rotations++;
  Node<T> *y = x->right;
  x->right = y->left;
  y->left = x;
  return y;
}

template <typename T> Node<T> *RedBlackTree<T>::rotateRight(Node<T> *y) {
  this->rotations++;
  Node<T> *x = y->left;
  y->left = x->right;
  x->right = y;
  return x;
}

template <typename T>
void RedBlackTree<T>::connectToParent(Node<T> *parent, Node<T> *oldChild,
                                      Node<T> *newChild) {
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

template <typename T> void RedBlackTree<T>::insert(T key) {
  Node<T> *newNode = new Node<T>(key);
  if (!root) {
    root = newNode;
    root->color = BLACK;
    return;
  }

  std::stack<Node<T> *> path;
  Node<T> *curr = root;

  while (curr) {
    path.push(curr);
    this->comparisons += 1;
    if (key < curr->data) {
      this->comparisons += 1;
      if (!curr->left) {
        curr->left = newNode;
        break;
      }
      curr = curr->left;
    } else if (key > curr->data) {
      this->comparisons += 3;
      if (!curr->right) {
        curr->right = newNode;
        break;
      }
      curr = curr->right;
    } else {
      this->comparisons += 2;
      delete newNode;
      return;
    }
  }

  Node<T> *z = newNode;

  while (!path.empty() && getColor(path.top()) == RED) {
    Node<T> *p = path.top();
    path.pop();

    if (path.empty())
      break;
    Node<T> *g = path.top();
    path.pop();

    Node<T> *gg = path.empty() ? nullptr : path.top();

    this->comparisons += 1;
    if (p == g->left) {
      Node<T> *u = g->right;

      this->comparisons += 1;
      if (getColor(u) == RED) {
        setColor(p, BLACK);
        setColor(u, BLACK);
        setColor(g, RED);
        z = g;
        if (gg)
          path.push(gg);
      } else {
        this->comparisons += 1;
        if (z == p->right) {
          p = rotateLeft(p);
          g->left = p;
        }
        setColor(p, BLACK);
        setColor(g, RED);
        Node<T> *newSubRoot = rotateRight(g);
        connectToParent(gg, g, newSubRoot);
        break;
      }
    } else {
      Node<T> *u = g->left;

      this->comparisons += 1;
      if (getColor(u) == RED) {
        setColor(p, BLACK);
        setColor(u, BLACK);
        setColor(g, RED);
        z = g;
        if (gg)
          path.push(gg);
      } else {
        this->comparisons += 1;
        if (z == p->left) {
          p = rotateRight(p);
          g->right = p;
        }
        setColor(p, BLACK);
        setColor(g, RED);
        Node<T> *newSubRoot = rotateLeft(g);
        connectToParent(gg, g, newSubRoot);
        break;
      }
    }
  }
  root->color = BLACK;
}

template <typename T> bool RedBlackTree<T>::search(T key) {
  Node<T> *curr = root;
  while (curr) {
    if (key == curr->data)
      return true;
    else if (key < curr->data)
      curr = curr->left;
    else
      curr = curr->right;
  }
  return false;
}

template <typename T> void RedBlackTree<T>::print() {
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
    std::cout << curr->data << "(" << (curr->color == RED ? "V" : "P") << ") ";
    curr = curr->right;
  }
  std::cout << std::endl;
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
