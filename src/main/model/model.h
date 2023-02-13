#ifndef MLP_SRC_MAIN_MODEL_MODEL_H_
#define MLP_SRC_MAIN_MODEL_MODEL_H_

#include <fstream>
#include <sstream>
#include <thread>

#include "neural_network.h"
#include "qimage.h"
#include "signal_handler.h"

namespace s21 {

class Model {
 public:
  Model();
  ~Model() = default;
  Model(const Model &) = delete;
  Model(Model &&) = default;
  Model &operator=(const Model &) = delete;
  Model &operator=(Model &&) = delete;

  void ChangeMLP(Strategy strategy);
  void LoadTrainData(const std::string &filename);
  void LoadTestData(const std::string &filename);
  void CrossValidation(size_t hidden_layers, size_t epoch, size_t folds,
                       double learning_rate);
  void Train(size_t hidden_layers, size_t epoch, double learning_rate);
  void Test(double percentage);
  void LoadWeights(const std::string &filename);
  void SaveWeights(const std::string &filename);
  void GuessLetter(const QImage &image);

 private:
  std::unique_ptr<NeuralNetwork> network_;
  SignalHandler &signal_handler_ = SignalHandler::GetInstance();
  std::string train_filename_;
  std::string test_filename_;
  std::list<Letter> train_data_;
  std::list<Letter> test_data_;

  void LoadData(const std::string &filename, std::list<Letter> &save_letters,
                std::string &save_filename);
};

}  // namespace s21

#endif  // MLP_SRC_MAIN_MODEL_MODEL_H_
