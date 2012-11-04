#include "qxrdthread.h"

QxrdThread::QxrdThread(QObject *parent) :
  QThread(parent),
  m_Mutex(QMutex::Recursive)
{
}
