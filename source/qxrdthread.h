#ifndef QXRDTHREAD_H
#define QXRDTHREAD_H

#include <QThread>
#include <QMutex>

class QxrdThread : public QThread
{
  Q_OBJECT
public:
  explicit QxrdThread(QObject *parent = 0);

  virtual void shutdown() = 0;

protected:
  mutable QMutex    m_Mutex;
};

#endif // QXRDTHREAD_H
