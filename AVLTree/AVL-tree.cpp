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
  Node<T> *x = y->left;
  Node<T> *T2 = x->right;

  x->right = y;
  y->left = T2;

  updateHeight(y);
  updateHeight(x);

  return x;
}

template <typename T> Node<T> *AVLTree<T>::rotateLeft(Node<T> *x) {
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

  // Caso Esquerda-Esquerda (LL)
  if (balance > 1 && getBalance(subRoot->left) >= 0) {
    return rotateRight(subRoot);
  }

  // Caso Esquerda-Direita (LR)
  if (balance > 1 && getBalance(subRoot->left) < 0) {
    subRoot->left = rotateLeft(subRoot->left);
    return rotateRight(subRoot);
  }

  // Caso Direita-Direita (RR)
  if (balance < -1 && getBalance(subRoot->right) <= 0) {
    return rotateLeft(subRoot);
  }

  // Caso Direita-Esquerda (RL)
  if (balance < -1 && getBalance(subRoot->right) > 0) {
    subRoot->right = rotateRight(subRoot->right);
    return rotateLeft(subRoot);
  }

  return subRoot; // Não precisou de rotação
}

template <typename T>
void AVLTree<T>::connectToParent(Node<T> *parent, Node<T> *oldChild,
                                 Node<T> *newChild) {
  if (!parent) {
    root = newChild;
  } else if (parent->left == oldChild) {
    parent->left = newChild;
  } else {
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
  if (!root) {
    root = newNode;
    return;
  }

  std::stack<Node<T> *> path;
  Node<T> *curr = root;

  // Fase 1: Descida (BST padrão) guardando o caminho
  while (curr) {
    path.push(curr);
    if (key < curr->data) {
      if (!curr->left) {
        curr->left = newNode;
        break;
      }
      curr = curr->left;
    } else if (key > curr->data) {
      if (!curr->right) {
        curr->right = newNode;
        break;
      }
      curr = curr->right;
    } else {
      delete newNode; // Chave duplicada não é inserida
      return;
    }
  }

  // Fase 2: Subida desempilhando e balanceando
  while (!path.empty()) {
    curr = path.top();
    path.pop();

    Node<T> *parent = path.empty() ? nullptr : path.top();
    Node<T> *balanced = balanceNode(curr);

    connectToParent(parent, curr, balanced);
  }
}

template <typename T> void AVLTree<T>::remove(T key) {
  if (!root)
    return;

  std::stack<Node<T> *> path;
  Node<T> *curr = root;

  // Fase 1: Busca o nó a ser removido
  while (curr && curr->data != key) {
    path.push(curr);
    if (key < curr->data)
      curr = curr->left;
    else
      curr = curr->right;
  }

  if (!curr)
    return; // Chave não encontrada

  // Fase 2: Substituição do nó (Casos de exclusão da BST)
  Node<T> *target = curr;
  Node<T> *child = nullptr;

  if (!target->left || !target->right) {
    // Caso 0 ou 1 filho
    child = target->left ? target->left : target->right;
    Node<T> *parent = path.empty() ? nullptr : path.top();
    connectToParent(parent, target, child);
    delete target;
  } else {
    // Caso 2 filhos: Encontrar o sucessor em ordem
    path.push(target);
    Node<T> *successor = target->right;

    while (successor->left) {
      path.push(successor);
      successor = successor->left;
    }

    // Copia o dado do sucessor para o nó alvo
    target->data = successor->data;

    // O pai do sucessor agora aponta para o filho da direita do sucessor
    Node<T> *succParent = path.top();
    if (succParent == target) {
      succParent->right = successor->right;
    } else {
      succParent->left = successor->right;
    }

    delete successor;
  }

  // Fase 3: Subida recalculando a altura e rebalanceando
  while (!path.empty()) {
    curr = path.top();
    path.pop();

    Node<T> *parent = path.empty() ? nullptr : path.top();
    Node<T> *balanced = balanceNode(curr);

    connectToParent(parent, curr, balanced);
  }
}

template <typename T> bool AVLTree<T>::search(T key) {
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
//   // Exemplo 1: Árvore AVL de Inteiros
//   std::cout << "--- AVL de Inteiros ---" << std::endl;
//   AVLTree<int> intTree;
//   intTree.insert(15);
//   intTree.insert(10);
//   intTree.insert(20);
//   intTree.insert(5);
//   intTree.print();
//
//   // Exemplo 2: Árvore AVL de Strings (Ordenação Alfabética)
//   std::cout << "\n--- AVL de Strings ---" << std::endl;
//   AVLTree<std::string> stringTree;
//   stringTree.insert("C++");
//   stringTree.insert("Java");
//   stringTree.insert("Python");
//   stringTree.insert("Ada");
//
//   stringTree.print(); // Deve imprimir em ordem alfabética: Ada C++ Java Python
//
//   std::cout << "Buscar 'Java': " << (stringTree.search("Java") ? "Sim" : "Não")
//             << std::endl;
//
//   return 0;
// }
