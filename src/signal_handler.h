#ifndef MLP_SRC_MAIN_SIGNAL_HANDLER_H_
#define MLP_SRC_MAIN_SIGNAL_HANDLER_H_

#include <QObject>

class SignalHandler : public QObject {
  Q_OBJECT

 public:
  static SignalHandler& GetInstance() {
    static SignalHandler instance;
    return instance;
  }

 signals:
  // files
  void WeightsLoaded(const std::string& filename, int layers, int neurons,
                     double precision);
  void TrainDataLoaded(const std::string& filename, double size);
  void TestDataLoaded(const std::string& fiename, double size);

  // learning
  void LearningCompleted();
  void EpochPassed(int epoch, double error);
  void EpochProgressChanged(int percentage);
  void FoldPassed();

  // testing
  void TestingProgressChanged(int percentage);
  void TestingCompleted(double accuracy = 0, double precision = 0,
                        double recall = 0, double f_measure = 0,
                        double time = 0);

  // recognition
  void RecognitionCompleted(char letter, const std::vector<double>& output);

  // logs
  void ShowMessage(const std::string&);
  void ShowError(const std::string&);

 protected:
  SignalHandler() = default;
  ~SignalHandler() = default;
  SignalHandler(const SignalHandler&) = delete;
  SignalHandler(SignalHandler&&) = delete;
  SignalHandler& operator=(const SignalHandler&) = delete;
  SignalHandler& operator=(SignalHandler&&) = delete;
};

#endif  // MLP_SRC_MAIN_SIGNAL_HANDLER_H_
