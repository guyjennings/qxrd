#ifndef QXRDFILEBROWSERMODEL_H
#define QXRDFILEBROWSERMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QFileInfo>
#include <QStringList>

class QxrdFileBrowserModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  explicit QxrdFileBrowserModel(QObject *parent=0);
  typedef QAbstractTableModel inherited;

  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
  QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  void setNameFilters(QStringList filters);
  void setNameFilterDisables(bool disables);

  QFileInfo fileInfo(const QModelIndex &index) const;
  QString fileName(const QModelIndex &index) const;
  QString filePath(const QModelIndex &index) const;

  void setRootPath(QString path);

  bool isDir(const QModelIndex &index) const;

  virtual void sort ( int column, Qt::SortOrder order = Qt::AscendingOrder );
  void refresh();

private:
  void updateModel();

private:
  QString            m_RootPath;
  QStringList        m_NameFilters;
  QVector<QFileInfo> m_DirList;
  QVector<QFileInfo> m_FileList;
};

#endif // QXRDFILEBROWSERMODEL_H
