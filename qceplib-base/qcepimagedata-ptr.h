#ifndef QCEPIMAGEDATA_PTR_H
#define QCEPIMAGEDATA_PTR_H

#include <QSharedPointer>

class QcepImageDataBase;
template <typename T> class QcepImageData;

class QcepUInt16ImageData;
class QcepInt16ImageData;
class QcepUInt32ImageData;
class QcepInt32ImageData;
class QcepDoubleImageData;
class QcepFloatImageData;

typedef QSharedPointer<QcepImageDataBase>  QcepImageDataBasePtr;
typedef QSharedPointer<QcepUInt16ImageData> QcepUInt16ImageDataPtr;
typedef QSharedPointer<QcepUInt32ImageData> QcepUInt32ImageDataPtr;
typedef QSharedPointer<QcepDoubleImageData> QcepDoubleImageDataPtr;

typedef QWeakPointer<QcepImageDataBase> QcepImageDataBaseWPtr;
typedef QWeakPointer<QcepUInt16ImageData> QcepUInt16ImageDataWPtr;
typedef QWeakPointer<QcepUInt32ImageData> QcepUInt32ImageDataWPtr;
typedef QWeakPointer<QcepDoubleImageData> QcepDoubleImageDataWPtr;

#endif
