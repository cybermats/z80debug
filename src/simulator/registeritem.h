#ifndef REGISTERITEM_H
#define REGISTERITEM_H

#include <QString>
#include <QVariant>
#include <QVector>

class RegisterItem
{
public:
    explicit RegisterItem(QString name, RegisterItem *parentItem = nullptr, bool editable = true);
    virtual ~RegisterItem();

    void appendChild(RegisterItem *child);

    RegisterItem *child(int childNumber);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column, int role) const;
    bool setData(int column, const QVariant &value, int role);
    int childNumber() const;
    RegisterItem *parentItem();
    virtual void benchmark();
    bool editable() const {
        return m_editable;
    }
protected:

private:
    virtual bool hasChanged() const;
    virtual QString toString() const;
    virtual bool setItemData(const QVariant &value, int role);

    QString m_name;
    QVector<RegisterItem*> m_childItems;
    RegisterItem *m_parentItem;
    bool m_edited;
    bool m_editable;
};

class ByteRegisterItem : public RegisterItem
{
public:
    explicit ByteRegisterItem(QString name, uchar *value, RegisterItem *parentItem = nullptr, bool editable = true);
    virtual void benchmark() override;
private:
    virtual bool hasChanged() const override;
    virtual QString toString() const;
    virtual bool setItemData(const QVariant &value, int role) override;
    uchar *m_value;
    uchar m_oldValue;
};

class ShortRegisterItem : public RegisterItem
{
public:
    explicit ShortRegisterItem(QString name, ushort *value, RegisterItem *parentItem = nullptr, bool editable = true);
    virtual void benchmark();

private:
    virtual bool hasChanged() const override;
    virtual QString toString() const;
    virtual bool setItemData(const QVariant &value, int role) override;
    ushort *m_value;
    ushort m_oldValue;
};

class IntRegisterItem : public RegisterItem
{
public:
    explicit IntRegisterItem(QString name, unsigned *value, RegisterItem *parentItem = nullptr, bool editable = true);
    virtual void benchmark();

private:
    virtual bool hasChanged() const override;
    virtual QString toString() const;
    virtual bool setItemData(const QVariant &value, int role) override;
    unsigned *m_value;
    unsigned m_oldValue;
};

class FlagRegisterItem : public RegisterItem
{
public:
    explicit FlagRegisterItem(QString name, uchar *value, RegisterItem *parentItem = nullptr, bool editable = true);
    virtual void benchmark();
private:
    virtual bool hasChanged() const override;
    virtual QString toString() const;
    virtual bool setItemData(const QVariant &value, int role) override;
    uchar *m_value;
    uchar m_oldValue;
};

#endif // REGISTERITEM_H
