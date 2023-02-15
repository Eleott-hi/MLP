#include "letter.h"

Letter::Letter(const std::string &str) {
  pixels_.reserve(784);
  ParseLetter(str);
}

void Letter::ParseLetter(const std::string &str) {
  size_t i = 0;
  letter_ = std::stoi(str, &i);
  i++;
  for (size_t tmp = 0; i < str.size();) {
    pixels_.push_back(std::stod(str.substr(i), &tmp) / 255.0);
    i += tmp + 1;
  }
}
