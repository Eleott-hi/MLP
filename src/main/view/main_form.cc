#include "main_form.h"

#include <QFileDialog>

#include "ui_main_form.h"

namespace s21 {

MainForm::MainForm(Controller* controller)
    : QMainWindow(nullptr), ui(new Ui::MainForm), controller_(controller) {
  ui->setupUi(this);
  SetupPlot();
  ConnectSignals();

  letter_pct_bars_ = {ui->pctBar_A, ui->pctBar_B, ui->pctBar_C, ui->pctBar_D,
                      ui->pctBar_E, ui->pctBar_F, ui->pctBar_G, ui->pctBar_H,
                      ui->pctBar_I, ui->pctBar_J, ui->pctBar_K, ui->pctBar_L,
                      ui->pctBar_M, ui->pctBar_N, ui->pctBar_O, ui->pctBar_P,
                      ui->pctBar_Q, ui->pctBar_R, ui->pctBar_S, ui->pctBar_T,
                      ui->pctBar_U, ui->pctBar_V, ui->pctBar_W, ui->pctBar_X,
                      ui->pctBar_Y, ui->pctBar_Z};
}

void MainForm::SetupPlot() {
  // epoch count
  ui->plot->xAxis->setLabel("Epoch");
  ui->plot->xAxis->setRangeLower(1.f);

  // error percentage
  ui->plot->yAxis->setLabel("Error");
}

void MainForm::AddGraph() {
  QColor color(rand() % 240, rand() % 240, rand() % 240);

  ui->plot->addGraph();
  ui->plot->graph()->setPen(QPen(color));
  color.setAlpha(20);
  ui->plot->graph()->setBrush(QBrush(color));
  ui->plot->graph()->setLineStyle(QCPGraph::lsLine);
  ui->plot->graph()->setScatterStyle(
      QCPScatterStyle(QCPScatterStyle::ssCircle, 5));

  if (ui->learningTypeComboBox->currentText() == "Cross validation") {
    ui->plot->graph()->setName("Fold: " +
                               QString::number(ui->plot->graphCount()));
  }
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
  ui->hiddenLayersSpinBox->setEnabled(state);
  ui->epochCountSpinBox->setEnabled(state);
  ui->foldCountSpinBox->setEnabled(state);
  ui->learningRateSpinBox->setEnabled(state);
  ui->learningTypeComboBox->setEnabled(state);
}

void MainForm::SetMainTweaksEnbaled(bool state) {
  ui->perceptronTypeComboBox->setEnabled(state);
  ui->weigthsImportButton->setEnabled(state);
  ui->weightsExportButton->setEnabled(state);
  ui->learningImportButton->setEnabled(state);
  ui->testingImportButton->setEnabled(state);
}

MainForm::~MainForm() { delete ui; }

void MainForm::on_perceptronTypeComboBox_currentIndexChanged(int index) {
  controller_->ChangeMLP(index);
}

void MainForm::on_weigthsImportButton_clicked() {
  QString filename = QFileDialog::getOpenFileName(this, "Open File", "~/",
                                                  "Text files (*.txt)");
  if (!filename.isEmpty()) {
    controller_->LoadWeights(filename.toStdString());
  }
}

void MainForm::on_weightsExportButton_clicked() {
  QString filename = QFileDialog::getSaveFileName(this, "Save File", "~/",
                                                  "Text files (*.txt)");
  if (!filename.isEmpty()) {
    controller_->SaveWeights(filename.toStdString());
  }
}

void MainForm::on_learningImportButton_clicked() {
  QString filename = QFileDialog::getOpenFileName(this, "Open File", "~/",
                                                  "Text files (*.csv)");
  if (!filename.isEmpty()) {
    ui->learningImportButton->setEnabled(false);
    controller_->LoadTrainData(filename.toStdString());
  }
}

void MainForm::on_testingImportButton_clicked() {
  QString filename = QFileDialog::getOpenFileName(this, "Open File", "~/",
                                                  "Text files (*.csv)");
  if (!filename.isEmpty()) {
    ui->testingImportButton->setEnabled(false);
    controller_->LoadTestData(filename.toStdString());
  }
}

void MainForm::on_imageImportButton_clicked() {
  QString filename = QFileDialog::getOpenFileName(this, "Open File", "~/",
                                                  "Image files (*.bmp)");
  if (!filename.isEmpty()) {
    ui->scribbleArea->OpenImage(filename);
  }
}

void MainForm::on_learningStartButton_clicked() {
  if (ui->learningStartButton->text() == "Start") {
    ui->learningStartButton->setText("Stop");
    ui->learningStartButton->setStyleSheet(
        "QPushButton{background-color: rgb(244,113,116)}");

    // lock ui
    ui->recognitionTab->setEnabled(false);
    ui->testingTab->setEnabled(false);
    SetLearningTweaksEnabled(false);
    SetMainTweaksEnbaled(false);
    ui->learningProgressBar_1->setEnabled(true);
    ui->learningProgressBar_2->setEnabled(true);

    ui->plot->clearGraphs();
    ui->plot->replot();
    AddGraph();
  } else {
    OnLearningCompleted();
  }

  ui->learningProgressBar_1->setValue(0);
  ui->learningProgressBar_2->setValue(0);
  ui->plot->xAxis->setRangeUpper(ui->epochCountSpinBox->value());

  if (ui->learningTypeComboBox->currentText() == "Default") {
    ui->learningProgressBar_1->setMaximum(ui->epochCountSpinBox->value());
    ui->plot->legend->setVisible(false);

    controller_->Train(ui->hiddenLayersSpinBox->value(),
                       ui->epochCountSpinBox->value(),
                       ui->learningRateSpinBox->value());
  } else {
    ui->learningProgressBar_1->setMaximum(ui->epochCountSpinBox->value() *
                                          ui->foldCountSpinBox->value());
    ui->plot->legend->setVisible(true);

    controller_->CrossValidation(
        ui->hiddenLayersSpinBox->value(), ui->epochCountSpinBox->value(),
        ui->foldCountSpinBox->value(), ui->learningRateSpinBox->value());
  }
}

void MainForm::on_testingStartButton_clicked() {
  if (ui->testingStartButton->text() == "Start") {
    ui->testingStartButton->setText("Stop");
    ui->testingStartButton->setStyleSheet(
        "QPushButton{background-color: rgb(244,113,116)}");

    // lock ui
    ui->recognitionTab->setEnabled(false);
    ui->learningTab->setEnabled(false);
    ui->testingRateSpinBox->setEnabled(false);
    ui->testingProgressBar->setEnabled(true);
    SetMainTweaksEnbaled(false);

  } else {
    OnTestingCompleted(0, 0, 0, 0, 0);
  }

  ui->testingProgressBar->setValue(0);

  controller_->TestMLP(ui->testingRateSpinBox->value());
}

void MainForm::on_scribbleArea_updated(QImage image) {
  controller_->GuessLetter(
      image.scaled(28, 28, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void MainForm::on_scribbleArea_cleared() {
  ui->resultLabel->setText("A");
  for (QProgressBar* bar : letter_pct_bars_) bar->setValue(0);
}

void MainForm::OnWeightsLoaded(const std::string& filename, int layers,
                               int neurons, double precision) {
  ui->weightsFileLabel->setText(
      QString::fromStdString(filename).section("/", -1).left(10) + "..");
  ui->hiddenLayersLabel->setText(QString::number(layers));
  ui->neuronsInLayerLabel->setText(QString::number(neurons));
  ui->weightsPrecisionLabel->setText(QString::number(precision, 'g', 3) + "%");
}

void MainForm::OnTrainDataLoaded(const std::string& filename, double size) {
  ui->learningImportButton->setEnabled(true);
  ui->learningFileLabel->setText(
      QString::fromStdString(filename).section("/", -1).left(10) + "..");
  ui->learningSizeLabel->setText(QString::number(size));
}

void MainForm::OnTestDataLoaded(const std::string& filename, double size) {
  ui->testingImportButton->setEnabled(true);
  ui->testingFileLabel->setText(
      QString::fromStdString(filename).section("/", -1).left(10) + "..");
  ui->testingSizeLabel->setText(QString::number(size));
}

void MainForm::OnLearningCompleted() {
  ui->learningStartButton->setText("Start");
  ui->learningStartButton->setStyleSheet(
      "QPushButton{background-color: rgb(111,194,118)}");

  // unlock ui
  ui->recognitionTab->setEnabled(true);
  ui->testingTab->setEnabled(true);
  SetLearningTweaksEnabled(true);
  SetMainTweaksEnbaled(true);
  ui->learningProgressBar_1->setEnabled(false);
  ui->learningProgressBar_2->setEnabled(false);
}

void MainForm::OnEpochPassed(int epoch, double error) {
  int progress = epoch;
  if (ui->learningTypeComboBox->currentText() == "Cross validation") {
    progress += ui->epochCountSpinBox->value() * (ui->plot->graphCount() - 1);
  }
  ui->learningProgressBar_1->setValue(progress);

  if (ui->plot->graph()) {
    ui->plot->graph()->addData(epoch, error);
    ui->plot->yAxis->rescale();
    ui->plot->replot();
  }
}

void MainForm::OnEpochProgressChanged(int percentage) {
  ui->learningProgressBar_2->setValue(percentage);
}

void MainForm::OnFoldPassed() { AddGraph(); }

void MainForm::OnTestingProgressChanged(int percentage) {
  ui->testingProgressBar->setValue(percentage);
}

void MainForm::OnTestingCompleted(double accuracy, double precision,
                                  double recall, double f_measure,
                                  double time) {
  ui->testingStartButton->setText("Start");
  ui->testingStartButton->setStyleSheet(
      "QPushButton{background-color: rgb(111,194,118)}");

  // unlock ui
  ui->recognitionTab->setEnabled(true);
  ui->learningTab->setEnabled(true);
  ui->testingRateSpinBox->setEnabled(true);
  ui->testingProgressBar->setEnabled(false);
  SetMainTweaksEnbaled(true);

  ui->testingAccuracyLabel->setText(QString::number(accuracy, 'g', 3));
  ui->testingPrecisionLabel->setText(QString::number(precision, 'g', 3));
  ui->testingRecallLabel->setText(QString::number(recall, 'g', 3));
  ui->testingMeasureLabel->setText(QString::number(f_measure, 'g', 3));
  ui->testingTimeLabel->setText(QString::number(time, 'g', 3));
}

void MainForm::OnRecognitionCompleted(char letter,
                                      const std::vector<double>& output) {
  ui->resultLabel->setText(QString(letter));
  for (qsizetype i = 0; i < letter_pct_bars_.size(); i++) {
    letter_pct_bars_[i]->setValue(output[i] * 100);
  }
}

void MainForm::OnMessage(const std::string& text) {
  QMessageBox::information(this, "Info", QString::fromStdString(text));
}

void MainForm::OnError(const std::string& text) {
  QMessageBox::critical(this, "Error", QString::fromStdString(text));
}

}  // namespace s21
