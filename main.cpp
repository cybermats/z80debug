#include "mainwindow.h"

#include <QApplication>


#include <QBuffer>
#include <vector>
#include <iostream>
#include <string>

#include "options.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Simulator sim;
    Options options;
    MainWindow w(&sim, &options);
    w.show();
    return a.exec();
}
