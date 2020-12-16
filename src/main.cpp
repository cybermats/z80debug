#include "src/ui/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  Simulator sim;
  Options options;
  MainWindow w(&sim, &options);
  w.show();
  return a.exec();
}
