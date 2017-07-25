#ifndef QXRDTESTTHREAD_H
#define QXRDTESTTHREAD_H

#include "qxrdthread.h"

template <class T>
class QxrdTestThread : public QxrdThread
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
