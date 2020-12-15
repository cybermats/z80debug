#ifndef MAPREADER_H
#define MAPREADER_H

#include <vector>
#include <map>
#include <QString>
#include <QRegularExpression>


class MapReader
{
public:
    MapReader();

    bool load(QString filename);

    const QString& content() const;

    int position(ushort programCounter) const;

    int pc(size_t position) const;

private:
    QRegularExpression m_pattern;
    QString m_content;
    std::map<ushort, int> m_positionLookup;
    std::vector<std::pair<size_t, ushort>> m_pcLookup;
};

#endif // MAPREADER_H
