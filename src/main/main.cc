#include <QApplication>

#include "main_form.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  s21::Model model;
  s21::Controller controller(&model);
  s21::MainForm form(&controller);
  form.show();

  return app.exec();
}
