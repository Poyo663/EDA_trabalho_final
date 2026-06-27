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
      palavraAtual += texto[i];
    } else {
      if (!palavraAtual.empty()) {
        palavras.push_back(new Word(palavraAtual));
        palavraAtual = "";
      }
    }
  }
  if (!palavraAtual.empty()) {
    palavras.push_back(new Word(palavraAtual));
  }

  return palavras;
}
