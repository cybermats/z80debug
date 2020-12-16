#include <QDebug>
#include "registermodel.h"

RegisterModel::RegisterModel(Z80Context *ctx, QObject *parent)
    : QAbstractItemModel(parent),
      m_ctx(ctx) {
  m_rootItem = new RegisterItem("Root");
  setupModelData(m_ctx, m_rootItem);
  m_rootItem->benchmark();
}

RegisterModel::~RegisterModel() {
  delete m_rootItem;
}

QModelIndex RegisterModel::index(int row, int column, const QModelIndex &parent) const {
  if (!hasIndex(row, column, parent))
    return QModelIndex();

  auto parentItem = getItem(parent);
  if (!parentItem)
    return QModelIndex();

  auto childItem = parentItem->child(row);
  if (childItem)
    return createIndex(row, column, childItem);

  if (!hasIndex(row, column, parent))
    return QModelIndex();

  return QModelIndex();

}
QModelIndex RegisterModel::parent(const QModelIndex &child) const {
  if (!child.isValid())
    return QModelIndex();

  auto childItem = getItem(child);
  auto parentItem = childItem ? childItem->parentItem() : nullptr;

  if (parentItem == m_rootItem || !parentItem)
    return QModelIndex();

  return createIndex(parentItem->childNumber(), 0, parentItem);
}
int RegisterModel::rowCount(const QModelIndex &parent) const {
  auto parentItem = getItem(parent);
  return parentItem ? parentItem->childCount() : 0;
}
int RegisterModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return m_rootItem->columnCount();
}
QVariant RegisterModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid())
    return QVariant();

  auto item = getItem(index);
  auto result = item->data(index.column(), role);
  return result;

}
bool RegisterModel::setData(const QModelIndex &index, const QVariant &value, int role) {
  if (role != Qt::EditRole)
    return false;

  auto item = getItem(index);
  bool result = item->setData(index.column(), value, role);

  if (result)
      emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});

  return result;
}
QVariant RegisterModel::headerData(int section, Qt::Orientation orientation, int role) const {
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
    if (section == 0)
      return QString("Register");
    else
      return QString("Value");
  }
  return QVariant();
}
Qt::ItemFlags RegisterModel::flags(const QModelIndex &index) const {
  if (!index.isValid())
    return Qt::NoItemFlags;

  auto item = getItem(index);
  if (index.column() > 0 && item && item->editable()) {

    return Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
  }
  return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

void RegisterModel::update() {
  emit dataChanged(createIndex(0, 0), createIndex(2, 2));
}

void RegisterModel::benchmark() {
  m_rootItem->benchmark();
}

void RegisterModel::setupModelData(Z80Context *ctx, RegisterItem *parent) {
  RegisterItem *top = new RegisterItem("Main", parent);
  parent->appendChild(top);

  RegisterItem *reg = new ShortRegisterItem("AF", &ctx->R1.wr.AF, top);
  top->appendChild(reg);
  reg->appendChild(new ByteRegisterItem("A", &ctx->R1.br.A, reg));
  reg->appendChild(new FlagRegisterItem("F", &ctx->R1.br.F, reg));

  reg = new ShortRegisterItem("HL", &ctx->R1.wr.HL, top);
  top->appendChild(reg);
  reg->appendChild(new ByteRegisterItem("H", &ctx->R1.br.H, reg));
  reg->appendChild(new ByteRegisterItem("L", &ctx->R1.br.L, reg));

  reg = new ShortRegisterItem("BC", &ctx->R1.wr.BC, top);
  top->appendChild(reg);
  reg->appendChild(new ByteRegisterItem("B", &ctx->R1.br.B, reg));
  reg->appendChild(new ByteRegisterItem("C", &ctx->R1.br.C, reg));

  reg = new ShortRegisterItem("DE", &ctx->R1.wr.DE, top);
  top->appendChild(reg);
  reg->appendChild(new ByteRegisterItem("D", &ctx->R1.br.D, reg));
  reg->appendChild(new ByteRegisterItem("E", &ctx->R1.br.E, reg));

  top = new RegisterItem("Alternate", parent);
  parent->appendChild(top);

  reg = new ShortRegisterItem("'AF", &ctx->R2.wr.AF, top);
  top->appendChild(reg);
  reg->appendChild(new ByteRegisterItem("'A", &ctx->R2.br.A, reg));
  reg->appendChild(new FlagRegisterItem("'F", &ctx->R2.br.F, reg));

  reg = new ShortRegisterItem("'HL", &ctx->R2.wr.HL, top);
  top->appendChild(reg);
  reg->appendChild(new ByteRegisterItem("'H", &ctx->R2.br.H, reg));
  reg->appendChild(new ByteRegisterItem("'L", &ctx->R2.br.L, reg));

  reg = new ShortRegisterItem("'BC", &ctx->R2.wr.BC, top);
  top->appendChild(reg);
  reg->appendChild(new ByteRegisterItem("'B", &ctx->R2.br.B, reg));
  reg->appendChild(new ByteRegisterItem("'C", &ctx->R2.br.C, reg));

  reg = new ShortRegisterItem("'DE", &ctx->R2.wr.DE, top);
  top->appendChild(reg);
  reg->appendChild(new ByteRegisterItem("'D", &ctx->R2.br.D, reg));
  reg->appendChild(new ByteRegisterItem("'E", &ctx->R2.br.E, reg));

  top = new RegisterItem("Special", parent);
  parent->appendChild(top);

  reg = new ShortRegisterItem("PC", &ctx->PC, top);
  top->appendChild(reg);

  reg = new ShortRegisterItem("SP", &ctx->R1.wr.SP, top);
  top->appendChild(reg);

  reg = new ShortRegisterItem("IX", &ctx->R1.wr.IX, top);
  top->appendChild(reg);

  reg = new ShortRegisterItem("IY", &ctx->R1.wr.IY, top);
  top->appendChild(reg);

  reg = new ByteRegisterItem("I", &ctx->I, top);
  top->appendChild(reg);

  reg = new ByteRegisterItem("R", &ctx->R, top);
  top->appendChild(reg);

  top = new RegisterItem("Misc", parent);
  parent->appendChild(top);

  reg = new IntRegisterItem("T State", &ctx->tstates, top, false);
  top->appendChild(reg);

  reg = new ByteRegisterItem("Halted", &ctx->halted, top, false);
  top->appendChild(reg);

  top = new RegisterItem("Interrupt", parent);
  parent->appendChild(top);

  reg = new ByteRegisterItem("IFF1", &ctx->IFF1, top);
  top->appendChild(reg);

  reg = new ByteRegisterItem("IFF1", &ctx->IFF1, top);
  top->appendChild(reg);
}

RegisterItem *RegisterModel::getItem(const QModelIndex &index) const {
  if (index.isValid()) {
    RegisterItem *item = static_cast<RegisterItem *>(index.internalPointer());
    if (item)
      return item;
  }
  return m_rootItem;
}

