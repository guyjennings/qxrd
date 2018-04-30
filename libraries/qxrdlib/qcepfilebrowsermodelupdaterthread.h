#ifndef QCEPFILEBROWSERMODELUPDATERTHREAD_H
#define QCEPFILEBROWSERMODELUPDATERTHREAD_H

#include "qxrdlib_global.h"
#include "qcepthread.h"
#include "qcepfilebrowsermodelupdater-ptr.h"
#include "qcepfilebrowsermodel-ptr.h"

class QXRD_EXPORT QcepFileBrowserModelUpdaterThread : public QcepThread
{
  Q_OBJECT

private:
  typedef QcepThread inherited;

public:
  Q_INVOKABLE QcepFileBrowserModelUpdaterThread(QString name);
  virtual ~QcepFileBrowserModelUpdaterThread();

  void haltUpdater();

  void setModel(QcepFileBrowserModelWPtr model);
  QcepFileBrowserModelUpdaterPtr updater() const;

protected:
  void run();

private:
  QcepFileBrowserModelWPtr        m_Model;
  QcepFileBrowserModelUpdaterPtr  m_Updater;
};

#endif // QCEPFILEBROWSERMODELUPDATERTHREAD_H
