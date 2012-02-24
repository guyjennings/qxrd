#ifndef QCEPIMAGEDATAFORMATTIF_H
#define QCEPIMAGEDATAFORATTIFF_H

#include "qcepmacros.h"

#include <tiffio.h>
#include <QWriteLocker>

#include "qcepimagedataformat.h"

template <typename T>
class QcepImageDataFormatTiff : public QcepImageDataFormat<T>
{
public:
  QcepImageDataFormatTiff(QString name);

  QcepImageDataFormat<T>* canLoadFile(QString path);
  QcepImageDataFormat<T>* loadFile(QString path, QcepImageData<T> *img);
  QcepImageDataFormat<T>* saveFile(QString path, QcepImageData<T> *img);
  QcepImageDataFormatBase::Priority priority() const;

private:
  T unpackSignedBitField(void *buffer, int bitsPerSample, int x, int fillOrder);
  T unpackUnsignedBitField(void *buffer, int bitsPerSample, int x, int fillOrder);
};

extern void qceptiff_warningHandler(const char *module, const char *fmt, va_list ap);

#endif

template <typename T> class QcepImageDataFormatTiff;
