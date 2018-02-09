#ifndef QNEWTHREAD_H
#define QNEWTHREAD_H

#include <QThread>
#include "qcepthread-ptr.h"

class QNewThread : public QThread
{
public:
  QNewThread(QcepThread *t);

  int exec();
  void run();

private:
  QcepThread *m_CepThread;
};

#endif // QNEWTHREAD_H
