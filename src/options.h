#ifndef OPTIONS_H
#define OPTIONS_H

#include <cstdio>

class Options {
 public:
  Options();

  size_t continueTStates() const;
  void setContinueTStates(const size_t &continueTStates);
  bool syntaxHighlighting() const;
  void setSyntaxHighlighting(bool m_syntax_highlighting);

 private:
  size_t m_continueTStates;
  bool m_syntaxHighlighting;
 public:
};

#endif // OPTIONS_H
