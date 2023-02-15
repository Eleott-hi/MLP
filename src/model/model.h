#ifndef MLP_SRC_MODEL_MODEL_H_
#define MLP_SRC_MODEL_MODEL_H_

#include <QImage>

#include "neural_network.h"
#include "signal_handler.h"

class Model {
 public:
  Model();
  ~Model() = default;
  Model(Model &&) = default;
  Model(const Model &) = delete;
  Model &operator=(Model &&) = delete;
  Model &operator=(const Model &) = delete;

  void Test(double percentage);
  void ChangeMLP(Strategy strategy);
  void GuessLetter(const QImage &image);
  void LoadWeights(const std::string &filename);
  void SaveWeights(const std::string &filename);
  void LoadTestData(const std::string &filename);
  void LoadTrainData(const std::string &filename);
  void Train(size_t hidden_layers, size_t epoch, double learning_rate);
  void CrossValidation(size_t hidden_layers, size_t epoch, size_t folds,
                       double learning_rate);

 private:
  std::unique_ptr<NeuralNetwork> network_;
  std::list<Letter> train_data_, test_data_;
  std::string train_filename_, test_filename_;
  SignalHandler &signal_handler_ = SignalHandler::GetInstance();

  void LoadData(const std::string &filename, std::list<Letter> &save_letters,
                std::string &save_filename);
};

#endif  // MLP_SRC_MODEL_MODEL_H_
