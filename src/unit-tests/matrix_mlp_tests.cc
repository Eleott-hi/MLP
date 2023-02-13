#include <gtest/gtest.h>

#include "config.h"
#include "matrix-mlp/matrix_mlp.h"
#include "utils.h"

TEST(matrix_perceptron_suite, forward_propagation) {
  // configure
  std::vector<double> answer{0.99, 0.99};
  s21::Config config = LoadWeights("weights.txt");

  // test forward propagation
  s21::MatrixMLP matrix_mlp(config);
  matrix_mlp.SetInputLayer({1.0, 0.5, 0.25, 0.125});
  matrix_mlp.FeedForward();
  const auto& output = matrix_mlp.GetOutput();
  for (size_t i = 0; i < output.size(); ++i) {
    ASSERT_NEAR(output[i], answer[i], 0.01);
  }
}

TEST(matrix_perceptron_suite, back_propagation) {
  // configure
  s21::Config config = LoadWeights("weights.txt");
  s21::MatrixMLP matrix_mlp(config);

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
