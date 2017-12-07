#ifndef QCEPIMAGEDATA_PTR_H
#define QCEPIMAGEDATA_PTR_H

#include "qcepsharedpointer.h"

QCEP_SHAREDPOINTER(QcepImageDataBase)

template <typename T> class QcepImageData;

QCEP_SHAREDPOINTER(QcepUInt16ImageData)
QCEP_SHAREDPOINTER(QcepInt16ImageData)
QCEP_SHAREDPOINTER(QcepUInt32ImageData)
QCEP_SHAREDPOINTER(QcepInt32ImageData)
QCEP_SHAREDPOINTER(QcepDoubleImageData)
QCEP_SHAREDPOINTER(QcepFloatImageData)

#endif
