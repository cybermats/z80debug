#ifndef CODEWIDGET_H
#define CODEWIDGET_H

#include <QWidget>
#include <QPlainTextEdit>

#include "src/simulator/simulator.h"

class CodeWidget : public QPlainTextEdit {
 Q_OBJECT
 public:
  explicit CodeWidget(QWidget *parent = nullptr);

  void setSimulator(Simulator *sim);

  void setBreakpoints(QVector<Breakpoint *> *breakpoints);

  void refresh();

 protected:
  void contextMenuEvent(QContextMenuEvent *e) override;

 private:
  Simulator *m_simulator = nullptr;
  QVector<Breakpoint *> *m_breakpoints = nullptr;
};

#endif // CODEWIDGET_H
