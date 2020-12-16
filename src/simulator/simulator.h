#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <array>
#include <QString>

extern "C" {
#include <z80.h>
}

#include "mapreader.h"
#include "breakpoint.h"

class Simulator {
 public:
  Simulator();
  Z80Context *context() {
    return &m_ctx;
  }

  bool load(const QString &filename, size_t address,
            size_t offset = 0, size_t length = 0);

  bool loadMap(const QString &filename);

  void step();
  void reset();

  const QString operation() const;

  unsigned int tStates() const {
    return m_ctx.tstates;
  }

  char *memPtr() {
    return m_memory.data();
  }

  size_t memSize() {
    return m_memory.size();
  }

  const QString &codeListing() const {
    return m_mapReader.content();
  }

  int codePosition() const {
    return m_mapReader.position(m_ctx.PC);
  }

  int calcPosition(ushort pc) const {
    return m_mapReader.position(pc);
  }

  int calcPC(int position) const {
    return m_mapReader.pc(position);
  }

  bool assertBreakpoint(const Breakpoint *bp) const;

 private:
  byte read(ushort address);
  void write(ushort address, byte data);

  byte in(ushort address);
  void out(ushort address, byte data);

  Z80Context m_ctx;
  std::array<char, 64 * 1024> m_memory;
  MapReader m_mapReader;

  friend byte readMem(void *param, ushort address);
  friend void writeMem(void *param, ushort address, byte data);
  friend byte readIO(void *param, ushort address);
  friend void writeIO(void *param, ushort address, byte data);
};

byte readMem(void *param, ushort address);
void writeMem(void *param, ushort address, byte data);
byte readIO(void *param, ushort address);
void writeIO(void *param, ushort address, byte data);

#endif // SIMULATOR_H
