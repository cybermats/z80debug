#include "options.h"

Options::Options()
    : m_continueTStates(1000000), m_syntaxHighlighting(true) {
}

size_t Options::continueTStates() const {
  return m_continueTStates;
}

void Options::setContinueTStates(const size_t &continueTStates) {
  m_continueTStates = continueTStates;
}
bool Options::syntaxHighlighting() const {
  return m_syntaxHighlighting;
}
void Options::setSyntaxHighlighting(bool m_syntax_highlighting) {
  m_syntaxHighlighting = m_syntax_highlighting;
}
