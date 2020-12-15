#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFutureWatcher>
#include "src/simulator/registermodel.h"
#include "src/simulator/simulator.h"
#include "loaddialog.h"
#include "src/simulator/breakpoint.h"
#include "src/options.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Simulator *sim, Options *options, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionLoad_triggered();
    void on_actionLoad_finished(int result);

    void on_actionReset_triggered();

    void on_actionStep_triggered();

    void on_actionQuit_triggered();

    void on_actionLoad_many_triggered();

    void on_actionContinue_triggered();

private:
    void refresh();
    void loadFile(QString filename, size_t address = 0, size_t offset = 0, size_t length = 0);

    void on_futureWatcher_finished();

    Ui::MainWindow *ui;
    RegisterModel m_regModel;
    Simulator *m_simulator;
    LoadDialog *m_loadDialog;
    QVector<Breakpoint*> m_breakpoints;
    Options *m_options;
    QFutureWatcher<void> m_futureWatcher;
};
#endif // MAINWINDOW_H
