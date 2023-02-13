#ifndef MLP_SRC_MAIN_MODEL_GRAPH_MLP_GRAPH_MLP_H_
#define MLP_SRC_MAIN_MODEL_GRAPH_MLP_GRAPH_MLP_H_

#include <cmath>
#include <functional>

#include "layer.h"
#include "virtual_mlp.h"

namespace s21 {

class GraphMLP : public VirtualMLP {
 public:
  explicit GraphMLP(const Config &config) { Init(config); }
  ~GraphMLP() = default;
  GraphMLP(const GraphMLP &) = delete;
  GraphMLP(GraphMLP &&) = default;
  GraphMLP &operator=(const GraphMLP &) = delete;
  GraphMLP &operator=(GraphMLP &&) = default;

  int FeedForward() override;
  bool BackPropagation(size_t right_index, double learning_rate) override;
  bool SetInputLayer(const std::vector<double> &data) override;
  const std::vector<double> &GetOutput() override { return output_; }
  double GetError(size_t right_index) override;
  const Config &GetConfig() override;

 private:
  std::vector<Layer> layers_;
  std::vector<double> output_;

  Layer &GetInputLayer() { return layers_[0]; }
  Layer &GetOutputLayer() { return layers_[layers_.size() - 1]; }
  void ConnectLayers();
  void Init(const Config &config);
  void SetWeightsAndBiases(const Config &data);
};

}  // namespace s21

#endif  // MLP_SRC_MAIN_MODEL_GRAPH_MLP_GRAPH_MLP_H_
