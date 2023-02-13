#ifndef MLP_SRC_MAIN_MODEL_METRICS_H_
#define MLP_SRC_MAIN_MODEL_METRICS_H_

#include <iomanip>
#include <sstream>
#include <vector>

namespace s21 {

struct Metrics {
  double error = 0;
  double accuracy = 0;
  double precision = 0;
  double recall = 0;
  double f_measure = 0;

  int right_answers;
  int all_answers;
  std::vector<int> true_guess;
  std::vector<int> all_letter;
  std::vector<int> all_guess;

  Metrics() { Clear(); }

  void Clear();
  void AddError(double e) { error += e; };
  void UpdateInfo(int right, int guess);
  void Calc(double size);
  double GetAccuracy();
  double GetPrecision();
  double GetRecall();
  double GetFMeasure();
  std::string ToString() const;
};

}  // namespace s21

#endif  // MLP_SRC_MAIN_MODEL_METRICS_H_
