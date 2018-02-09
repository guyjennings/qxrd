#include "qnewthread.h"
#include "qcepthread.h"

QNewThread::QNewThread(QcepThread *t)
  : QThread(),
    m_CepThread(t)
{
}

int QNewThread::exec()
{
  return QThread::exec();
}

void QNewThread::run()
{
  if (m_CepThread) {
    m_CepThread -> run();
  }
}
