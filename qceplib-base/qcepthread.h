#ifndef QCEPTHREAD_H
#define QCEPTHREAD_H

#include "qceplib_global.h"
//#include <QThread>
//#include <QMutex>
#include "qcepobject-ptr.h"
#include "qcepobject.h"
#include "qnewthread-ptr.h"

class QCEP_EXPORT QcepThread : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  QcepThread(QcepObjectWPtr parent);

  virtual ~QcepThread();

  static void msleep(long unsigned int);

  QcepObjectWPtr parent();

  bool isRunning() const;
  int exec();
  void exit(int retcode = 0);
  bool wait();
  void start();
  void quit();

  virtual void run() = 0;

  void setObjectName(const QString &name);

private:
  QcepObjectWPtr m_Parent;
  QNewThreadPtr  m_Thread;
};

#endif // QCEPTHREAD_H
