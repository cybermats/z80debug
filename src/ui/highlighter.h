//
// Created by mats on 2020-12-17.
//

#ifndef Z80DEBUG_SRC_UI_HIGHLIGHTER_H_
#define Z80DEBUG_SRC_UI_HIGHLIGHTER_H_

#include <QSyntaxHighlighter>
#include <QRegularExpression>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

class Highlighter : public QSyntaxHighlighter {
 Q_OBJECT

 public:
  explicit Highlighter(QTextDocument *parent = nullptr);
  ~Highlighter() override;

 protected:
  void highlightBlock(const QString &text) override;

 private:
  struct HighlightingRule {
    QRegularExpression pattern;
    QTextCharFormat format;
  };

  QVector<HighlightingRule> m_rules;
};

#endif //Z80DEBUG_SRC_UI_HIGHLIGHTER_H_
