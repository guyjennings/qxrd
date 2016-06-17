#include "qcepthread.h"
#include <stdio.h>

QcepThread::QcepThread(QcepObjectWPtr parent) :
  QThread(),
  m_Parent(parent)
{
}

QcepThread::~QcepThread()
{
#ifndef QT_NO_DEBUG
  if (isRunning()) {
    printf("QcepThread %s destroyed while still running\n",
           qPrintable(objectName()));
  }
#endif
}

void QcepThread::msleep(unsigned long t)
{
  QThread::msleep(t);
}

QcepObjectWPtr QcepThread::parent()
{
  return m_Parent;
}
