#ifndef LOADDIALOG_H
#define LOADDIALOG_H

#include <QDialog>
#include <QStyledItemDelegate>

namespace Ui {
class LoadDialog;
}

class HexDelegate : public QStyledItemDelegate {
 Q_OBJECT

 public:
  using QStyledItemDelegate::QStyledItemDelegate;

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
  void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
};

class LoadDialog : public QDialog {
 Q_OBJECT
 public:
  struct FileLoadInfo {
    QString filename;
    size_t address;
    size_t offset;
    size_t length;
  };

  explicit LoadDialog(QWidget *parent = nullptr);
  ~LoadDialog() override;

  QVector<FileLoadInfo> getFileLoadInfo();

 public slots:
  void onCellDoubleClicked(int row, int column);
  void onCellChanged(int row, int column);
  void onAddButtonClicked();
  void onRemoveButtonClicked();
  void onClearButtonClicked();
  void onItemSelectionChanged();

 private:
  void init();
  void addRow();

  Ui::LoadDialog *ui{};
};

#endif // LOADDIALOG_H
