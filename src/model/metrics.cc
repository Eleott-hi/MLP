#include "metrics.h"

#include <iomanip>
#include <sstream>

void Metrics::Clear() {
  error = all_answers = right_answers = 0;
  all_guess = all_letter = true_guess = std::vector<int>(26, 0);
}

void Metrics::UpdateInfo(int right, int guess) {
  all_answers++;
  all_letter[right]++;
  all_guess[guess]++;
  if (guess == right) {
    true_guess[guess]++;
    right_answers++;
  }
}

void Metrics::Calc(double size) {
  error /= size;
  GetAccuracy(), GetPrecision(), GetRecall(), GetFMeasure();
}

double Metrics::GetAccuracy() {
  return (accuracy = (double)right_answers / (double)all_answers * 100);
}

double Metrics::GetPrecision() {
  double res = 0.0;
  int count = 0;
  for (int i = 0; i < 26; i++) {
    if (all_guess[i]) {
      count++;
      res += (double)true_guess[i] / (double)all_guess[i];
    }
  }
  return (precision = res / count * 100);
}

double Metrics::GetRecall() {
  double res = 0.0;
  int count = 0;
  for (int i = 0; i < 26; i++) {
    if (all_letter[i]) {
      count++;
      res += (double)true_guess[i] / (double)all_letter[i];
    }
  }
  return (recall = res / count * 100);
}

double Metrics::GetFMeasure() {
  return (f_measure = 2.0 / (1.0 / GetPrecision() + 1.0 / GetRecall()));
}

std::string Metrics::ToString() const {
  std::stringstream ss;
  ss << std::fixed << std::setprecision(2) << "\tAll: " << all_answers
     << "\n\tRight: " << right_answers << "\n\tAverage accuracy: " << accuracy
     << " %"
     << "\n\tPrecision: " << precision << " %"
     << "\n\tRecall: " << recall << " %"
     << "\n\tF-measure: " << f_measure << " %";
  return ss.str();
}
