#ifndef QCEPTHREAD_H
#define QCEPTHREAD_H

#include <QThread>
#include <QMutex>

class QcepThread : public QThread
{
  Q_OBJECT

public:
  QcepThread(QObject *parent = 0);

  static void msleep(long unsigned int);

protected:
  mutable QMutex    m_Mutex;
};

#endif // QCEPTHREAD_H
