#include <QMessageBox>
#include <QFileDialog>
#include <QTextCursor>
#include <QtConcurrent/QtConcurrent>

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "highlighter.h"

MainWindow::MainWindow(Simulator *sim, Options *options, QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      m_regModel(sim->context(), this),
      m_simulator(sim),
      m_options(options),
      m_loadDialog(new LoadDialog(this)) {
  ui->setupUi(this);
  ui->treeView->setModel(&m_regModel);
  ui->treeView->expandToDepth(0);
  ui->hexEdit->setBytesPerLine(8);
  ui->hexEdit->setData(m_simulator->memPtr(), m_simulator->memSize());
  ui->codeTextEdit->setSimulator(m_simulator);
  ui->codeTextEdit->setBreakpoints(&m_breakpoints);
  if (m_options->syntaxHighlighting())
    ui->codeTextEdit->setSyntaxHighlighter(new Highlighter());

  connect(m_loadDialog, &LoadDialog::finished, this, &MainWindow::onActionLoad_finished);
  connect(ui->stepButton, &QPushButton::released, this, &MainWindow::on_actionStep_triggered);
  connect(&m_futureWatcher, &QFutureWatcher<void>::finished, this, &MainWindow::on_futureWatcher_finished);
}

MainWindow::~MainWindow() {
  delete ui;
  delete m_loadDialog;
}

void MainWindow::on_actionLoad_triggered() {
  auto filename = QFileDialog::getOpenFileName(this,
                                               "Open Image", "", "Image Files (*.img *.bin);;(*.*)");
  loadFile(filename);
  ui->hexEdit->setData(m_simulator->memPtr(), m_simulator->memSize());
  refresh();
}

void MainWindow::onActionLoad_finished(int result) {
  if (result == QDialog::Accepted) {
    qDebug() << "Loading: ";
    for (auto fileinfo : m_loadDialog->getFileLoadInfo()) {
      loadFile(fileinfo.filename, fileinfo.address, fileinfo.offset, fileinfo.length);
      m_simulator->load(fileinfo.filename, fileinfo.address, fileinfo.offset, fileinfo.length);
    }
    ui->hexEdit->setData(m_simulator->memPtr(), m_simulator->memSize());
  }
}

void MainWindow::on_actionReset_triggered() {
  ui->hexEdit->write(m_simulator->memPtr(), m_simulator->memSize());
  m_regModel.benchmark();
  m_simulator->reset();
  refresh();
}

void MainWindow::on_actionStep_triggered() {
  ui->hexEdit->write(m_simulator->memPtr(), m_simulator->memSize());
  m_regModel.benchmark();
  m_simulator->step();
  refresh();
}

void MainWindow::on_actionQuit_triggered() {
  auto reply = QMessageBox::question(this, "Exit", "Are you sure you want to exit?");
  if (reply == QMessageBox::Yes) {
    close();
  }

}

void MainWindow::on_actionLoad_many_triggered() {
  m_loadDialog->open();
}

void MainWindow::refresh() {
  ui->hexEdit->refreshData();

  m_regModel.update();
  ui->opLabel->setText(m_simulator->operation());
  ui->codeTextEdit->refresh();
}

void MainWindow::loadFile(const QString &filename, size_t address, size_t offset, size_t length) {
  if (!filename.isEmpty()) {
    QFileInfo fileInfo(filename);
    if (!fileInfo.exists()) {
      QMessageBox::warning(this, "File not found", "Couldn't find file.");
      return;
    }
    if (!m_simulator->load(filename, address, offset, length)) {
      QMessageBox::warning(this, "File not loaded", "Unable to load file.");
      return;
    }

    // Try to load map file.
    QString mapFileName = fileInfo.completeBaseName() + ".lst";
    QDir dir = fileInfo.dir();
    QFileInfo mapFileInfo(dir.filePath(mapFileName));
    if (mapFileInfo.exists()) {
      if (m_simulator->loadMap(mapFileInfo.filePath())) {
        ui->codeTextEdit->setPlainText(m_simulator->codeListing());
      }
    }

  }

}

void MainWindow::on_futureWatcher_finished() {
  QApplication::restoreOverrideCursor();
  refresh();
}

void MainWindow::on_actionContinue_triggered() {
  m_regModel.benchmark();
  auto runUntil = m_simulator->tStates() + m_options->continueTStates();
  QApplication::setOverrideCursor(Qt::WaitCursor);
  auto future = QtConcurrent::run([=]() {
    while (m_simulator->tStates() < runUntil && !m_simulator->context()->halted) {
      m_simulator->step();
      for (auto bp : m_breakpoints) {
        if (m_simulator->assertBreakpoint(bp))
          return;
      }
    }
  });
  m_futureWatcher.setFuture(future);
}
