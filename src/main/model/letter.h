#ifndef MLP_SRC_MAIN_MODEL_LETTER_H_
#define MLP_SRC_MAIN_MODEL_LETTER_H_

#include <iostream>
#include <string>
#include <vector>

namespace s21 {

class Letter {
 public:
  explicit Letter(const std::string& str);
  ~Letter() = default;
  Letter(const Letter&) = default;
  Letter(Letter&&) = default;
  Letter& operator=(const Letter&) = default;
  Letter& operator=(Letter&&) = default;

  const std::vector<double>& GetData() const { return pixels_; }
  int GetLetter() const { return letter_ - 1; }

 private:
  int letter_;
  std::vector<double> pixels_;

  void ParseLetter(const std::string& str);
};

}  // namespace s21

#endif  // MLP_SRC_MAIN_MODEL_LETTER_H_
