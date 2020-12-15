#ifndef REGISTERMODEL_H
#define REGISTERMODEL_H

#include <QAbstractItemModel>
#include <QStringBuilder>
#include "registeritem.h"

extern "C" {
    #include "z80.h"
}


class RegisterModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit RegisterModel(Z80Context *ctx, QObject *parent = nullptr);
    ~RegisterModel();

    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    void update();
    void benchmark();

private:

    void setupModelData(Z80Context *ctx, RegisterItem *parent);
    RegisterItem *getItem(const QModelIndex &index) const;

    static QString format(byte value) {
        return QString::number(value, 16).rightJustified(2, '0');
    }
    static QString format(ushort value) {
        return QString::number(value, 16).rightJustified(4, '0');
    }

    static QString formatFlags(byte value) {
        return ((value & 0b10000000) ? QChar('S') : QChar('s')) %
            ((value & 0b01000000) ? QChar('Z') : QChar('z')) %
            ((value & 0b00010000) ? QChar('H') : QChar('h')) %
            ((value & 0b00000100) ? QChar('P') : QChar('p')) %
            ((value & 0b00000010) ? QChar('N') : QChar('n')) %
            ((value & 0b00000001) ? QChar('C') : QChar('c'));
    }

    Z80Context *m_ctx;
    Z80Context *m_prevCtx;
    RegisterItem *m_rootItem;


    enum class Reg {
        Main,
        Alternate,
        Special,
        Interrupt,
        AF,
        HL,
        BC,
        DE,
        A,
        F,
        H,
        L,
        B,
        C,
        D,
        E,
        AFp,
        HLp,
        BCp,
        DEp,
        Ap,
        Fp,
        Hp,
        Lp,
        Bp,
        Cp,
        Dp,
        Ep,
        IX,
        IY,
        PC,
        SP,
        I,
        R,
        IFF1,
        IFF2
    };
};

#endif // REGISTERMODEL_H
