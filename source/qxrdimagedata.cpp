#include "qxrdimagedata.h"

static QAtomicInt s_ImageDataObjectCounter;

QxrdAllocatorInterface::QxrdAllocatorInterface(QObject *parent)
  : QObject(parent)
{
}

QxrdImageDataObjectCounter::QxrdImageDataObjectCounter()
{
  s_ImageDataObjectCounter.fetchAndAddOrdered(1);
}

QxrdImageDataObjectCounter::~QxrdImageDataObjectCounter()
{
  s_ImageDataObjectCounter.fetchAndAddOrdered(-1);
}

int QxrdImageDataObjectCounter::value()
{
  return s_ImageDataObjectCounter.fetchAndAddOrdered(0);
}

