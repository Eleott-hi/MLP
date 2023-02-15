#ifndef MLP_SRC_MODEL_NEURAL_NETWORK_H_
#define MLP_SRC_MODEL_NEURAL_NETWORK_H_

#include <memory>

#include "letter.h"
#include "metrics.h"
#include "signal_handler.h"
#include "virtual_mlp.h"

enum class Strategy { kMatrix, kGraph };

class NeuralNetwork {
 public:
  ~NeuralNetwork() = default;
  NeuralNetwork(NeuralNetwork &&) = default;
  NeuralNetwork(const NeuralNetwork &) = delete;
  NeuralNetwork &operator=(NeuralNetwork &&) = delete;
  NeuralNetwork &operator=(const NeuralNetwork &) = delete;

  NeuralNetwork(Strategy strategy, const Config &config);

  void CrossValidation(std::list<Letter> train_data,
                       const std::list<Letter> &test_data,  //
                       int folds, int epoch, double learning_rate,
                       const bool &exit);

  void Train(const std::list<Letter> &train_data,
             const std::list<Letter> &test_data,  //
             int epoch, double learning_rate, const bool &exit);

  void Test(const std::list<Letter> &test_data, const bool &exit);

  Strategy GetStrategy() { return strategy_; }
  double GetAccuracy() { return metrics_.accuracy; }
  void SetAccuracy(double a) { metrics_.accuracy = a; }
  void GuessPicturedLetter(const std::vector<double> &data);
  const Config &GetConfig() const { return mlp_->GetConfig(); }

 private:
  Metrics metrics_;
  Strategy strategy_;
  std::unique_ptr<VirtualMLP> mlp_;
  SignalHandler &signal_handler_ = SignalHandler::GetInstance();

  void GuessLetters(const std::list<Letter> &letters, const bool &exit,
                    double lr = 0);
};

#endif  // MLP_SRC_MODEL_NEURAL_NETWORK_H_
