#ifndef MLP_SRC_MAIN_MODEL_GRAPH_MLP_LAYER_H_
#define MLP_SRC_MAIN_MODEL_GRAPH_MLP_LAYER_H_

#include <memory>

#include "neuron.h"

namespace s21 {

class Layer {
 public:
  explicit Layer(int neurons);
  ~Layer() = default;
  Layer(const Layer &) = default;
  Layer(Layer &&) = default;
  Layer &operator=(const Layer &) = default;
  Layer &operator=(Layer &&) = default;

  size_t Size() const { return neurons_.size(); }
  bool IsInputLayer() const { return prev_layer_ == nullptr; }
  bool IsOutputLayer() const { return next_layer_ == nullptr; }

  void SetNextLayer(Layer *l);
  void SetPrevLayer(Layer *l) { prev_layer_ = l; }
  void SetInputData(const std::vector<double> &data);
  void SetOutputError(size_t right_index);

  void Clear();
  void Activate();
  void FeedForward();
  void BackPropagation();
  void UpdateBios(double lr);
  void UpdateWeights(double lr);
  std::vector<double> GetOutput();

  void GetBiases(Config &config);
  void GetWeights(Config &config);
  void SetWeights(const std::vector<double> &vec, int &i);
  void SetBios(const std::vector<double> &vec, int &i);

 private:
  std::vector<Neuron> neurons_;
  std::shared_ptr<Neuron> bias_ = nullptr;
  Layer *next_layer_ = nullptr;
  Layer *prev_layer_ = nullptr;
};

}  // namespace s21

#endif  // MLP_SRC_MAIN_MODEL_GRAPH_MLP_LAYER_H_
