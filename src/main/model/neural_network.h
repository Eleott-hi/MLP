#ifndef MLP_SRC_MAIN_MODEL_NEURAL_NETWORK_H_
#define MLP_SRC_MAIN_MODEL_NEURAL_NETWORK_H_

#include <memory>

#include "letter.h"
#include "metrics.h"
#include "signal_handler.h"
#include "virtual_mlp.h"

namespace s21 {

enum class Strategy { kMatrix, kGraph };

class NeuralNetwork {
 public:
  NeuralNetwork(Strategy strategy, const Config &config);
  ~NeuralNetwork() = default;
  NeuralNetwork(const NeuralNetwork &) = delete;
  NeuralNetwork(NeuralNetwork &&) = default;
  NeuralNetwork &operator=(const NeuralNetwork &) = delete;
  NeuralNetwork &operator=(NeuralNetwork &&) = delete;

  void CrossValidation(std::list<Letter> train_data,
                       const std::list<Letter> &test_data, int folds, int epoch,
                       double learning_rate, const bool &exit);
  void Train(const std::list<Letter> &train_data,
             const std::list<Letter> &test_data, int epoch,
             double learning_rate, const bool &exit);
  void Test(const std::list<Letter> &test_data, const bool &exit);
  void GuessPicturedLetter(const std::vector<double> &data);

  Strategy GetStrategy() { return strategy_; }
  const Config &GetConfig() const { return mlp_->GetConfig(); }
  void SetAccuracy(double a) { metrics_.accuracy = a; }
  double GetAccuracy() { return metrics_.accuracy; }

 private:
  std::unique_ptr<VirtualMLP> mlp_;
  SignalHandler &signal_handler_ = SignalHandler::GetInstance();
  Strategy strategy_;
  Metrics metrics_;

  void GuessLetters(const std::list<Letter> &letters, const bool &exit,
                    double lr = 0);
};

}  // namespace s21

#endif  // MLP_SRC_MAIN_MODEL_NEURAL_NETWORK_H_
