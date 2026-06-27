#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "AVLTree/AVL-tree.h"
#include "Hashs/HashChaining.hpp"
#include "Hashs/HashOpenAddress.hpp"
#include "Parser/reader.h"
#include "Parser/word.h"
#include "RBTree/RBTree.h"

void print_usage(const char *programName) {
  std::cout << "Uso:    \"" << programName << " (modo) arquivo.txt\""
            << std::endl;
  std::cout << "Ou:     \"" << programName << " (modo) \"texto\"\""
            << std::endl;
  std::cout << "Modos: " << std::endl;
  std::cout << "       " << "use_avl   --Árvore AVL" << std::endl;
  std::cout << "       " << "use_rbt   --Árvore Rubro Negro" << std::endl;
  std::cout << "       " << "use_open  --Tabela Hash com Endereçamento Aberto"
            << std::endl;
  std::cout << "       " << "use_chain --Tabela Hash com Encadeamento Externo"
            << std::endl;
  std::cout << "       " << "use_all   --Todas as Estruturas de Dados"
            << std::endl;
}

void use_avl(std::vector<Word *> &words, AVLTree<std::string> &tree) {
  for (int i = 0; i < words.size(); i++) {
    tree.insert(words.at(i)->toString());
  }
  tree.print();
  std::ofstream file("arvore_avl.csv");
  std::vector<std::string> v = tree.getKeys();
  if (file.is_open()) {
    for (int i = 0; i < v.size(); i++) {
      file << v.at(i) << "," << tree.search(v.at(i))->n << "\n";
    }
  } else {
    std::cerr << "Erro ao tentar abrir ou criar o arquivo.\n";
  }
  std::cout << "Árvore AVL: " << std::endl;
  std::cout << "            Comparações feitas na montagem: "
            << tree.comparisons << std::endl;
  std::cout << "            Rotações feitas na montagem:    " << tree.rotations
            << std::endl;
}

void use_rbt(std::vector<Word *> &words, RedBlackTree<std::string> &tree) {
  for (int i = 0; i < words.size(); i++) {
    tree.insert(words.at(i)->toString());
  }
  tree.print();
  std::ofstream file("arvore_rubro_negro.csv");
  std::vector<std::string> v = tree.getKeys();
  if (file.is_open()) {
    for (int i = 0; i < v.size(); i++) {
      file << v.at(i) << "," << tree.search(v.at(i))->n << "\n";
    }
  } else {
    std::cerr << "Erro ao tentar abrir ou criar o arquivo.\n";
  }
  std::cout << "Árvore Rubro Negro: " << std::endl;
  std::cout << "            Comparações feitas na montagem: "
            << tree.comparisons << std::endl;
  std::cout << "            Rotações feitas na montagem:    " << tree.rotations
            << std::endl;
}

void use_open(std::vector<Word *> &words,
              HashOpenAddress<std::string, int> &hash) {
  for (int i = 0; i < words.size(); i++) {
    std::string w = words.at(i)->toString();
    if (hash.contains(w))
      hash.modify(w, hash.get(w) + 1);
    else
      hash.add(w);
  }
  hash.print();
  std::ofstream file("enderecamento_aberto.csv");
  std::vector<std::string> v = hash.getKeys();
  if (file.is_open()) {
    for (int i = 0; i < v.size(); i++) {
      file << v.at(i) << "," << hash.get(v.at(i)) << "\n";
    }
  } else {
    std::cerr << "Erro ao tentar abrir ou criar o arquivo.\n";
  }
  std::cout << "Hash com Endereçamento Aberto: " << std::endl;
  std::cout << "            Comparações feitas na montagem: "
            << hash.getKeyComparisons() << std::endl;
}

void use_chain(std::vector<Word *> &words,
               HashChaining<std::string, int> *hash) {
  for (int i = 0; i < words.size(); i++) {
    std::string w = words.at(i)->toString();
    if (hash->contains(w))
      hash->modify(w, hash->get(w) + 1);
    else
      hash->add(w);
  }
  hash->print();
  std::ofstream file("encadeamento_externo.csv");
  std::vector<std::string> v = hash->getKeys();
  if (file.is_open()) {
    for (int i = 0; i < v.size(); i++) {
      file << v.at(i) << "," << hash->get(v.at(i)) << "\n";
    }
  } else {
    std::cerr << "Erro ao tentar abrir ou criar o arquivo.\n";
  }
  std::cout << "Hash com Encadeamento Externo: " << std::endl;
  std::cout << "            Comparações feitas na montagem: "
            << hash->getKeyComparisons() << std::endl;
}

int main(int argc, const char *argv[]) {
  if (argc != 3) {
    print_usage(argv[0]);
    return 1;
  }

  std::string_view mode = argv[1];
  std::string path{argv[2]};

  if (mode == "--help") {
    print_usage(argv[0]);
    return 0;
  }

  std::string text = lerArquivoParaString(path);
  if (text == "")
    return 1;
  std::vector<Word *> palavras = extrairPalavras(text);

  if (mode == "use_avl") {
    AVLTree<std::string> t;
    auto start = std::chrono::high_resolution_clock::now();
    use_avl(palavras, t);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Operação realizada em " << duration.count() << " μs"
              << std::endl;
  } else if (mode == "use_rbt") {
    RedBlackTree<std::string> t;
    auto start = std::chrono::high_resolution_clock::now();
    use_rbt(palavras, t);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Operação realizada em " << duration.count() << " μs"
              << std::endl;
  } else if (mode == "use_open") {
    HashOpenAddress<std::string, int> h;
    auto start = std::chrono::high_resolution_clock::now();
    use_open(palavras, h);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Operação realizada em " << duration.count() << " μs"
              << std::endl;
  } else if (mode == "use_chain") {
    HashChaining<std::string, int> *h = new HashChaining<std::string, int>(10);
    auto start = std::chrono::high_resolution_clock::now();
    use_chain(palavras, h);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Operação realizada em " << duration.count() << " μs"
              << std::endl;
  } else if (mode == "use_all") {
    AVLTree<std::string> t;
    RedBlackTree<std::string> t2;
    HashOpenAddress<std::string, int> h;
    HashChaining<std::string, int> *h2 = new HashChaining<std::string, int>(10);
    auto start = std::chrono::high_resolution_clock::now();
    auto new_start = std::chrono::high_resolution_clock::now();

    use_avl(palavras, t);
    auto avl_time = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now() - new_start);
    new_start = std::chrono::high_resolution_clock::now();

    use_rbt(palavras, t2);
    auto rbt_time = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now() - new_start);
    new_start = std::chrono::high_resolution_clock::now();

    use_open(palavras, h);
    auto open_time = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now() - new_start);
    new_start = std::chrono::high_resolution_clock::now();

    use_chain(palavras, h2);
    auto chain_time = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now() - new_start);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Operação inteira realizada em " << duration.count() << " μs"
              << std::endl;
    std::cout << "Operações singulares:" << std::endl;
    std::cout << "                     Árvore Avl:                    "
              << avl_time.count() << "μs" << std::endl;
    std::cout << "                     Árvore Rubro Negro:            "
              << rbt_time.count() << "μs" << std::endl;
    std::cout << "                     Hash com Endereçamento Aberto: "
              << open_time.count() << "μs" << std::endl;
    std::cout << "                     Hash com Encadeamento Externo: "
              << chain_time.count() << "μs" << std::endl;
  } else {
    print_usage(argv[0]);
    return 1;
  }

  return 0;
}
