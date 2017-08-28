#ifndef QCEPTHREAD_H
#define QCEPTHREAD_H

#include "qceplib_global.h"
#include <QThread>
#include <QMutex>
#include "qcepobject-ptr.h"

class QCEP_EXPORT QcepThread : public QThread
{
  Q_OBJECT

public:
  QcepThread(QcepObjectWPtr parent);

  virtual ~QcepThread();

  static void msleep(long unsigned int);

  QcepObjectWPtr parent();

private:
  QcepObjectWPtr m_Parent;
};

#endif // QCEPTHREAD_H
