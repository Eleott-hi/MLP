#include <gtest/gtest.h>

#include <vector>

#include "graph-mlp/graph_mlp.h"
#include "letter.h"
#include "utils.h"

s21::Config config_ = LoadWeights("../weights/weights_2.txt");
s21::Letter letter_(letter_a);

TEST(s21_graph_mlp, config) {
  s21::Config config = s21::Config{4};
  s21::GraphMLP mlp_(config);

  auto func = [&](s21::Config config_1, s21::Config config_2) {
    return config_1.GetBiases() == config_2.GetBiases() &&
           config_1.GetLayers() == config_2.GetLayers() &&
           config_1.GetWeights() == config_2.GetWeights();
  };

  ASSERT_TRUE(func(config, mlp_.GetConfig()));
}

TEST(s21_graph_mlp, FeedForward) {
  s21::GraphMLP mlp_(config_);
  char answer = -1;
  if (mlp_.SetInputLayer(letter_.GetData())) answer = mlp_.FeedForward();
  ASSERT_EQ(answer, letter_.GetLetter());
}

TEST(s21_graph_mlp, output) {
  s21::GraphMLP mlp_(config_);

  if (mlp_.SetInputLayer(letter_.GetData())) mlp_.FeedForward();

  auto output = mlp_.GetOutput();

  ASSERT_EQ(std::distance(output.begin(),
                          std::max_element(output.begin(), output.end())),
            letter_.GetLetter());
}

TEST(s21_graph_mlp, error) {
  s21::GraphMLP mlp_(config_);

  if (mlp_.SetInputLayer(letter_.GetData())) mlp_.FeedForward();
  EXPECT_NEAR(mlp_.GetError(letter_.GetLetter()), 0.140086, 1e-6);
}

TEST(s21_graph_mlp, learning) {
  s21::GraphMLP mlp_(config_);

  for (int i = 0; i < 2; i++) {
    if (mlp_.SetInputLayer(letter_.GetData())) mlp_.FeedForward();
    mlp_.BackPropagation(letter_.GetLetter(), 0.01);
  }

  EXPECT_NEAR(mlp_.GetError(letter_.GetLetter()), 0.132351, 1e-6);
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
