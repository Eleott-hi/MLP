#include "matrix_mlp.h"

#include <algorithm>

namespace s21 {

MatrixMLP::MatrixMLP() : afunc_(nullptr), dafunc_(nullptr) {}

MatrixMLP::MatrixMLP(const Config &config) : MatrixMLP() {
  std::copy(config.GetLayers().begin(), config.GetLayers().end(),
            std::back_inserter(layer_config_));
  const auto &weights = config.GetWeights();
  const auto &biases = config.GetBiases();

  // memory allocation for neurons and errors of neurons
  for (const auto &i : layer_config_) {
    layers_errors_.emplace_back(i, 1);
    layer_derivative_.emplace_back(i, 1);
    layers_neurons_.emplace_back(i, 1);
  }

  // Memory allocation for weights and transpose weights
  for (size_t i = 0; i < layer_config_.size() - 1; ++i) {
    weights_.emplace_back(layer_config_[i + 1], layer_config_[i]);
    transpose_weights_.emplace_back(layer_config_[i], layer_config_[i + 1]);
  }

  // Copying weight values from a config
  int offset = 0;
  for (auto &item : weights_) {
    for (size_t i = 0; i < item.get_rows(); ++i) {
      for (size_t j = 0; j < item.get_cols(); ++j) {
        item[i][j] = weights[offset + j * item.get_rows() + i];
      }
    }
    offset += item.get_rows() * item.get_cols();
  }

  // Biases
  for (size_t i = 0; i < layer_config_.size() - 1; ++i) {
    biases_.emplace_back(layer_config_[i + 1], 1);
    for (size_t j = 0; j < biases_[i].get_rows(); ++j) {
      // bias layer[i]
      // matrix[j][0]
      biases_[i][j][0] = biases[i];
    }
  }

  SetActivationFunc([](Matrix *m) -> void {
    for (size_t i = 0; i < m->get_rows(); ++i) {
      for (size_t j = 0; j < m->get_cols(); ++j) {
        (*m)[i][j] = 1.0 / (1.0 + std::pow(std::exp(1.0), -(*m)[i][j]));
      }
    }
  });

  SetDerivativeActivationFunc([](const Matrix &from, Matrix *to) -> void {
    for (size_t i = 0; i < from.get_rows(); ++i) {
      for (size_t j = 0; j < from.get_cols(); ++j) {
        (*to)[i][j] = from[i][j] * (1.0 - from[i][j]);
      }
    }
  });
}

void MatrixMLP::SetActivationFunc(ActivationFunction f) { afunc_ = f; }

void MatrixMLP::SetDerivativeActivationFunc(DerivativeActivationFunction f) {
  dafunc_ = f;
}

// Virtual MLP interface methods
bool MatrixMLP::SetInputLayer(const std::vector<double> &data) {
  if (data.size() != layers_neurons_.front().get_rows()) {
    return false;
  }

  for (size_t i = 0; i < data.size(); ++i) {
    layers_neurons_.front()[i][0] = data[i];
  }
  return true;
}

const std::vector<double> &MatrixMLP::GetOutput() { return output_layer_; }

const Config &MatrixMLP::GetConfig() {
  config_.Clear();

  for (const auto &size : layer_config_) {
    config_.AddLayer(size);
  }

  for (const auto &weight_vector : weights_) {
    for (size_t i = 0; i < weight_vector.get_cols(); ++i) {
      for (size_t j = 0; j < weight_vector.get_rows(); ++j) {
        config_.AddWeight(weight_vector[j][i]);
      }
    }
  }

  for (const auto &bias_vector : biases_) {
    config_.AddBias(bias_vector[0][0]);
  }

  return config_;
}

int MatrixMLP::FeedForward() {
  // Process input data
  for (size_t i = 1; i < layer_config_.size(); ++i) {
    Matrix::Mull(weights_[i - 1], layers_neurons_[i - 1], &layers_neurons_[i]);
    Matrix::Add(layers_neurons_[i], biases_[i - 1], &layers_neurons_[i]);
    afunc_(&layers_neurons_[i]);
  }

  // Copy output vector column layer to vector
  output_layer_.clear();
  auto &output_layer = layers_neurons_.back();
  for (size_t i = 0; i < output_layer.get_rows(); ++i) {
    output_layer_.push_back(output_layer[i][0]);
  }

  return static_cast<int>(std::distance(
      output_layer_.begin(),
      std::max_element(output_layer_.begin(), output_layer_.end())));
}

bool MatrixMLP::BackPropagation(size_t right_index, double learning_rate) {
  if (!(right_index < layers_neurons_.back().get_rows())) {
    return false;
  }

  for (size_t i = 0; i < layers_neurons_.size(); ++i) {
    dafunc_(layers_neurons_[i], &layer_derivative_[i]);
  }

  for (size_t i = 0; i < layers_neurons_.back().get_rows(); ++i) {
    layers_errors_.back()[i][0] =
        (double)(i == right_index) - layers_neurons_.back()[i][0];
  }
  Matrix::HadamardMull(layers_errors_.back(), layer_derivative_.back(),
                       &layers_errors_.back());

  for (int k = weights_.size() - 1; k > 0; --k) {
    Matrix::Transpose(weights_[k], &transpose_weights_[k]);
    Matrix::Mull(transpose_weights_[k], layers_errors_[k + 1],
                 &layers_errors_[k]);
    for (size_t j = 0; j < layers_neurons_[k].get_rows(); ++j) {
      layers_errors_[k][j][0] *= layer_derivative_[k][j][0];
    }
  }

  UpdateWeights(learning_rate);
  return true;
}

void MatrixMLP::UpdateWeights(double lr) {
  for (size_t i = 0; i < weights_.size(); ++i) {
    for (size_t j = 0; j < layers_neurons_[i + 1].get_rows(); ++j) {
      for (size_t k = 0; k < layers_neurons_[i].get_rows(); ++k) {
        weights_[i][j][k] +=
            layers_errors_[i + 1][j][0] * layers_neurons_[i][k][0] * lr;
      }
    }
  }

  for (size_t i = 0; i < biases_.size(); ++i) {
    for (size_t j = 0; j < layers_neurons_[i + 1].get_rows(); ++j) {
      biases_[i][j][0] += layers_errors_[i + 1][j][0] * lr;
    }
  }
}

double MatrixMLP::GetError(size_t right_answer) {
  double error = 0.0;
  for (size_t i = 0; i < output_layer_.size(); i++) {
    error += pow((double)(i == right_answer) - output_layer_[i], 2);
  }
  return error;
}

}  // namespace s21
