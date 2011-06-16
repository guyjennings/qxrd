#ifndef QXRDFILEBROWSERMODELUPDATER_H
#define QXRDFILEBROWSERMODELUPDATER_H

#include <QObject>
#include <QFileSystemWatcher>
#include <QTimer>
#include "qxrdfilebrowsermodel.h"

class QxrdFileBrowserModelUpdater : public QObject
{
    Q_OBJECT
public:
  explicit QxrdFileBrowserModelUpdater(QxrdFileBrowserModel *browser, QObject *parent = 0);

  bool updateNeeded();

public slots:
  void shutdown();
  void changeRoot(const QString &path);
  void changeContents(const QString &path);
  void updateTimeout();
  void needUpdate();
  void updateContents();

private:
  QxrdFileBrowserModel    *m_BrowserModel;
  QString                  m_RootPath;
  QFileSystemWatcher      *m_FileSystemWatcher;
  QAtomicInt               m_UpdateNeeded;
  QTimer                   m_UpdateTimer;
  int                      m_UpdateInterval;
  QVector<QFileInfo>       m_Directories;
  QVector<QFileInfo>       m_Files;
};

#endif // QXRDFILEBROWSERMODELUPDATER_H
