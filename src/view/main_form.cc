#include "main_form.h"

#include <QFileDialog>

#include "ui_main_form.h"

MainForm::MainForm(Controller* controller)
    : QMainWindow(nullptr), ui_(new Ui::MainForm), controller_(controller) {
  ui_->setupUi(this);
  SetupPlot();
  ConnectSignals();

  letter_pct_bars_ = {
      ui_->pctBar_A, ui_->pctBar_B, ui_->pctBar_C, ui_->pctBar_D, ui_->pctBar_E,
      ui_->pctBar_F, ui_->pctBar_G, ui_->pctBar_H, ui_->pctBar_I, ui_->pctBar_J,
      ui_->pctBar_K, ui_->pctBar_L, ui_->pctBar_M, ui_->pctBar_N, ui_->pctBar_O,
      ui_->pctBar_P, ui_->pctBar_Q, ui_->pctBar_R, ui_->pctBar_S, ui_->pctBar_T,
      ui_->pctBar_U, ui_->pctBar_V, ui_->pctBar_W, ui_->pctBar_X, ui_->pctBar_Y,
      ui_->pctBar_Z};
}

MainForm::~MainForm() { delete ui_; }

void MainForm::SetupPlot() {
  // epoch count
  ui_->plot->xAxis->setLabel("Epoch");
  ui_->plot->xAxis->setRangeLower(1);

  // error percentage
  ui_->plot->yAxis->setLabel("Error");
}

void MainForm::AddGraph() {
  QColor color(rand() % 240, rand() % 240, rand() % 240);

  ui_->plot->addGraph();
  ui_->plot->graph()->setPen(QPen(color));
  color.setAlpha(20);
  ui_->plot->graph()->setBrush(QBrush(color));
  ui_->plot->graph()->setLineStyle(QCPGraph::lsLine);
  ui_->plot->graph()->setScatterStyle(
      QCPScatterStyle(QCPScatterStyle::ssCircle, 5));

  if (ui_->learningTypeComboBox->currentText() == "Cross validation")
    ui_->plot->graph()->setName("Fold: " +
                                QString::number(ui_->plot->graphCount()));
}

void MainForm::ConnectSignals() {
  // files
  connect(&signal_handler_, &SignalHandler::WeightsLoaded, this,
          &MainForm::OnWeightsLoaded);
  connect(&signal_handler_, &SignalHandler::TrainDataLoaded, this,
          &MainForm::OnTrainDataLoaded);
  connect(&signal_handler_, &SignalHandler::TestDataLoaded, this,
          &MainForm::OnTestDataLoaded);

  // learning
  connect(&signal_handler_, &SignalHandler::LearningCompleted, this,
          &MainForm::OnLearningCompleted);
  connect(&signal_handler_, &SignalHandler::EpochPassed, this,
          &MainForm::OnEpochPassed);
  connect(&signal_handler_, &SignalHandler::EpochProgressChanged, this,
          &MainForm::OnEpochProgressChanged);
  connect(&signal_handler_, &SignalHandler::FoldPassed, this,
          &MainForm::OnFoldPassed);

  // testing
  connect(&signal_handler_, &SignalHandler::TestingProgressChanged, this,
          &MainForm::OnTestingProgressChanged);
  connect(&signal_handler_, &SignalHandler::TestingCompleted, this,
          &MainForm::OnTestingCompleted);

  // recognition
  connect(&signal_handler_, &SignalHandler::RecognitionCompleted, this,
          &MainForm::OnRecognitionCompleted);

  // logs
  connect(&signal_handler_, &SignalHandler::ShowMessage, this,
          &MainForm::OnMessage);
  connect(&signal_handler_, &SignalHandler::ShowError, this,
          &MainForm::OnError);
}

void MainForm::SetLearningTweaksEnabled(bool state) {
  ui_->learningTypeComboBox->setEnabled(state);
  ui_->learningRateSpinBox->setEnabled(state);
  ui_->hiddenLayersSpinBox->setEnabled(state);
  ui_->epochCountSpinBox->setEnabled(state);
  ui_->foldCountSpinBox->setEnabled(state);
}

void MainForm::SetMainTweaksEnbaled(bool state) {
  ui_->perceptronTypeComboBox->setEnabled(state);
  ui_->learningImportButton->setEnabled(state);
  ui_->weigthsImportButton->setEnabled(state);
  ui_->weightsExportButton->setEnabled(state);
  ui_->testingImportButton->setEnabled(state);
}

