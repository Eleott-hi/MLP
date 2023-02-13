#include "config.h"

namespace s21 {

Config::Config(const std::vector<size_t> &layers) : layers_(layers) {
  SetRandWeights();
}

Config::Config(size_t layers) : layers_({784, 26}) {
  std::vector<size_t> tmp(layers > 2 ? layers - 2 : 0, 100);
  layers_.insert(layers_.begin() + 1, tmp.begin(), tmp.end());
  SetRandWeights();
}

size_t Config::HiddenLayersSize() const {
  return LayersSize() > 2 ? LayersSize() - 2 : 0;
}

std::string Config::ToString() const {
  std::stringstream ss;
  for (auto &i : layers_) ss << i << " ";
  ss << "\n";
  for (auto &i : weights_) ss << i << " ";
  ss << "\n";
  for (auto &i : biases_) ss << i << " ";
  return ss.str();
}

void Config::SetRandWeights() {
  std::random_device rd;
  std::uniform_real_distribution<double> uniform_dist(-1, 1);
  for (size_t i = 0; i < layers_.size() - 1; i++) {
    biases_.push_back(uniform_dist(rd));
    for (size_t j = 0; j < layers_[i] * layers_[i + 1]; j++)
      weights_.push_back(uniform_dist(rd));
  }
}

}  // namespace s21
