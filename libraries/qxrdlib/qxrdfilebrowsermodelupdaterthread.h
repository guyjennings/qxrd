#ifndef QXRDFILEBROWSERMODELUPDATERTHREAD_H
#define QXRDFILEBROWSERMODELUPDATERTHREAD_H

#include "qxrdlib_global.h"
#include "qxrdthread.h"
#include "qxrdfilebrowsermodelupdater-ptr.h"
#include "qxrdfilebrowsermodel-ptr.h"

class QXRD_EXPORT QxrdFileBrowserModelUpdaterThread : public QxrdThread
{
  Q_OBJECT
public:
  explicit QxrdFileBrowserModelUpdaterThread(QcepObjectWPtr parent);
  virtual ~QxrdFileBrowserModelUpdaterThread();

  void setModel(QxrdFileBrowserModelWPtr model);
  QxrdFileBrowserModelUpdaterPtr updater() const;

  void shutdown();

protected:
  void run();

private:
  QxrdFileBrowserModelWPtr        m_Model;
  QxrdFileBrowserModelUpdaterPtr  m_Updater;
};

#endif // QXRDFILEBROWSERMODELUPDATERTHREAD_H
