#ifndef QXRDTHREAD_H
#define QXRDTHREAD_H

#include "qcepthread.h"

class QxrdThread : public QcepThread
{
  Q_OBJECT

public:
  explicit QxrdThread(QObject *parent = 0);

  virtual void shutdown() = 0;
};

#endif // QXRDTHREAD_H
