#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "reader.h"

std::string lerArquivoParaString(const std::string &caminhoDoArquivo) {
  std::ifstream arquivo(caminhoDoArquivo);
  if (!arquivo.is_open()) {
    std::cerr << "Erro ao abrir o arquivo: " << caminhoDoArquivo << std::endl;
    return "";
  }
  std::stringstream buffer;
  buffer << arquivo.rdbuf();
  return buffer.str();
}

std::vector<Word *> extrairPalavras(const std::string &texto) {
  std::vector<Word *> palavras;
  std::stringstream ss(texto);
  std::string palavra;

  while (ss >> palavra) {
    std::string palavraLimpa = "";
    for (char c : palavra) {
      if (!std::ispunct(static_cast<unsigned char>(c))) {
        palavraLimpa += c;
      }
    }
    if (!palavraLimpa.empty()) {
      palavras.push_back(new Word(palavraLimpa));
    }
  }
  return palavras;
}

std::vector<Word *> extrairPalavras2(const std::string &texto) {
  size_t length = texto.length();
  std::vector<Word *> palavras;
  std::string palavraAtual = "";

  for (size_t i = 0; i < length; i++) {
    if (texto[i] == '-' &&
        !std::ispunct(
            static_cast<unsigned char>(texto[std::min(i + 1, length)])) &&
        !std::isspace(
            static_cast<unsigned char>(texto[std::min(i + 1, length)])) &&
        !std::ispunct(
            static_cast<unsigned char>(texto[std::max(((int)i) - 1, 0)])) &&
        !std::isspace(
            static_cast<unsigned char>(texto[std::max(((int)i) - 1, 0)]))) {
      palavraAtual += texto[i];
    } else if (!std::ispunct(static_cast<unsigned char>(texto[i])) &&
               !std::isspace(static_cast<unsigned char>(texto[i]))) {
      palavraAtual += texto[i]; // Vai montando a palavra letra por letra
    } else {
      // Se caiu aqui, encontramos um separador (espaço, ponto, vírgula, etc.)
      // Se a palavra atual não estiver vazia, adicionamos ao vetor
      if (!palavraAtual.empty()) {
        palavras.push_back(new Word(palavraAtual));
        palavraAtual = ""; // Limpa para começar a próxima palavra
      }
    }
  }

  // Segurança: se o texto não terminar com pontuação/espaço,
  // a última palavra ainda estará na variável 'palavraAtual'
  if (!palavraAtual.empty()) {
    palavras.push_back(new Word(palavraAtual));
  }

  return palavras;
}

int main() {
  std::string path = "exemplo.txt";
  std::string s = lerArquivoParaString(path);
  std::string s2 = "mostra-lo";
  std::vector<Word *> v = extrairPalavras2(s2);
  for (int i = 0; i < v.size(); i++) {
    std::cout << v.at(i)->toString() << std::endl;
  }
  std::cout << v.size() << std::endl;
  return 0;
}