void MainForm::on_perceptronTypeComboBox_currentIndexChanged(int index) {
  controller_->ChangeMLP(index);
}

void MainForm::on_weigthsImportButton_clicked() {
  QString filename = QFileDialog::getOpenFileName(this, "Open File", "~/",
                                                  "Text files (*.txt)");

  if (!filename.isEmpty()) controller_->LoadWeights(filename.toStdString());
}

void MainForm::on_weightsExportButton_clicked() {
  QString filename = QFileDialog::getSaveFileName(this, "Save File", "~/",
                                                  "Text files (*.txt)");

  if (!filename.isEmpty()) controller_->SaveWeights(filename.toStdString());
}

void MainForm::on_learningImportButton_clicked() {
  QString filename = QFileDialog::getOpenFileName(this, "Open File", "~/",
                                                  "Text files (*.csv)");
  if (!filename.isEmpty()) {
    ui_->learningImportButton->setEnabled(false);
    controller_->LoadTrainData(filename.toStdString());
  }
}

void MainForm::on_testingImportButton_clicked() {
  QString filename = QFileDialog::getOpenFileName(this, "Open File", "~/",
                                                  "Text files (*.csv)");
  if (!filename.isEmpty()) {
    ui_->testingImportButton->setEnabled(false);
    controller_->LoadTestData(filename.toStdString());
  }
}

void MainForm::on_imageImportButton_clicked() {
  QString filename = QFileDialog::getOpenFileName(this, "Open File", "~/",
                                                  "Image files (*.bmp)");

  if (!filename.isEmpty()) ui_->scribbleArea->OpenImage(filename);
}

void MainForm::on_learningStartButton_clicked() {
  if (ui_->learningStartButton->text() == "Start") {
    ui_->learningStartButton->setText("Stop");
    ui_->learningStartButton->setStyleSheet(
        "QPushButton{background-color: rgb(244,113,116)}");

    // lock ui
    ui_->recognitionTab->setEnabled(false);
    ui_->testingTab->setEnabled(false);
    SetLearningTweaksEnabled(false);
    SetMainTweaksEnbaled(false);
    ui_->learningProgressBar_1->setEnabled(true);
    ui_->learningProgressBar_2->setEnabled(true);

    ui_->plot->clearGraphs();
    ui_->plot->replot();
    AddGraph();

  } else {
    OnLearningCompleted();
  }

  ui_->learningProgressBar_1->setValue(0);
  ui_->learningProgressBar_2->setValue(0);
  ui_->plot->xAxis->setRangeUpper(ui_->epochCountSpinBox->value());

  if (ui_->learningTypeComboBox->currentText() == "Default") {
    ui_->learningProgressBar_1->setMaximum(ui_->epochCountSpinBox->value());
    ui_->plot->legend->setVisible(false);

    controller_->Train(ui_->hiddenLayersSpinBox->value(),
                       ui_->epochCountSpinBox->value(),
                       ui_->learningRateSpinBox->value());

  } else {
    ui_->learningProgressBar_1->setMaximum(ui_->epochCountSpinBox->value() *
                                           ui_->foldCountSpinBox->value());
    ui_->plot->legend->setVisible(true);

    controller_->CrossValidation(
        ui_->hiddenLayersSpinBox->value(), ui_->epochCountSpinBox->value(),
        ui_->foldCountSpinBox->value(), ui_->learningRateSpinBox->value());
  }
}

void MainForm::on_testingStartButton_clicked() {
  if (ui_->testingStartButton->text() == "Start") {
    ui_->testingStartButton->setText("Stop");
    ui_->testingStartButton->setStyleSheet(
        "QPushButton{background-color: rgb(244,113,116)}");

    // lock ui
    ui_->recognitionTab->setEnabled(false);
    ui_->learningTab->setEnabled(false);
    ui_->testingRateSpinBox->setEnabled(false);
    ui_->testingProgressBar->setEnabled(true);
    SetMainTweaksEnbaled(false);

  } else {
    OnTestingCompleted(0, 0, 0, 0, 0);
  }

  ui_->testingProgressBar->setValue(0);

  controller_->TestMLP(ui_->testingRateSpinBox->value());
}

