#ifndef QCEPFILEBROWSERMODELUPDATER_H
#define QCEPFILEBROWSERMODELUPDATER_H

#include "qxrdlib_global.h"
#include "qcepobject.h"
#include "qfilesystemwatcher-ptr.h"
#include <QTimer>
#include <QDateTime>
#include <QFileInfo>
#include <QVector>
#include "qcepfilebrowsermodel-ptr.h"
#include "qcepfilebrowsermodel.h"

class QXRD_EXPORT QcepFileBrowserModelUpdater : public QcepObject
{
    Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  explicit QcepFileBrowserModelUpdater(QString name);
  void initialize(QcepObjectWPtr parent);
  virtual ~QcepFileBrowserModelUpdater();

  bool updateNeeded();
  void setBrowserModel(QcepFileBrowserModelWPtr browser);

public slots:
//  void shutdown();
  void haltUpdater();
  void changeRoot(const QString &path);
  void changeContents(const QString &path);
  void updateTimeout();
  void needUpdate();
  void updateContents();
  void generateFileUpdates(int doIt);

private:
  QcepFileBrowserModelWPtr m_BrowserModel;
  QString                  m_RootPath;
  QFileSystemWatcherPtr    m_FileSystemWatcher;
  QAtomicInt               m_UpdateNeeded;
  QTimer                   m_UpdateTimer;
  int                      m_UpdateInterval;
  QVector<QFileInfo>       m_Directories;
  QVector<QFileInfo>       m_Files;
  QDateTime                m_PreviousUpdate;
  QAtomicInt               m_GenerateUpdates;
};

#endif // QCEPFILEBROWSERMODELUPDATER_H
