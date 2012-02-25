#include "qcepimagedataformat.h"
#include "qcepimagedataformatfactory.h"

template <typename T>
QcepImageDataFormat<T>::QcepImageDataFormat(QString fmtname)
  : QcepImageDataFormatBase(fmtname)
{
//    printf("QcepImageDataFormatFactory<T>::factory()->insert(%s)\n", qPrintable(name()));
//
  QcepImageDataFormatFactory<T>::factory()->insert(this);
}

template class QcepImageDataFormat<unsigned short>;
template class QcepImageDataFormat<short>;
template class QcepImageDataFormat<unsigned int>;
template class QcepImageDataFormat<int>;
template class QcepImageDataFormat<double>;
