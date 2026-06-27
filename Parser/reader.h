#ifndef READER
#define READER

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "word.h"

std::string lerArquivoParaString(const std::string &caminhoDoArquivo);

std::vector<Word *> stringParaArray(std::string &s);

std::vector<Word *> extrairPalavras(const std::string &texto);

#endif
