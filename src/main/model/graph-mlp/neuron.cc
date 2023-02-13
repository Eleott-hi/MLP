#include "neuron.h"

namespace s21 {

void Neuron::FeedForward() {
  for (size_t i = 0; i < next_neurons_.size(); i++)
    next_neurons_[i]->value_ += value_ * weights_[i];
}

void Neuron::CalcError() {
  double error = 0;
  for (size_t i = 0; i < next_neurons_.size(); i++)
    error += weights_[i] * next_neurons_[i]->error_;
  SetError(error);
}

void Neuron::UpdateWeights(double lr) {
  for (size_t i = 0; i < next_neurons_.size(); i++)
    weights_[i] -= lr * value_ * next_neurons_[i]->error_;
}

void Neuron::SetWeights(const std::vector<double>& vec, int& i) {
  for (size_t j = 0; j < next_neurons_.size(); j++)
    weights_.push_back(vec[i++]);
}

void Neuron::GetWieght(Config& config) {
  std::for_each(weights_.begin(), weights_.end(),
                [&](const auto& wieght) { config.AddWeight(wieght); });
}

}  // namespace s21
