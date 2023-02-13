#include "layer.h"

namespace s21 {

Layer::Layer(int neurons)
    : neurons_(neurons), bias_(std::make_unique<Neuron>()) {}

void Layer::GetWeights(Config &config) {
  for (auto &neuron : neurons_) neuron.GetWieght(config);
}

void Layer::GetBiases(Config &config) { config.AddBias(bias_->GetValue()); }

void Layer::SetNextLayer(Layer *l) {
  next_layer_ = l;
  if (next_layer_)
    for (auto &neuron : neurons_)
      for (auto &next_neuron : next_layer_->neurons_)
        neuron.AddNextNeuron(&next_neuron);
}

void Layer::SetInputData(const std::vector<double> &data) {
  for (size_t i = 0; i < Size(); i++) neurons_[i].SetValue(data[i]);
}

void Layer::SetOutputError(size_t right_index) {
  for (size_t i = 0; i < Size(); i++)
    neurons_[i].SetError(neurons_[i].GetValue() - (double)(i == right_index));
}

void Layer::Clear() {
  for (auto &neuron : neurons_) neuron.SetValue(0.0);
}

void Layer::FeedForward() {
  for (auto &neuron : neurons_) neuron.FeedForward();
}

void Layer::Activate() {
  for (auto &neuron : neurons_) {
    neuron.SetValue(neuron.GetValue() + bias_->GetValue());
    neuron.Activate();
  }
}

void Layer::UpdateBios(double lr) {
  double bias_error = 0.0;
  std::for_each(neurons_.begin(), neurons_.end(),
                [&](auto &neuron) { bias_error += neuron.GetError(); });
  bias_->SetValue(bias_->GetValue() - lr * bias_error);
}

std::vector<double> Layer::GetOutput() {
  std::vector<double> output;
  std::for_each(neurons_.begin(), neurons_.end(),
                [&](auto &neuron) { output.push_back(neuron.GetValue()); });
  return output;
}

void Layer::BackPropagation() {
  for (auto &neuron : neurons_) neuron.CalcError();
}

void Layer::UpdateWeights(double lr) {
  for (auto &neuron : neurons_) neuron.UpdateWeights(lr);
}

void Layer::SetWeights(const std::vector<double> &vec, int &i) {
  for (auto &neuron : neurons_) neuron.SetWeights(vec, i);
}

void Layer::SetBios(const std::vector<double> &vec, int &i) {
  bias_->SetValue(vec[i++]);
}

}  // namespace s21
