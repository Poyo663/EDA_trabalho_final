#include <string>

#include "word.h"

Word::Word(std::string word) {
  this->word = word;
  for (size_t i = 0; i < this->word.length(); i++) {
    if (this->word[i] >= 'A' && this->word[i] <= 'Z')
      this->word[i] += 32;
  }
}

std::string Word::toString() { return this->word; }
