#ifndef QCEPTHREAD_H
#define QCEPTHREAD_H

#include <QThread>
#include <QMutex>

class QcepThread : public QThread
{
  Q_OBJECT

public:
  QcepThread(QObject *parent = 0);
  virtual ~QcepThread();

  static void msleep(long unsigned int);
};

#endif // QCEPTHREAD_H
