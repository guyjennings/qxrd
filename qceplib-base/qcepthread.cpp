#include "qcepthread.h"
#include <stdio.h>

QcepThread::QcepThread(QObject *parent) :
  QThread(parent)
{
}

QcepThread::~QcepThread()
{
#ifndef QT_NODEBUG
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

