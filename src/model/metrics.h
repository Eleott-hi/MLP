#ifndef MLP_SRC_MODEL_METRICS_H_
#define MLP_SRC_MODEL_METRICS_H_

#include <vector>

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
  double GetRecall();
  double GetAccuracy();
  double GetFMeasure();
  double GetPrecision();
  void Calc(double size);
  std::string ToString() const;
  void UpdateInfo(int right, int guess);
  void AddError(double e) { error += e; };
};

#endif  // MLP_SRC_MODEL_METRICS_H_
