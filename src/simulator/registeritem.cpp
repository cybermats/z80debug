#include "registeritem.h"
#include <QStringBuilder>
#include <QFont>
#include <QColor>

RegisterItem::RegisterItem(QString name, RegisterItem *parentItem, bool editable)
    : m_name(name)
    , m_parentItem(parentItem)
    , m_edited(false)
    , m_editable(editable)
{}

RegisterItem::~RegisterItem()
{
    qDeleteAll(m_childItems);
}

void RegisterItem::appendChild(RegisterItem *child)
{
    m_childItems.append(child);
}

RegisterItem *RegisterItem::child(int row)
{
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

int RegisterItem::childCount() const
{
    return m_childItems.count();
}

int RegisterItem::columnCount() const
{
    return 2;
}

QVariant RegisterItem::data(int column, int role) const
{
    Q_UNUSED(role);
    if (role == Qt::DisplayRole && column == 0)
        return m_name;
    if (column == 1) {
        if (role == Qt::DisplayRole || role == Qt::EditRole)
            return toString();
        else if (role == Qt::FontRole) {
            QFont font;
            if (m_edited)
                font.setItalic(true);
            return font;
        }
        else if (role == Qt::ForegroundRole) {
            if (hasChanged())
                return QColor(Qt::red);
            return QColor(Qt::black);
        }
    }
    return QVariant();

}

bool RegisterItem::setData(int column, const QVariant &value, int role)
{
    auto result = false;
    if (column == 1)
        result = setItemData(value, role);
    if (result)
        m_edited = true;
    return result;
}

int RegisterItem::childNumber() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<RegisterItem*>(this));
    return 0;
}

RegisterItem *RegisterItem::parentItem()
{
    return m_parentItem;
}

void RegisterItem::benchmark()
{
    m_edited = false;
    for(auto item : m_childItems)
        item->benchmark();
}

bool RegisterItem::hasChanged() const
{
    return false;
}

QString RegisterItem::toString() const
{
    return QString("...");
}

bool RegisterItem::setItemData(const QVariant &value, int role)
{
    Q_UNUSED(value);
    Q_UNUSED(role);
    return false;
}

ByteRegisterItem::ByteRegisterItem(QString name, uchar *value, RegisterItem *parentItem, bool editable)
    : RegisterItem(name, parentItem, editable)
    , m_value(value)
{}

QString ByteRegisterItem::toString() const
{
    return QString::number(*m_value, 16).rightJustified(2, '0');
}

bool ByteRegisterItem::setItemData(const QVariant &value, int role)
{
    if (role == Qt::EditRole)
    {
        bool ok;
        auto val = value.toString().toInt(&ok, 16);
        if (!ok)
            return false;
        *m_value = (uchar)val;
        return true;
    }
    return false;
}

void ByteRegisterItem::benchmark()
{
    RegisterItem::benchmark();
    m_oldValue = *m_value;
}

bool ByteRegisterItem::hasChanged() const
{
    return *m_value != m_oldValue;
}

ShortRegisterItem::ShortRegisterItem(QString name, ushort *value, RegisterItem *parentItem, bool editable)
    : RegisterItem(name, parentItem, editable)
    , m_value(value)
{}

QString ShortRegisterItem::toString() const
{
    return QString::number(*m_value, 16).rightJustified(4, '0');
}

bool ShortRegisterItem::setItemData(const QVariant &value, int role)
{
    if (role == Qt::EditRole)
    {
        bool ok;
        auto val = value.toString().toInt(&ok, 16);
        if (!ok)
            return false;
        *m_value = (ushort)val;
        return true;
    }
    return false;
}

void ShortRegisterItem::benchmark()
{
    RegisterItem::benchmark();
    m_oldValue = *m_value;
}

bool ShortRegisterItem::hasChanged() const
{
    return *m_value != m_oldValue;
}

IntRegisterItem::IntRegisterItem(QString name, unsigned *value, RegisterItem *parentItem, bool editable)
    : RegisterItem(name, parentItem, editable)
    , m_value(value)
{}

QString IntRegisterItem::toString() const
{
    return QString::number(*m_value, 10);
}

bool IntRegisterItem::setItemData(const QVariant &value, int role)
{
    if (role == Qt::EditRole)
    {
        bool ok;
        auto val = value.toString().toInt(&ok, 16);
        if (!ok)
            return false;
        *m_value = (ushort)val;
        return true;
    }
    return false;
}

void IntRegisterItem::benchmark()
{
    RegisterItem::benchmark();
    m_oldValue = *m_value;
}

bool IntRegisterItem::hasChanged() const
{
    return *m_value != m_oldValue;
}

FlagRegisterItem::FlagRegisterItem(QString name, uchar *value, RegisterItem *parentItem, bool editable)
    : RegisterItem(name, parentItem, editable)
    , m_value(value)
{}

QString FlagRegisterItem::toString() const
{
    return QString(((*m_value & 0b10000000) ? QChar('S') : QChar('s')) %
        ((*m_value & 0b01000000) ? QChar('Z') : QChar('z')) %
        ((*m_value & 0b00010000) ? QChar('H') : QChar('h')) %
        ((*m_value & 0b00000100) ? QChar('P') : QChar('p')) %
        ((*m_value & 0b00000010) ? QChar('N') : QChar('n')) %
        ((*m_value & 0b00000001) ? QChar('C') : QChar('c')));
}

bool FlagRegisterItem::setItemData(const QVariant &value, int role)
{
    if (role == Qt::EditRole)
    {
        bool ok;
        auto val = value.toString().toInt(&ok, 16);
        if (!ok)
            return false;
        *m_value = (uchar)val;
        return true;
    }
    return false;
}

void FlagRegisterItem::benchmark()
{
    RegisterItem::benchmark();
    m_oldValue = *m_value;
}

bool FlagRegisterItem::hasChanged() const
{
    return *m_value != m_oldValue;
}
