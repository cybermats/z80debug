#ifndef CODEWIDGET_H
#define CODEWIDGET_H

#include <QWidget>
#include <QPlainTextEdit>

#include "src/simulator/simulator.h"

class CodeWidget : public QPlainTextEdit
{
    Q_OBJECT
public:
    CodeWidget(const QString &text, QWidget *parent = nullptr);
    CodeWidget(QWidget *parent = nullptr);

    void setSimulator(Simulator *sim);

    void setBreakpoints(QVector<Breakpoint*> *breakpoints);

    void refresh();

protected:
    virtual void contextMenuEvent(QContextMenuEvent *e) override;

private:
    Simulator *m_simulator = nullptr;
    QVector<Breakpoint*> *m_breakpoints = nullptr;
};

#endif // CODEWIDGET_H
