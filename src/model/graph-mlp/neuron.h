#ifndef MLP_SRC_MODEL_GRAPH_MLP_NEURON_H_
#define MLP_SRC_MODEL_GRAPH_MLP_NEURON_H_

#include <algorithm>
#include <cmath>

#include "config.h"

class Neuron {
 public:
  void SetError(double e) { error_ = e * SigmoidDerivate(value_); }
  void SetValue(double v) { value_ = v; }
  double GetValue() const { return value_; }
  double GetError() const { return error_; }
  void SetWeights(const std::vector<double> &vec, int &i);
  void GetWieght(Config &config);

  void AddNextNeuron(Neuron *n) { next_neurons_.push_back(n); }
  void Activate() { value_ = Sigmoid(value_); }
  void UpdateWeights(double lr);
  void FeedForward();
  void CalcError();

 private:
  std::vector<Neuron *> next_neurons_;
  std::vector<double> weights_;
  double value_ = 0;
  double error_ = 0;

  static double Sigmoid(double x) { return 1. / (1. + pow(exp(1.0), -x)); }
  static double SigmoidDerivate(double x) { return x * (1 - x); }
};

#endif  // MLP_SRC_MODEL_GRAPH_MLP_NEURON_H_
