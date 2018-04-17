#ifndef QXRDFILEBROWSERMODELUPDATERTHREAD_H
#define QXRDFILEBROWSERMODELUPDATERTHREAD_H

#include "qxrdlib_global.h"
#include "qcepthread.h"
#include "qxrdfilebrowsermodelupdater-ptr.h"
#include "qxrdfilebrowsermodel-ptr.h"

class QXRD_EXPORT QxrdFileBrowserModelUpdaterThread : public QcepThread
{
  Q_OBJECT

private:
  typedef QcepThread inherited;

public:
  Q_INVOKABLE QxrdFileBrowserModelUpdaterThread(QString name);
  virtual ~QxrdFileBrowserModelUpdaterThread();

  void setModel(QxrdFileBrowserModelWPtr model);
  QxrdFileBrowserModelUpdaterPtr updater() const;

protected:
  void run();

private:
  QxrdFileBrowserModelWPtr        m_Model;
  QxrdFileBrowserModelUpdaterPtr  m_Updater;
};

#endif // QXRDFILEBROWSERMODELUPDATERTHREAD_H
