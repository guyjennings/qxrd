#include "qcepthread.h"

QcepThread::QcepThread(QObject *parent) :
  QThread(parent),
  m_Mutex(QMutex::Recursive)
{

}

void QcepThread::msleep(unsigned long t)
{
  QThread::msleep(t);
}

