#ifndef MLP_SRC_VIEW_MAIN_FORM_H_
#define MLP_SRC_VIEW_MAIN_FORM_H_

#include <QMainWindow>
#include <QProgressBar>

#include "controller.h"
#include "signal_handler.h"

namespace Ui {
class MainForm;
}

class MainForm : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainForm(Controller* controller);
  ~MainForm();
  MainForm(const MainForm&) = delete;
  MainForm(MainForm&&) = delete;
  MainForm& operator=(const MainForm&) = delete;
  MainForm& operator=(MainForm&&) = delete;

 private slots:
  void on_perceptronTypeComboBox_currentIndexChanged(int index);
  void on_scribbleArea_updated(QImage image);
  void on_learningImportButton_clicked();
  void on_weigthsImportButton_clicked();
  void on_weightsExportButton_clicked();
  void on_testingImportButton_clicked();
  void on_learningStartButton_clicked();
  void on_testingStartButton_clicked();
  void on_imageImportButton_clicked();
  void on_scribbleArea_cleared();

  // files
  void OnWeightsLoaded(const std::string& filename, int layers, int neurons,
                       double precision);
  void OnTrainDataLoaded(const std::string& filename, double size);
  void OnTestDataLoaded(const std::string& filename, double size);

  // learning
  void OnFoldPassed();
  void OnLearningCompleted();
  void OnEpochPassed(int epoch, double error);
  void OnEpochProgressChanged(int percentage);

  // testing
  void OnTestingProgressChanged(int percentage);
  void OnTestingCompleted(double accuracy, double precision, double recall,
                          double f_measure, double time);

  // recognition
  void OnRecognitionCompleted(char letter, const std::vector<double>& output);

  // logs
  void OnError(const std::string&);
  void OnMessage(const std::string&);

 private:
  Ui::MainForm* ui_;
  Controller* controller_;
  SignalHandler& signal_handler_ = SignalHandler::GetInstance();
  QVector<QProgressBar*> letter_pct_bars_;

  void AddGraph();
  void SetupPlot();
  void ConnectSignals();
  void SetMainTweaksEnbaled(bool state);
  void SetLearningTweaksEnabled(bool state);
};

#endif  // MLP_SRC_VIEW_MAIN_FORM_H_
