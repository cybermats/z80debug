#ifndef OPTIONS_H
#define OPTIONS_H

#include <cstdio>

class Options {
 public:
  Options();

  size_t continueTStates() const;
  void setContinueTStates(const size_t &continueTStates);

 private:
  size_t m_continueTStates;
};

#endif // OPTIONS_H
