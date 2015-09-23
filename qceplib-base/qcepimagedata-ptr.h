#ifndef QCEPIMAGEDATA_PTR_H
#define QCEPIMAGEDATA_PTR_H

#include <QSharedPointer>

class QcepImageDataBase;
template <typename T> class QcepImageData;

typedef QcepImageData<quint16> QcepInt16ImageData;
typedef QcepImageData<quint32> QcepInt32ImageData;
typedef QcepImageData<double>  QcepDoubleImageData;

typedef QSharedPointer<QcepImageDataBase>  QcepImageDataBasePtr;
typedef QSharedPointer<QcepInt16ImageData> QcepInt16ImageDataPtr;
typedef QSharedPointer<QcepInt32ImageData> QcepInt32ImageDataPtr;
typedef QSharedPointer<QcepDoubleImageData> QcepDoubleImageDataPtr;

typedef QWeakPointer<QcepImageDataBase> QcepImageDataBaseWPtr;
typedef QWeakPointer<QcepInt16ImageData> QcepInt16ImageDataWPtr;
typedef QWeakPointer<QcepInt32ImageData> QcepInt32ImageDataWPtr;
typedef QWeakPointer<QcepDoubleImageData> QcepDoubleImageDataWPtr;

#endif
