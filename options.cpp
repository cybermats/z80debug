#include "options.h"

Options::Options()
    : m_continueTStates(1000000)
{
}

size_t Options::continueTStates() const
{
    return m_continueTStates;
}

void Options::setContinueTStates(const size_t &continueTStates)
{
    m_continueTStates = continueTStates;
}
