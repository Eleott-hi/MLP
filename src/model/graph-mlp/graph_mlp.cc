#include "graph_mlp.h"

void GraphMLP::Init(const Config &config) {
  layers_.reserve(config.LayersSize());
  for (size_t i = 0; i < config.LayersSize(); i++)
    layers_.push_back(Layer(config.GetLayers()[i]));
  ConnectLayers();
  SetWeightsAndBiases(config);
}

void GraphMLP::ConnectLayers() {
  for (size_t i = 1; i < layers_.size(); i++) {
    layers_[i].SetPrevLayer(&layers_[i - 1]);
    layers_[i - 1].SetNextLayer(&layers_[i]);
  }
}

void GraphMLP::SetWeightsAndBiases(const Config &data) {
  if (data.WeightsSize()) {
    int w = 0, b = 0;
    for (auto &layer : layers_) {
      if (!layer.IsOutputLayer()) layer.SetWeights(data.GetWeights(), w);
      if (!layer.IsInputLayer()) layer.SetBios(data.GetBiases(), b);
    }
  }
}

const Config &GraphMLP::GetConfig() {
  config_.Clear();
  for (auto &layer : layers_) config_.AddLayer(layer.Size());
  for (auto &layer : layers_)
    if (!layer.IsOutputLayer()) layer.GetWeights(config_);
  for (auto &layer : layers_)
    if (!layer.IsInputLayer()) layer.GetBiases(config_);
  return config_;
}

bool GraphMLP::SetInputLayer(const std::vector<double> &data) {
  if (GetInputLayer().Size() != data.size()) return false;

  GetInputLayer().SetInputData(data);
  return true;
}

int GraphMLP::FeedForward() {
  for (auto &layer : layers_)
    if (!layer.IsInputLayer()) layer.Clear();

  for (auto &layer : layers_) {
    if (!layer.IsInputLayer()) layer.Activate();
    if (!layer.IsOutputLayer()) layer.FeedForward();
  }

  output_ = GetOutputLayer().GetOutput();

  return std::distance(output_.begin(),
                       std::max_element(output_.begin(), output_.end()));
}

double GraphMLP::GetError(size_t right_index) {
  double error = 0.0;
  for (size_t i = 0; i < output_.size(); i++) {
    error += pow((double)(i == right_index) - output_[i], 2);
  }
  return error;
}

bool GraphMLP::BackPropagation(size_t right_index, double learning_rate) {
  if (!(right_index < GetOutputLayer().Size())) return false;

  GetOutputLayer().SetOutputError(right_index);
  for (int i = layers_.size() - 1; i > -1; i--) {
    if (!layers_[i].IsOutputLayer()) {
      if (!layers_[i].IsInputLayer()) layers_[i].BackPropagation();
      layers_[i].UpdateWeights(learning_rate);
    }
    if (!layers_[i].IsInputLayer()) layers_[i].UpdateBios(learning_rate);
  }

  return true;
}
