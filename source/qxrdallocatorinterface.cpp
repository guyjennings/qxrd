#include "qxrdallocatorinterface.h"
#include "qcepdebug.h"
#include <stdio.h>

QxrdAllocatorInterface::QxrdAllocatorInterface(QObject *parent)
  : QObject(parent)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAllocatorInterface::QxrdAllocatorInterface\n");
  }
}

QxrdAllocatorInterface::~QxrdAllocatorInterface()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAllocatorInterface::~QxrdAllocatorInterface\n");
  }
}
