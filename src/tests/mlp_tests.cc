#include <gtest/gtest.h>

#include <vector>

#include "graph-mlp/graph_mlp.h"
#include "letter.h"
#include "matrix-mlp/matrix_mlp.h"
#include "utils.h"

Config config_ = LoadWeights("../../weights/weights_2.txt");
Letter letter_(letter_a);

TEST(graph_mlp, config) {
  Config config = Config{4};
  GraphMLP mlp_(config);

  auto func = [&](Config config_1, Config config_2) {
    return config_1.GetBiases() == config_2.GetBiases() &&
           config_1.GetLayers() == config_2.GetLayers() &&
           config_1.GetWeights() == config_2.GetWeights();
  };

  ASSERT_TRUE(func(config, mlp_.GetConfig()));
}

TEST(graph_mlp, FeedForward) {
  GraphMLP mlp_(config_);
  char answer = -1;
  if (mlp_.SetInputLayer(letter_.GetData())) answer = mlp_.FeedForward();
  ASSERT_EQ(answer, letter_.GetLetter());
}

TEST(graph_mlp, output) {
  GraphMLP mlp_(config_);

  if (mlp_.SetInputLayer(letter_.GetData())) mlp_.FeedForward();

  auto output = mlp_.GetOutput();

  ASSERT_EQ(std::distance(output.begin(),
                          std::max_element(output.begin(), output.end())),
            letter_.GetLetter());
}

TEST(graph_mlp, error) {
  GraphMLP mlp_(config_);

  if (mlp_.SetInputLayer(letter_.GetData())) mlp_.FeedForward();
  EXPECT_NEAR(mlp_.GetError(letter_.GetLetter()), 0.140086, 1e-6);
}

TEST(graph_mlp, learning) {
  GraphMLP mlp_(config_);

  for (int i = 0; i < 2; i++) {
    if (mlp_.SetInputLayer(letter_.GetData())) mlp_.FeedForward();
    mlp_.BackPropagation(letter_.GetLetter(), 0.01);
  }

  EXPECT_NEAR(mlp_.GetError(letter_.GetLetter()), 0.132351, 1e-6);
}

TEST(matrix_perceptron_suite, forward_propagation) {
  // configure
  std::vector<double> answer{0.99, 0.99};
  Config config = LoadWeights("weights.txt");

  // test forward propagation
  MatrixMLP matrix_mlp(config);
  matrix_mlp.SetInputLayer({1.0, 0.5, 0.25, 0.125});
  matrix_mlp.FeedForward();
  const auto& output = matrix_mlp.GetOutput();
  for (size_t i = 0; i < output.size(); ++i) {
    ASSERT_NEAR(output[i], answer[i], 0.01);
  }
}

TEST(matrix_perceptron_suite, back_propagation) {
  // configure
  Config config = LoadWeights("weights.txt");
  MatrixMLP matrix_mlp(config);

  // train network how "and" element
  double lr = 1.0;
  for (int j = 0; j < 10000; ++j) {
    lr -= 1.0 / 10000.0;
    matrix_mlp.SetInputLayer({1.0, 1.0});
    matrix_mlp.FeedForward();
    matrix_mlp.BackPropagation(1, lr);

    matrix_mlp.SetInputLayer({1.0, 0.0});
    matrix_mlp.FeedForward();
    matrix_mlp.BackPropagation(0, lr);

    matrix_mlp.SetInputLayer({0.0, 1.0});
    matrix_mlp.FeedForward();
    matrix_mlp.BackPropagation(0, lr);

    matrix_mlp.SetInputLayer({0.0, 0.0});
    matrix_mlp.FeedForward();
    matrix_mlp.BackPropagation(0, lr);
  }

  // test network answers
  int network_answer = -1;
  double error = 0.0;
  std::vector<double> output;

  matrix_mlp.SetInputLayer({1.0, 1.0});
  network_answer = matrix_mlp.FeedForward();
  output = matrix_mlp.GetOutput();
  error = matrix_mlp.GetError(1);
  ASSERT_EQ(network_answer, true);
  ASSERT_LT(output[0], 0.1);
  ASSERT_GT(output[1], 0.9);
  ASSERT_LT(error, 0.01);

  matrix_mlp.SetInputLayer({0.0, 0.0});
  network_answer = matrix_mlp.FeedForward();
  output = matrix_mlp.GetOutput();
  error = matrix_mlp.GetError(0);
  ASSERT_EQ(network_answer, false);
  ASSERT_GT(output[0], 0.9);
  ASSERT_LT(output[1], 0.1);
  ASSERT_LT(error, 0.01);

  matrix_mlp.SetInputLayer({1.0, 0.0});
  network_answer = matrix_mlp.FeedForward();
  output = matrix_mlp.GetOutput();
  error = matrix_mlp.GetError(0);
  ASSERT_EQ(network_answer, false);
  ASSERT_GT(output[0], 0.9);
  ASSERT_LT(output[1], 0.1);
  ASSERT_LT(error, 0.01);

  matrix_mlp.SetInputLayer({0.0, 1.0});
  network_answer = matrix_mlp.FeedForward();
  output = matrix_mlp.GetOutput();
  error = matrix_mlp.GetError(0);
  ASSERT_EQ(network_answer, false);
  ASSERT_GT(output[0], 0.9);
  ASSERT_LT(output[1], 0.1);
  ASSERT_LT(error, 0.01);
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
