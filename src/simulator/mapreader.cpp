#include <algorithm>

#include <QFile>
#include <QTextStream>

#include "mapreader.h"

MapReader::MapReader()
    : m_pattern("^[^\\/]*\\/ +([[:xdigit:]]+)[^:]+:.*$") {

}

bool MapReader::load(QString filename) {
  QFile file(filename);
  if (!file.open(QIODevice::Text | QIODevice::ReadOnly))
    return false;

  QTextStream stream(&file);
  m_content.clear();
  m_positionLookup.clear();

  size_t position = 0;
  while (!stream.atEnd()) {
    auto line = stream.readLine() + '\n';
    m_content.append(line);
    // parse line
    auto match = m_pattern.match(line);
    if (match.hasMatch()) {
      auto addrStr = match.captured(1);
      if (!addrStr.isEmpty()) {
        bool ok;
        auto addr = addrStr.toUInt(&ok, 16);
        if (ok) {
          m_positionLookup[addr] = position;
          m_pcLookup.push_back(std::make_pair(position, addr));
        }
      }
    }

    position += line.size();
  }
  return true;
}

const QString &MapReader::content() const {
  return m_content;
}

int MapReader::position(ushort programCounter) const {
  auto it = m_positionLookup.find(programCounter);
  if (it != m_positionLookup.end()) {
    return it->second;
  }
  return -1;
}

int MapReader::pc(size_t position) const {
  auto it = std::upper_bound(m_pcLookup.begin(), m_pcLookup.end(), std::make_pair(position, (ushort) 0));
  if (it == m_pcLookup.end())
    return -1;

  if (it != m_pcLookup.begin())
    --it;

  return it->second;
}
