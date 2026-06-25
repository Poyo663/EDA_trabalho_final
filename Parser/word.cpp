#include <string>

#include "word.h"

Word::Word(std::string word) { this->word = word; }

std::string Word::toString() { return this->word; }
