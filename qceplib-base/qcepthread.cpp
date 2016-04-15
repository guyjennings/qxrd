#include "qcepthread.h"

QcepThread::QcepThread(QObject *parent) :
  QThread(parent)
{
}

void QcepThread::msleep(unsigned long t)
{
  QThread::msleep(t);
}

