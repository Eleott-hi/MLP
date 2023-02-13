#ifndef MLP_SRC_MAIN_CONTROLLER_H_
#define MLP_SRC_MAIN_CONTROLLER_H_

#include "model.h"

namespace s21 {

class Controller {
 public:
  Controller(const Controller&) = delete;
  Controller& operator=(const Controller&) = delete;
  explicit Controller(Model* model) : model_(model) {}
  ~Controller() {}

  void ChangeMLP(int strategy) {
    model_->ChangeMLP(static_cast<Strategy>(strategy));
  }

  void LoadTrainData(const std::string& filename) {
    model_->LoadTrainData(filename);
  }

  void LoadTestData(const std::string& filename) {
    model_->LoadTestData(filename);
  }

  void CrossValidation(size_t hidden_layers, size_t epoch, size_t folds,
                       double learning_rate) {
    model_->CrossValidation(hidden_layers, epoch, folds, learning_rate);
  }

  void Train(size_t hidden_layers, size_t epoch, double learning_rate) {
    model_->Train(hidden_layers, epoch, learning_rate);
  }

  void TestMLP(double percentage) { model_->Test(percentage); }

  void LoadWeights(const std::string& filename) {
    model_->LoadWeights(filename);
  }

  void SaveWeights(const std::string& filename) {
    model_->SaveWeights(filename);
  }

  void GuessLetter(const QImage& image) { model_->GuessLetter(image); }

 private:
  Model* model_ = nullptr;
};

}  // namespace s21

#endif  // MLP_SRC_MAIN_CONTROLLER_H_
