#include <QFileIconProvider>
#include <QLabel>
#include <QTableWidgetItem>
#include <QDebug>
#include <QMessageBox>
#include <QMetaProperty>
#include <QLineEdit>
#include <QFileDialog>

#include "loaddialog.h"
#include "ui_loaddialog.h"

LoadDialog::LoadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadDialog) {
  ui->setupUi(this);
  init();
}

LoadDialog::~LoadDialog() {
  delete ui;
}

QVector<LoadDialog::FileLoadInfo> LoadDialog::getFileLoadInfo() {
  QVector<LoadDialog::FileLoadInfo> result;
  for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
    bool ok;
    auto filename = ui->tableWidget->item(row, 0)->text();
    auto address = ui->tableWidget->item(row, 1)->text().toUInt(&ok, 16);
    if (!ok)
      continue;
    auto offset = ui->tableWidget->item(row, 1)->text().toUInt(&ok, 16);
    if (!ok)
      continue;
    auto length = ui->tableWidget->item(row, 1)->text().toUInt(&ok, 16);
    if (!ok)
      continue;

    result.push_back(LoadDialog::FileLoadInfo{filename, address, offset, length});
  }
  return result;
}

void LoadDialog::init() {
  ui->tableWidget->setRowCount(0);
  ui->tableWidget->setColumnCount(4);
  ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "File" << "Address" << "File Offset" << "File Length");

  ui->tableWidget->setItemDelegateForColumn(1, new HexDelegate());
  ui->tableWidget->setItemDelegateForColumn(2, new HexDelegate());
  ui->tableWidget->setItemDelegateForColumn(3, new HexDelegate());

  connect(ui->addButton, &QAbstractButton::clicked, this, &LoadDialog::onAddButtonClicked);
  connect(ui->removeButton, &QAbstractButton::clicked, this, &LoadDialog::onRemoveButtonClicked);
  connect(ui->clearButton, &QAbstractButton::clicked, this, &LoadDialog::onClearButtonClicked);

  connect(ui->tableWidget, &QTableWidget::itemSelectionChanged, this, &LoadDialog::onItemSelectionChanged);

  connect(ui->tableWidget, &QTableWidget::cellDoubleClicked, this, &LoadDialog::onCellDoubleClicked);
  connect(ui->tableWidget, &QTableWidget::cellChanged, this, &LoadDialog::onCellChanged);
  addRow();
}

void LoadDialog::addRow() {
  QFileIconProvider iconProvider;
  auto fileWidget = new QTableWidgetItem(iconProvider.icon(QFileIconProvider::Folder), "");
  fileWidget->setFlags({Qt::ItemIsSelectable, Qt::ItemIsEnabled});
  auto rowCount = ui->tableWidget->rowCount();
  ui->tableWidget->insertRow(rowCount);
  ui->tableWidget->setItem(rowCount, 0, fileWidget);

  auto addrWidget = new QTableWidgetItem("0000");
  addrWidget->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

  ui->tableWidget->setItem(rowCount, 1, addrWidget);

  auto offsetWidget = new QTableWidgetItem();
  offsetWidget->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

  ui->tableWidget->setItem(rowCount, 2, offsetWidget);

  auto lengthWidget = new QTableWidgetItem();
  lengthWidget->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

  ui->tableWidget->setItem(rowCount, 3, lengthWidget);
}

void LoadDialog::onCellDoubleClicked(int row, int column) {
  qDebug() << QString("DoubleClicked %1, %2").arg(row).arg(column);
  if (column == 0) {
    auto filename = QFileDialog::getOpenFileName(this,
                                                 "Open Image", "", "Image Files (*.img *.bin);;(*.*)");
    if (!filename.isEmpty()) {
      ui->tableWidget->item(row, column)->setText(filename);
    }
  }
}

void LoadDialog::onCellChanged(int row, int column) {
  qDebug() << QString("Changed %1, %2").arg(row).arg(column);
}

void LoadDialog::onAddButtonClicked() {
  addRow();
}

void LoadDialog::onRemoveButtonClicked() {
  auto items = ui->tableWidget->selectedItems();
  if (!items.empty()) {
    ui->tableWidget->removeRow(items[0]->row());
  }
}

void LoadDialog::onClearButtonClicked() {
  auto reply = QMessageBox::question(this, "Clear all files?", "Are you sure you want to clear all files?");
  if (reply == QMessageBox::Yes) {
    ui->tableWidget->setRowCount(0);
  }
}

void LoadDialog::onItemSelectionChanged() {
  ui->removeButton->setEnabled(!ui->tableWidget->selectedItems().empty());
}

QWidget *HexDelegate::createEditor(QWidget *parent,
                                   const QStyleOptionViewItem &option,
                                   const QModelIndex &index) const {
  auto widget = QStyledItemDelegate::createEditor(parent, option, index);
  auto editor = qobject_cast<QLineEdit *>(widget);
  if (editor) {
    editor->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    return editor;
  }
  return widget;
}

void HexDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
  QByteArray n = editor->metaObject()->userProperty().name();
  if (!n.isEmpty()) {
    auto strValue = editor->property(n).toString();
    bool ok;
    strValue.toUInt(&ok, 16);
    if (ok) {
      model->setData(index, strValue.rightJustified(4, '0'), Qt::EditRole);
    }
  }

}
