#ifndef MLP_SRC_MAIN_MODEL_CONFIG_H_
#define MLP_SRC_MAIN_MODEL_CONFIG_H_

#include <random>
#include <sstream>
#include <vector>

namespace s21 {

class Config {
 public:
  explicit Config(size_t layers);
  explicit Config(const std ::vector<size_t> &layers);
  Config() = default;
  ~Config() = default;
  Config(const Config &) = default;
  Config(Config &&) = default;
  Config &operator=(const Config &) = default;
  Config &operator=(Config &&) = default;

  void AddLayer(int layer) { layers_.push_back(layer); }
  void AddWeight(double weight) { weights_.push_back(weight); }
  void AddBias(double bias) { biases_.push_back(bias); }

  const std::vector<size_t> &GetLayers() const { return layers_; }
  const std::vector<double> &GetWeights() const { return weights_; }
  const std::vector<double> &GetBiases() const { return biases_; }

  void Clear() { layers_.clear(), weights_.clear(), biases_.clear(); }
  size_t WeightsSize() const { return weights_.size() + biases_.size(); }
  size_t LayersSize() const { return layers_.size(); }
  size_t HiddenLayersSize() const;
  size_t HiddenNeurons() const { return HiddenLayersSize() ? layers_[1] : 0; }
  std::string ToString() const;

 private:
  std::vector<size_t> layers_;
  std::vector<double> weights_;
  std::vector<double> biases_;

  void SetRandWeights();
};

}  // namespace s21

#endif  // MLP_SRC_MAIN_MODEL_CONFIG_H_