void MainForm::on_scribbleArea_updated(QImage image) {
  controller_->GuessLetter(
      image.scaled(28, 28, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void MainForm::on_scribbleArea_cleared() {
  ui_->resultLabel->setText("A");
  for (auto& bar : letter_pct_bars_) bar->setValue(0);
}

void MainForm::OnWeightsLoaded(const std::string& filename, int layers,
                               int neurons, double precision) {
  ui_->weightsFileLabel->setText(
      QString::fromStdString(filename).section("/", -1).left(10) + "..");
  ui_->hiddenLayersLabel->setText(QString::number(layers));
  ui_->neuronsInLayerLabel->setText(QString::number(neurons));
  ui_->weightsPrecisionLabel->setText(QString::number(precision, 'g', 3) + "%");
}

void MainForm::OnTrainDataLoaded(const std::string& filename, double size) {
  ui_->learningImportButton->setEnabled(true);
  ui_->learningFileLabel->setText(
      QString::fromStdString(filename).section("/", -1).left(10) + "..");
  ui_->learningSizeLabel->setText(QString::number(size));
}

void MainForm::OnTestDataLoaded(const std::string& filename, double size) {
  ui_->testingImportButton->setEnabled(true);
  ui_->testingFileLabel->setText(
      QString::fromStdString(filename).section("/", -1).left(10) + "..");
  ui_->testingSizeLabel->setText(QString::number(size));
}

void MainForm::OnLearningCompleted() {
  ui_->learningStartButton->setText("Start");
  ui_->learningStartButton->setStyleSheet(
      "QPushButton{background-color: rgb(111,194,118)}");

  // unlock ui
  ui_->recognitionTab->setEnabled(true);
  ui_->testingTab->setEnabled(true);
  SetLearningTweaksEnabled(true);
  SetMainTweaksEnbaled(true);
  ui_->learningProgressBar_1->setEnabled(false);
  ui_->learningProgressBar_2->setEnabled(false);
}

void MainForm::OnEpochPassed(int epoch, double error) {
  int progress = epoch;

  if (ui_->learningTypeComboBox->currentText() == "Cross validation")
    progress += ui_->epochCountSpinBox->value() * (ui_->plot->graphCount() - 1);

  ui_->learningProgressBar_1->setValue(progress);

  if (ui_->plot->graph()) {
    ui_->plot->graph()->addData(epoch, error);
    ui_->plot->yAxis->rescale();
    ui_->plot->replot();
  }
}

void MainForm::OnEpochProgressChanged(int percentage) {
  ui_->learningProgressBar_2->setValue(percentage);
}

void MainForm::OnFoldPassed() { AddGraph(); }

void MainForm::OnTestingProgressChanged(int percentage) {
  ui_->testingProgressBar->setValue(percentage);
}

void MainForm::OnTestingCompleted(double accuracy, double precision,
                                  double recall, double f_measure,
                                  double time) {
  ui_->testingStartButton->setText("Start");
  ui_->testingStartButton->setStyleSheet(
      "QPushButton{background-color: rgb(111,194,118)}");

  // unlock ui
  SetMainTweaksEnbaled(true);
  ui_->learningTab->setEnabled(true);
  ui_->recognitionTab->setEnabled(true);
  ui_->testingRateSpinBox->setEnabled(true);
  ui_->testingProgressBar->setEnabled(false);

  ui_->testingAccuracyLabel->setText(QString::number(accuracy, 'g', 3));
  ui_->testingPrecisionLabel->setText(QString::number(precision, 'g', 3));
  ui_->testingRecallLabel->setText(QString::number(recall, 'g', 3));
  ui_->testingMeasureLabel->setText(QString::number(f_measure, 'g', 3));
  ui_->testingTimeLabel->setText(QString::number(time, 'g', 3));
}

void MainForm::OnRecognitionCompleted(char letter,
                                      const std::vector<double>& output) {
  ui_->resultLabel->setText(QString(letter));
  for (qsizetype i = 0; i < letter_pct_bars_.size(); i++)
    letter_pct_bars_[i]->setValue(output[i] * 100);
}

void MainForm::OnMessage(const std::string& text) {
  QMessageBox::information(this, "Info", QString::fromStdString(text));
}

void MainForm::OnError(const std::string& text) {
  QMessageBox::critical(this, "Error", QString::fromStdString(text));
}
