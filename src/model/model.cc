#include "model.h"

#include <fstream>
#include <sstream>
#include <thread>

Model::Model()
    : network_(std::make_unique<NeuralNetwork>(Strategy::kMatrix, Config{4})) {}

void Model::LoadWeights(const std::string &filename) {
  std::ifstream file(filename);

  if (!file.is_open()) {
    emit signal_handler_.ShowError("Can not open file " + filename);
    return;
  }

  Config config;
  double accuracy = -100;
  std::string line;

  for (int i = 0; std::getline(file, line); i++) {
    std::stringstream ss(line);
    double num = 0;

    while (ss >> num) {
      if (i == 0) {
        config.AddLayer(num);

      } else if (i == 1) {
        config.AddWeight(num);

      } else if (i == 2) {
        config.AddBias(num);

      } else {
        accuracy = num;
      }
    }
  }

  if (config.WeightsSize() != Config(config.GetLayers()).WeightsSize()) {
    emit signal_handler_.ShowError("Configuration file is corrupted: " +
                                   filename);
    return;
  }

  network_ = std::make_unique<NeuralNetwork>(network_->GetStrategy(), config);
  network_->SetAccuracy(accuracy);

  emit signal_handler_.WeightsLoaded(filename, config.HiddenLayersSize(),
                                     config.HiddenNeurons(), accuracy);
}

void Model::SaveWeights(const std::string &filename) {
  std::ofstream file(filename);

  if (!file.is_open()) {
    emit signal_handler_.ShowError("Can not open file " + filename);
    return;
  }

  const Config &config = network_->GetConfig();
  file << config.ToString() << "\n" << network_->GetAccuracy();

  emit signal_handler_.ShowMessage("Configuration is saved to file: " +
                                   filename);
  emit signal_handler_.WeightsLoaded(filename, config.HiddenLayersSize(),
                                     config.HiddenNeurons(),
                                     network_->GetAccuracy());
}

void Model::LoadTrainData(const std::string &filename) {
  std::thread([&, filename]() {
    LoadData(filename, train_data_, train_filename_);
    emit signal_handler_.TrainDataLoaded(filename, train_data_.size());
  }).detach();
}

void Model::LoadTestData(const std::string &filename) {
  std::thread([&, filename]() {
    LoadData(filename, test_data_, test_filename_);
    emit signal_handler_.TestDataLoaded(filename, test_data_.size());
  }).detach();
}

void Model::GuessLetter(const QImage &image) {
  static std::vector<double> data(784);

  for (int i = 0; i < 28; i++)
    for (int j = 0; j < 28; j++)
      data.at(i * 28 + j) = image.pixelColor(i, j).blackF();

  network_->GuessPicturedLetter(data);
}

void Model::ChangeMLP(Strategy strategy) {
  double accuracy = network_->GetAccuracy();
  network_ = std::make_unique<NeuralNetwork>(strategy, network_->GetConfig());
  network_->SetAccuracy(accuracy);
}

void Model::CrossValidation(size_t hidden_layers, size_t epoch, size_t folds,
                            double learning_rate) {
  static bool training = false, exit = false;

  if (!training) {
    exit = false, training = true;

    // ========================= Thread ===========================
    std::thread([&, hidden_layers, folds, epoch, learning_rate]() {
      if (train_data_.empty() || test_data_.empty()) {
        emit signal_handler_.ShowError(
            (train_data_.empty() ? "Training" : "Test") +
            std::string(" data is empty. Please, load data from file"));

      } else {
        auto tmp = std::make_unique<NeuralNetwork>(network_->GetStrategy(),
                                                   Config{hidden_layers + 2});
        tmp->CrossValidation(train_data_, test_data_, folds, epoch,
                             learning_rate, exit);

        if (!exit) {
          network_ = std::move(tmp);
          const Config &config = network_->GetConfig();

          emit signal_handler_.WeightsLoaded(
              "Unsaved", config.HiddenLayersSize(), config.HiddenNeurons(),
              network_->GetAccuracy());
        }
      }

      training = false;
      emit signal_handler_.LearningCompleted();
    }).detach();
    // ========================= Thread ===========================
  } else {
    exit = true;
  }
}

void Model::Train(size_t hidden_layers, size_t epoch, double learning_rate) {
  static bool training = false, exit = false;
  if (!training) {
    exit = false, training = true;

    // ========================= Thread ===========================
    std::thread([&, hidden_layers, epoch, learning_rate]() {
      if (train_data_.empty() || test_data_.empty()) {
        emit signal_handler_.ShowError(
            (std::string)(train_data_.empty() ? "Training" : "Test") +
            " data is empty. Please, load data from file");

      } else {
        auto tmp = std::make_unique<NeuralNetwork>(network_->GetStrategy(),
                                                   Config{hidden_layers + 2});
        tmp->Train(train_data_, test_data_, epoch, learning_rate, exit);

        if (!exit) {
          network_ = std::move(tmp);
          const Config &config = network_->GetConfig();

          emit signal_handler_.WeightsLoaded(
              "Unsaved", config.HiddenLayersSize(), config.HiddenNeurons(),
              network_->GetAccuracy());
        }
      }

      training = false;
      emit signal_handler_.LearningCompleted();
    }).detach();
    // ========================= Thread ===========================
  } else {
    exit = true;
  }
}

void Model::Test(double percentage) {
  static bool testing = false, exit = false;

  if (!testing) {
    exit = false, testing = true;
    // ========================= Thread ===========================
    std::thread([&, percentage]() {
      if (test_data_.empty()) {
        emit signal_handler_.ShowError(
            "Test data is empty. Please, load data from file");
        emit signal_handler_.TestingCompleted();

      } else {
        auto tmp = std::make_unique<NeuralNetwork>(network_->GetStrategy(),
                                                   network_->GetConfig());
        std::list<Letter> test_data_part;
        test_data_part.insert(
            test_data_part.begin(), test_data_.begin(),
            std::next(test_data_.begin(), test_data_.size() * percentage));
        tmp->Test(test_data_part, exit);
      }

      testing = false;
    }).detach();
    // ========================= Thread ===========================
  } else {
    exit = true;
  }
}

void Model::LoadData(const std::string &filename,
                     std::list<Letter> &save_letters,
                     std::string &save_filename) {
  if (filename == save_filename) {
    emit signal_handler_.ShowMessage("Data already loaded");
    return;
  }

  std::ifstream file(filename);
  if (!file.is_open()) {
    emit signal_handler_.ShowError("Can not open file " + filename);
    return;
  }

  std::list<Letter> letters;
  for (std::string line; std::getline(file, line);)
    letters.push_back(Letter(line));

  save_letters = std::move(letters);
  save_filename = filename;
}
