#ifndef QXRDTHREAD_H
#define QXRDTHREAD_H

#include "qxrdlib_global.h"
#include "qcepthread.h"
#include "qcepobject-ptr.h"

class QXRD_EXPORT QxrdThread : public QcepThread
{
  Q_OBJECT

private:
  typedef QcepThread inherited;

public:
  QxrdThread(QString name);
};

#endif // QXRDTHREAD_H
