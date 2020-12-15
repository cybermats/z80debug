#include <QFile>
#include <QFileInfo>
#include <QDataStream>
#include <QTextStream>

#include "simulator.h"

Simulator::Simulator()
{
    m_ctx.memRead = readMem;
    m_ctx.memWrite = writeMem;
    m_ctx.ioRead = readIO;
    m_ctx.ioWrite = writeIO;
    m_ctx.memParam = this;
    m_ctx.ioParam = this;
}

bool Simulator::load(const QString &filename, size_t address, size_t offset, size_t length)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    QDataStream stream(&file);

    if (0 < stream.skipRawData(offset))
        return false;

    if (length == 0)
        length = m_memory.size();
    length = length - address;

    if (0 > stream.readRawData(&m_memory[address], length))
        return false;

    return true;
}

bool Simulator::loadMap(const QString &filename)
{
    return m_mapReader.load(filename);

}

void Simulator::step()
{
    Z80Execute(&m_ctx);
}

void Simulator::reset()
{
    Z80RESET(&m_ctx);
}

const QString Simulator::operation() const
{
    char buffer[512];
    Z80Context ctx(m_ctx);
    Z80Debug(&ctx, nullptr, buffer);
    return buffer;
}

bool Simulator::assertBreakpoint(const Breakpoint *bp) const
{
    if (bp->isActive())
        return bp->isAsserted(&m_ctx, &m_memory[0]);
    return false;
}

byte Simulator::read(ushort address)
{
    return m_memory[address];
}

void Simulator::write(ushort address, byte data)
{
    m_memory[address] = data;
}

byte Simulator::in(ushort address)
{
    return 0;
}

void Simulator::out(ushort address, byte data)
{
}

byte readMem(void *param, ushort address)
{
    return ((Simulator*) param)->read(address);
}

void writeMem(void *param, ushort address, byte data)
{
    ((Simulator*) param)->write(address, data);
}

byte readIO(void *param, ushort address)
{
    return ((Simulator*) param)->in(address);
}

void writeIO(void *param, ushort address, byte data)
{
    ((Simulator*) param)->out(address, data);
}
