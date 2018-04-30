#ifndef QCEPFILEBROWSERMODEL_H
#define QCEPFILEBROWSERMODEL_H

#include "qxrdlib_global.h"
#include "qcepobject.h"
#include <QAbstractTableModel>
#include <QVector>
#include <QFileInfo>
#include <QStringList>
#include <QDateTime>
#include <QMutex>

#include "qcepfilebrowsermodelupdaterthread-ptr.h"
#include "qcepfilebrowsermodelupdater-ptr.h"
#include "qcepfilebrowsermodel-ptr.h"

class QXRD_EXPORT QcepFileBrowserModel : public QAbstractTableModel, public QEnableSharedFromThis<QcepFileBrowserModel>
{
  Q_OBJECT

public:
  explicit QcepFileBrowserModel(QString name);
  ~QcepFileBrowserModel();
  void initialize(QcepObjectWPtr parent);

  void haltFileBrowser();

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
  QString rootPath() const;

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
  QcepObjectWPtr     m_Parent;
  mutable QMutex     m_Mutex;
  QcepFileBrowserModelUpdaterThreadPtr m_UpdaterThread;
  QcepFileBrowserModelUpdaterWPtr m_Updater;
  QString            m_RootPath;
  QStringList        m_NameFilters;
  QVector<QFileInfo> m_DirList;
  QVector<QFileInfo> m_FileList;
  int                m_SortedColumn;
  Qt::SortOrder      m_SortOrder;
  int                m_Limit;
  int                m_TrueSize;
  double             m_HighlightOnTime;
  double             m_HighlightFadeTime;
  int                m_HighlightSaturation;
  int                m_HighlightHue;
};

#endif // QCEPFILEBROWSERMODEL_H
