#ifndef QXRDTESTTHREAD_H
#define QXRDTESTTHREAD_H

#include "qxrdlib_global.h"
#include "qxrdthread.h"

template <class T>
class QXRD_EXPORT QxrdTestThread : public QxrdThread
{
public:
  QxrdTestThread(QcepObjectWPtr parent);
  ~QxrdTestThread();

  void shutdown();

  QSharedPointer<T> object();

protected:
  void run();

private:
  QSharedPointer<T> m_Object;
};

#endif // QXRDTESTTHREAD_H
