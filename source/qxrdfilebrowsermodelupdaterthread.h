#ifndef QXRDFILEBROWSERMODELUPDATERTHREAD_H
#define QXRDFILEBROWSERMODELUPDATERTHREAD_H

#include "qxrdthread.h"
#include "qxrdfilebrowsermodel.h"

class QxrdFileBrowserModelUpdaterThread : public QxrdThread
{
  Q_OBJECT
public:
  explicit QxrdFileBrowserModelUpdaterThread(QxrdFileBrowserModel *parent = 0);
  ~QxrdFileBrowserModelUpdaterThread();

  QxrdFileBrowserModelUpdater *updater() const;

  void shutdown();

protected:
  void run();

private:
  QxrdFileBrowserModel                       *m_Browser;
  QAtomicPointer<QxrdFileBrowserModelUpdater> m_Updater;
};

#endif // QXRDFILEBROWSERMODELUPDATERTHREAD_H
