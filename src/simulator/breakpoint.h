#ifndef BREAKPOINT_H
#define BREAKPOINT_H

extern "C" {
#include <z80.h>
}


class Breakpoint
{
public:
    Breakpoint()
        : m_active(true)
    {}

    virtual ~Breakpoint()= default;

    virtual bool isAsserted(const Z80Context *ctx, const char *memory) const = 0;
    bool isActive() const { return m_active; }
    void setActive(bool value) { m_active = value; }

    virtual int getPC() const {
        return -1;
    }

    virtual int getAddr() const {
        return -1;
    }

private:
    bool m_active;
};

class CodeBreakpoint : public Breakpoint
{
public:
    explicit CodeBreakpoint(ushort pc)
        : m_pc(pc)
    {
    }
    bool isAsserted(const Z80Context *ctx, const char *) const override {
        return ctx->PC == m_pc;
    }

    int getPC() const override {
        return m_pc;
    }

private:
    ushort m_pc;
};


#endif // BREAKPOINT_H
