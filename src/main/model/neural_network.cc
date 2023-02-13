#include "neural_network.h"

#include "graph-mlp/graph_mlp.h"
#include "matrix-mlp/matrix_mlp.h"

namespace s21 {
NeuralNetwork::NeuralNetwork(Strategy strategy, const Config &config)
    : mlp_(strategy == Strategy::kGraph
               ? static_cast<std::unique_ptr<VirtualMLP>>(
                     std::make_unique<GraphMLP>(config))
               : static_cast<std::unique_ptr<VirtualMLP>>(
                     std::make_unique<MatrixMLP>(config))),
      strategy_(strategy) {}

void NeuralNetwork::CrossValidation(std::list<Letter> train_data,
                                    const std::list<Letter> &test_data,
                                    int folds, int epoch, double learning_rate,
                                    const bool &exit) {
  auto layers = GetConfig().LayersSize();
  metrics_.error = std::numeric_limits<double>::infinity();

  for (int k = 1; k <= folds && !exit; k++) {
    auto tmp = std::make_unique<NeuralNetwork>(strategy_, Config{layers});

    std::list<Letter> test_data_part;
    test_data_part.splice(
        test_data_part.begin(), train_data, train_data.begin(),
        std::next(train_data.begin(), train_data.size() / folds));

    for (int i = 1; i <= epoch && !exit; i++) {
      tmp->GuessLetters(train_data, exit, learning_rate / i);
      tmp->GuessLetters(test_data_part, exit);
      if (!exit) {
        emit signal_handler_.EpochPassed(i, tmp->metrics_.error);
      }
    }

    tmp->GuessLetters(test_data, exit);

    if (!exit) {
      if (metrics_.error > tmp->metrics_.error) {
        metrics_ = tmp->metrics_;
        this->mlp_ = std::move(tmp->mlp_);
      }
      train_data.splice(train_data.end(), test_data_part,
                        test_data_part.begin(), test_data_part.end());
    }

    emit signal_handler_.FoldPassed();
  }
}

void NeuralNetwork::Train(const std::list<Letter> &train_data,
                          const std::list<Letter> &test_data, int epoch,
                          double learning_rate, const bool &exit) {
  for (int i = 1; i <= epoch && !exit; i++) {
    GuessLetters(train_data, exit, learning_rate / i);
    GuessLetters(test_data, exit);

    emit signal_handler_.EpochPassed(i, metrics_.error);
  }
}

void NeuralNetwork::Test(const std::list<Letter> &test_data, const bool &exit) {
  auto start = std::chrono::steady_clock::now();

  GuessLetters(test_data, exit);
  if (exit) return;

  auto end = std::chrono::steady_clock::now();
  size_t time =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
          .count();

  emit signal_handler_.TestingCompleted(metrics_.accuracy, metrics_.precision,
                                        metrics_.recall, metrics_.f_measure,
                                        time / 1000.0);
}

void NeuralNetwork::GuessPicturedLetter(const std::vector<double> &data) {
  if (mlp_->SetInputLayer(data)) {
    char letter = 'A' + mlp_->FeedForward();
    emit signal_handler_.RecognitionCompleted(letter, mlp_->GetOutput());
  }
}

void NeuralNetwork::GuessLetters(const std::list<Letter> &letters,
                                 const bool &exit, double lr) {
  Metrics tmp;
  for (auto i = letters.begin(); i != letters.end() && !exit; ++i) {
    if (mlp_->SetInputLayer(i->GetData())) {
      int guess = mlp_->FeedForward();
      int right = i->GetLetter();
      tmp.UpdateInfo(right, guess);
      int percentage = (float)tmp.all_answers / letters.size() * 100.0f;
      if (lr) {
        if (!mlp_->BackPropagation(right, lr))
          emit signal_handler_.ShowError(
              "Dismatch rightIndex and OutputLayer size");
        emit signal_handler_.EpochProgressChanged(percentage);
      } else {
        tmp.AddError(mlp_->GetError(right));
        emit signal_handler_.TestingProgressChanged(percentage);
      }
    }
    if (tmp.all_answers % 10000 == 0) {
      tmp.Calc(1);
    }
  }

  if (exit) return;
  metrics_ = tmp;
  metrics_.Calc(letters.size());
}

}  // namespace s21
