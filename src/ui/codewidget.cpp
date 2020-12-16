#include <QMenu>
#include <QDebug>
#include "codewidget.h"

CodeWidget::CodeWidget(QWidget *parent)
    : QPlainTextEdit(parent)
{

}

void CodeWidget::setSimulator(Simulator *sim)
{
    m_simulator = sim;
}

void CodeWidget::setBreakpoints(QVector<Breakpoint *> *breakpoints)
{
    m_breakpoints = breakpoints;
}

void CodeWidget::refresh()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (m_breakpoints && !m_breakpoints->empty()) {
        for (auto bp : *m_breakpoints) {
            auto pc = bp->getPC();
            if (pc < 0)
                continue;
            auto pos = m_simulator->calcPosition(pc);

            auto cursor = textCursor();
            cursor.setPosition(pos);

            QTextEdit::ExtraSelection selection = QTextEdit::ExtraSelection();
            selection.format.setBackground(Qt::red);
            selection.format.setProperty(QTextFormat::FullWidthSelection, true);
            selection.cursor = cursor;
            selection.cursor.clearSelection();
            extraSelections.append(selection);
        }
    }

    auto codePos = m_simulator->codePosition();
    if (codePos >= 0) {
        auto cursor = textCursor();
        cursor.setPosition(codePos);
        setTextCursor(cursor);

        QTextEdit::ExtraSelection selection = QTextEdit::ExtraSelection();
        QColor lineColor = QColor(Qt::yellow);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = cursor;
        selection.cursor.clearSelection();
        extraSelections.append(selection);

    }

    setExtraSelections(extraSelections);
}

void CodeWidget::contextMenuEvent(QContextMenuEvent *e)
{
    auto cursor = cursorForPosition(e->pos());
//    if (!cursor.movePosition(QTextCursor::MoveOperation::StartOfBlock, QTextCursor::MoveMode::MoveAnchor))
//        return;
    auto pos = cursor.position();
    auto pc = m_simulator->calcPC(pos);
    qDebug() << "context: pc: " << pc << ", pos: " << pos;

    int found = -1;
    if (pc >= 0) {
        for (int i = 0; i < m_breakpoints->size(); ++i)
        {
            if ((*m_breakpoints)[i]->getPC() == pc) {
                found = i;
                break;
            }
        }
    }
    bool updated = false;
    auto *menu = new QMenu(this);
    if (found >= 0) {
        menu->addAction("Remove Breakpoint");
        if (menu->exec(e->globalPos())) {
            m_breakpoints->remove(found);
            updated = true;
        }

    }
    else {
        menu->addAction("Add Breakpoint");
        if (menu->exec(e->globalPos())) {
            m_breakpoints->push_back(new CodeBreakpoint(pc));
            updated = true;
        }
    }
    delete menu;
    if (updated)
        refresh();
}
