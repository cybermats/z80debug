//
// Created by mats on 2020-12-17.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Highlighter.h" resolved

#include <QColor>
#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent) :
    QSyntaxHighlighter(parent) {

  auto mnemFormat = QTextCharFormat();
  mnemFormat.setForeground(Qt::darkMagenta);
  mnemFormat.setFontWeight(QFont::Bold);
  const QString mnemPatterns[] = {
      QStringLiteral("\\b(?i:(j[pr]|call|ret))(?:\\s+(?i:([cmpz]|n[cz]|p[eo])))?\\b"),
      QStringLiteral(
          "\\s(?i:ad[cd]|and|bit|ccf|cp|cp[di]r?|cpl|daa|dec|[de]i|djnz|exx?|halt|i[mn]|inc|in[di]r?|ld|ld[di]r?|neg|nop|or|ot[di]r|out|out[di]|pop|push|res|ret[in]|rla?|rlca?|r[lr]d|rra?|rrca?|rst|sbc|scf|set|s[lr]a|s[lr]l|slia|sl1|sub|x?or)\\s")
  };
  for (const auto &pattern : mnemPatterns) {
    m_rules.append(HighlightingRule{
        QRegularExpression(pattern),
        mnemFormat
    });
  }

  auto controlFormat = QTextCharFormat();
  controlFormat.setForeground(Qt::red);
  controlFormat.setFontWeight(QFont::Bold);
  const QString controlPatterns[] = {
      QStringLiteral(
          "(?i:(?<=\\s)(?:equ|eval|[fr]?org|end?t|align|(?:de|un)?phase|shift|save(?:bin|dev|hob|nex|sna|tap|trd)|empty(?:tap|trd)|inc(?:bin|hob|trd)|binclude|includelua|insert|binary|end|out(?:put|end)|tap(?:out|end)|fpos|fname|slot|size|opt)\\b)"),
      QStringLiteral(
          "(?i:(?<=\\s)(?:cpu|device|proc|macro|label|local|global|shared|public|forward|rept|e?dup|block|end[mpr]|exitm|module|endmod(?:ule)?|(?:un)?define|export|disp|textarea|map|mmu|field|defarray|segment|restore|pushv|popv|enum|enumconf|nextenum)\\b)"),
      QStringLiteral(
          "(?i:(?<=\\s)(?:assert|fatal|error|warning|message|display|print|title|prtit|prtexit|fail|shellexec|def[bdghlmswir]|d[bcghmswz]|abyte[cz]?|byte|d?word|hex)\\b)"),
      QStringLiteral(
          "(?i:(?<=\\s)(?:page|newpage|radix|outradix|encoding|charset|codepage|macexp_(?:dft|ovr)|listing|(?:end)?struct|(?:end)?section|maxnest)\\b)"),
      QStringLiteral("(?i:(?<=\\s)(?:if|ifn?def|ifn?used|ifn?exist|else|elseif|endif)\\b)"),
      QStringLiteral("(?i:(?<=\\s)(?:bank|bankset|limit|protect|write\\s+direct|str)\\b)"),
      QStringLiteral("(?i:(?<=\\s)(?:inc(?:l4[89]|lz4|zx7|exo)|lz(?:4[89]?|w7|exo|close)|read)\\b)"),
      QStringLiteral(
          "(?i:(?<=\\s)(?:repeat|rend|until|switch|(?:else|end)case|default|break|endswitch|stop|while|wend|function)\\b)"),
      QStringLiteral("(?i:(?<=\\s)(?:amsdos|breakpoint|buildcpr|buildsna|run|save|setcpc|setcrtc)\\b)"),
      QStringLiteral("(?i:(?<=\\s)(?:list|nolist|let|labelslist)\\b)")
  };
  for (const auto &pattern : controlPatterns) {
    m_rules.append(HighlightingRule{
        QRegularExpression(pattern),
        controlFormat
    });
  }

  auto registerFormat = QTextCharFormat();
  registerFormat.setForeground(Qt::magenta);
  m_rules.append(HighlightingRule{
      QRegularExpression(QStringLiteral(
                             "\\b(?i:[abcdefhlir]|ix|iy|af'?|bc|de|hl|pc|sp|ix[hlu]|iy[hlu]|[lh]x|x[lh]|[lh]y|y[lh])\\b")),
      registerFormat
  });

  auto stringFormat = QTextCharFormat();
  stringFormat.setForeground(QColor::fromRgb(233, 185, 110));
  m_rules.append(HighlightingRule{
      QRegularExpression(QStringLiteral("\".*\"")),
      stringFormat
  });

  auto labelFormat = QTextCharFormat();
  labelFormat.setForeground(QColor::fromRgb(252, 175, 62));
  m_rules.append(HighlightingRule{
      QRegularExpression(QStringLiteral("(\\b|\\.)?[A-Za-z_]+:")),
      labelFormat
  });

  auto numberFormat = QTextCharFormat();
  numberFormat.setForeground(QColor::fromRgb(233, 178, 227));
  m_rules.append(HighlightingRule{
      QRegularExpression(QStringLiteral("\\b[0-9]+\\b")),
      numberFormat
  });
  m_rules.append(HighlightingRule{
      QRegularExpression(QStringLiteral("\\b[0-9][A-Fa-f0-9]+h\\b")),
      numberFormat
  });

  auto commentFormat = QTextCharFormat();
  commentFormat.setForeground(Qt::darkGreen);
  m_rules.append(HighlightingRule{
      QRegularExpression(QStringLiteral(";[^\n]*")),
      commentFormat
  });
}

Highlighter::~Highlighter() {
}
void Highlighter::highlightBlock(const QString &text) {
  for (const auto &rule : qAsConst(m_rules)) {
    auto matchIterator = rule.pattern.globalMatch(text);
    while (matchIterator.hasNext()) {
      auto match = matchIterator.next();
      setFormat(match.capturedStart(), match.capturedLength(), rule.format);
    }
  }
}
