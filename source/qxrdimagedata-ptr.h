#ifndef QXRDIMAGEDATA_PTR_H
#define QXRDIMAGEDATA_PTR_H

#include <QSharedPointer>

template <typename T>
class QxrdImageData;

typedef QxrdImageData<quint16> QxrdInt16ImageData;
typedef QxrdImageData<quint32> QxrdInt32ImageData;
typedef QxrdImageData<double>  QxrdDoubleImageData;

typedef QSharedPointer<QxrdInt16ImageData> QxrdInt16ImageDataPtr;
typedef QSharedPointer<QxrdInt32ImageData> QxrdInt32ImageDataPtr;
typedef QSharedPointer<QxrdDoubleImageData> QxrdDoubleImageDataPtr;

#endif
