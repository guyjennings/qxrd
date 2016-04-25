#ifndef QXRDTHREAD_H
#define QXRDTHREAD_H

#include "qcepthread.h"
#include "qcepobject-ptr.h"

class QxrdThread : public QcepThread
{
  Q_OBJECT

public:
  QxrdThread(QcepObjectWPtr      parent);

  virtual void shutdown() = 0;
};

#endif // QXRDTHREAD_H
