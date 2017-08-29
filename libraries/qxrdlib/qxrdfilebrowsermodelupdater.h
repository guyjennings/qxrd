#ifndef QXRDFILEBROWSERMODELUPDATER_H
#define QXRDFILEBROWSERMODELUPDATER_H

#include "qxrdlib_global.h"
#include "qcepobject.h"
#include <QFileSystemWatcher>
#include <QTimer>
#include <QDateTime>
#include <QFileInfo>
#include <QVector>
#include "qxrdfilebrowsermodel-ptr.h"
#include "qxrdfilebrowsermodel.h"

class QXRD_EXPORT QxrdFileBrowserModelUpdater : public QcepObject
{
    Q_OBJECT
public:
  explicit QxrdFileBrowserModelUpdater(QString name);
  void initialize(QxrdFileBrowserModelWPtr browser, QcepObjectWPtr parent);
  virtual ~QxrdFileBrowserModelUpdater();

  bool updateNeeded();
  void setBrowserModel(QxrdFileBrowserModelWPtr browser);

public slots:
//  void shutdown();
  void changeRoot(const QString &path);
  void changeContents(const QString &path);
  void updateTimeout();
  void needUpdate();
  void updateContents();
  void generateFileUpdates(int doIt);

private:
  QxrdFileBrowserModelWPtr m_BrowserModel;
  QString                  m_RootPath;
  QFileSystemWatcher      *m_FileSystemWatcher;
  QAtomicInt               m_UpdateNeeded;
  QTimer                   m_UpdateTimer;
  int                      m_UpdateInterval;
  QVector<QFileInfo>       m_Directories;
  QVector<QFileInfo>       m_Files;
  QDateTime                m_PreviousUpdate;
  QAtomicInt               m_GenerateUpdates;
};

#endif // QXRDFILEBROWSERMODELUPDATER_H
