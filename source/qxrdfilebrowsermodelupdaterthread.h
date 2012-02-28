#ifndef QXRDFILEBROWSERMODELUPDATERTHREAD_H
#define QXRDFILEBROWSERMODELUPDATERTHREAD_H

#include "qxrdthread.h"
#include "qxrdfilebrowsermodelupdater-ptr.h"
#include "qxrdfilebrowsermodel-ptr.h"

class QxrdFileBrowserModelUpdaterThread : public QxrdThread
{
  Q_OBJECT
public:
  explicit QxrdFileBrowserModelUpdaterThread(QxrdFileBrowserModelPtr parent);
  ~QxrdFileBrowserModelUpdaterThread();

  QxrdFileBrowserModelUpdaterPtr updater() const;

  void shutdown();

protected:
  void run();

private:
  QxrdFileBrowserModelWPtr        m_Browser;
  QxrdFileBrowserModelUpdaterPtr  m_Updater;
};

#endif // QXRDFILEBROWSERMODELUPDATERTHREAD_H
