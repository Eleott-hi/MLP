#ifndef MLP_SRC_MODEL_CONFIG_H_
#define MLP_SRC_MODEL_CONFIG_H_

#include <vector>
#include <string>

class Config {
 public:
  Config() = default;
  ~Config() = default;
  Config(Config &&) = default;
  Config(const Config &) = default;
  Config &operator=(Config &&) = default;
  Config &operator=(const Config &) = default;

  explicit Config(size_t layers);
  explicit Config(const std ::vector<size_t> &layers);

  void AddLayer(int layer) { layers_.push_back(layer); }
  void AddBias(double bias) { biases_.push_back(bias); }
  void AddWeight(double weight) { weights_.push_back(weight); }

  const std::vector<size_t> &GetLayers() const { return layers_; }
  const std::vector<double> &GetBiases() const { return biases_; }
  const std::vector<double> &GetWeights() const { return weights_; }

  std::string ToString() const;
  size_t HiddenLayersSize() const;
  size_t LayersSize() const { return layers_.size(); }
  void Clear() { layers_.clear(), weights_.clear(), biases_.clear(); }
  size_t WeightsSize() const { return weights_.size() + biases_.size(); }
  size_t HiddenNeurons() const { return HiddenLayersSize() ? layers_[1] : 0; }

 private:
  std::vector<size_t> layers_;
  std::vector<double> weights_, biases_;

  void SetRandWeights();
};

#endif  // MLP_SRC_MODEL_CONFIG_H_
