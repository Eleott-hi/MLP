#ifndef MLP_SRC_MODEL_LETTER_H_
#define MLP_SRC_MODEL_LETTER_H_

#include <string>
#include <vector>

class Letter {
 public:
  ~Letter() = default;
  Letter(Letter&&) = default;
  Letter(const Letter&) = default;
  Letter& operator=(Letter&&) = default;
  Letter& operator=(const Letter&) = default;

  explicit Letter(const std::string& str);

  int GetLetter() const { return letter_ - 1; }
  const std::vector<double>& GetData() const { return pixels_; }

 private:
  int letter_;
  std::vector<double> pixels_;

  void ParseLetter(const std::string& str);
};

#endif  // MLP_SRC_MODEL_LETTER_H_
