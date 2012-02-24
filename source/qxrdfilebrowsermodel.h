#ifndef QXRDFILEBROWSERMODEL_H
#define QXRDFILEBROWSERMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QFileInfo>
#include <QStringList>
#include <QDateTime>

#include "qxrdfilebrowsermodelupdaterthread.h"
#include "qxrdfilebrowsermodelupdater.h"

class QxrdFileBrowserModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  explicit QxrdFileBrowserModel(QObject *parent=0);
  ~QxrdFileBrowserModel();
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

  QStringList nameFilters() const;
  int sortedColumn() const;
  Qt::SortOrder sortOrder() const;

  void generateFileUpdates(int doIt);

public slots:
  void newDataAvailable(QVector<QFileInfo> dirs, QVector<QFileInfo> files, int limit=0, int trueSize=-1);
  void updatedFile(QFileInfo file);

signals:
  void rootChanged(const QString& path);
  void fileUpdated(QFileInfo file);

private:
  void updateModel();

private:
  QxrdFileBrowserModelUpdaterThread *m_UpdaterThread;
  QxrdFileBrowserModelUpdater *m_Updater;
  QString            m_RootPath;
  QStringList        m_NameFilters;
  QVector<QFileInfo> m_DirList;
  QVector<QFileInfo> m_FileList;
  int                m_SortedColumn;
  Qt::SortOrder      m_SortOrder;
  int                m_Limit;
  int                m_TrueSize;
  int                m_HighlightOnTime;
  int                m_HighlightFadeTime;
  int                m_HighlightSaturation;
  int                m_HighlightHue;
};

#endif // QXRDFILEBROWSERMODEL_H

class QxrdFileBrowserModel;
