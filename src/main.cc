#include <QApplication>

#include "main_form.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  Model model;
  Controller controller(&model);
  MainForm form(&controller);
  form.show();

  return app.exec();
}
