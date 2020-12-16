#ifndef REGISTERMODEL_H
#define REGISTERMODEL_H

#include <QAbstractItemModel>
#include <QStringBuilder>
#include "registeritem.h"

extern "C" {
#include "z80.h"
}

class RegisterModel : public QAbstractItemModel {
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

  Z80Context *m_ctx;
  RegisterItem *m_rootItem;
};

#endif // REGISTERMODEL_H
