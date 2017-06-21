#ifndef QCEPIMAGEDATAFORMATTIFF_H
#define QCEPIMAGEDATAFORMATTIFF_H

#include "qcepmacros.h"

#include "qcepimagedataformat.h"

#include <tiffio.h>

template <typename T>
class QcepImageDataFormatTiff : public QcepImageDataFormat<T>
{
public:
  QcepImageDataFormatTiff(QString name);

  QcepImageDataFormat<T>* canLoadFile(QString path);
  QcepImageDataFormat<T>* canSaveFile(QString path);
  QcepImageDataFormat<T>* loadFile(QString path, QcepImageData<T> *img);
  QcepImageDataFormat<T>* saveFile(QString path, QcepImageData<T> *img, int canOverwrite);
  QcepImageDataFormatBase::Priority priority() const;

private:
  void setTiffMetaData(TIFF *tif, QcepImageData<T> *img);
  void getTiffMetaData(TIFF *tif, QcepImageData<T> *img);

  T unpackSignedBitField(void *buffer, int bitsPerSample, int x, int fillOrder);
  T unpackUnsignedBitField(void *buffer, int bitsPerSample, int x, int fillOrder);
};

extern void qceptiff_warningHandler(const char *module, const char *fmt, va_list ap);
extern QString qcepTIFFVersion();

#endif /* QCEPIMAGEDATAFORMATTIFF_H */

