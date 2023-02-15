#ifndef MLP_SRC_MAIN_MODEL_MATRIX_MLP_MATRIX_MLP_H_
#define MLP_SRC_MAIN_MODEL_MATRIX_MLP_MATRIX_MLP_H_

#include "matrix.h"
#include "virtual_mlp.h"

class MatrixMLP : public VirtualMLP {
  using ActivationFunction = void (*)(Matrix *matrix);
  using DerivativeActivationFunction = void (*)(const Matrix &from, Matrix *to);

 public:
  explicit MatrixMLP(const Config &config);
  MatrixMLP();
  ~MatrixMLP() = default;
  MatrixMLP(const MatrixMLP &) = default;
  MatrixMLP(MatrixMLP &&) = default;
  MatrixMLP &operator=(const MatrixMLP &) = default;
  MatrixMLP &operator=(MatrixMLP &&) = default;

  void SetActivationFunc(ActivationFunction f);
  void SetDerivativeActivationFunc(DerivativeActivationFunction f);
  void UpdateWeights(double lr);

  bool SetInputLayer(const std::vector<double> &data) override;
  const std::vector<double> &GetOutput() override;
  const Config &GetConfig() override;
  int FeedForward() override;
  bool BackPropagation(size_t right_index, double learning_rate) override;
  double GetError(size_t right_answer) override;

 private:
  std::vector<int> layer_config_;
  std::vector<Matrix> weights_;            // Matrix of weights
  std::vector<Matrix> transpose_weights_;  // Vector column with a neurons
  std::vector<Matrix> layers_neurons_;     // Vector column with a neurons
  std::vector<Matrix> layers_errors_;      // Vector column with a delta errors
  std::vector<Matrix> biases_;             // Vector column with a biases
  std::vector<double> output_layer_;       // Only for interface compatibility
  std::vector<Matrix> layer_derivative_;   // Derivative values

  ActivationFunction afunc_;
  DerivativeActivationFunction dafunc_;
};

#endif  // MLP_SRC_MAIN_MODEL_MATRIX_MLP_MATRIX_MLP_H_
