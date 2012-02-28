#define _CRT_SECURE_NO_WARNINGS

#include "qcepimagedataformattiff.h"
#include "qcepimagedata.h"
#include <tiffio.h>

void qceptiff_warningHandler(const char * /*module*/, const char * /*fmt*/, va_list /*ap*/)
{
}

template <typename T>
QcepImageDataFormatTiff<T>::QcepImageDataFormatTiff(QString name)
  : QcepImageDataFormat<T>(name)
{
  TIFFSetWarningHandler(&qceptiff_warningHandler);
}

template <typename T>
QcepImageDataFormatBase::Priority QcepImageDataFormatTiff<T>::priority() const
{
  return QcepImageDataFormatBase::Versatile;
}

template <typename T>
QcepImageDataFormat<T>* QcepImageDataFormatTiff<T>::canLoadFile(QString path)
{
//   printf("QcepImageDataFormatTiff::canLoadFile(%s)\n", qPrintable(path));
  TIFFHeader header;
  FILE *file = fopen(qPrintable(path), "r");
  QcepImageDataFormat<T>* res = NULL;

  if (file) {
    if (fread(&header, sizeof(header), 1, file) == 1) {
      switch (header.tiff_magic) {
      case TIFF_BIGENDIAN:
      case TIFF_LITTLEENDIAN:
      case MDI_LITTLEENDIAN:
      case MDI_BIGENDIAN:
        res = this;
      }
    }

    fclose(file);
  }

  return res;
}

template <typename T>
T QcepImageDataFormatTiff<T>::unpackSignedBitField(void *buffer, int bitsPerSample, int x, int /*fillOrder*/)
{
  int val=0;
  int samplesPerByte = 8/bitsPerSample;
  int byteIndex = x / samplesPerByte;
  int bitIndex  = x % samplesPerByte;

  switch (bitsPerSample) {
  case 1:
    val = (((quint8*)buffer)[byteIndex] >> bitIndex) & 1;
    if (val >= 1) {
      val = val - 2;
    }
    break;
  case 2:
    val = (((quint8*)buffer)[byteIndex] >> bitIndex) & 3;
    if (val >= 2) {
      val = val - 4;
    }
    break;
  case 4:
    val = (((quint8*)buffer)[byteIndex] >> bitIndex) & 15;
    if (val >= 8) {
      val = val - 16;
    }
    break;
  }

  return val;
}

template <typename T>
T QcepImageDataFormatTiff<T>::unpackUnsignedBitField(void *buffer, int bitsPerSample, int x, int /*fillOrder*/)
{
  int val=0;
  int samplesPerByte = 8/bitsPerSample;
  int byteIndex = x / samplesPerByte;
  int bitIndex  = x % samplesPerByte;

  switch (bitsPerSample) {
  case 1:
    val = (((quint8*)buffer)[byteIndex] >> bitIndex) & 1;
    break;
  case 2:
    val = (((quint8*)buffer)[byteIndex] >> bitIndex) & 3;
    break;
  case 4:
    val = (((quint8*)buffer)[byteIndex] >> bitIndex) & 15;
    break;
  }

  return val;
}

template <typename T>
QcepImageDataFormat<T>* QcepImageDataFormatTiff<T>::loadFile(QString path, QcepImageData<T> *img)
{
  if (img) {
//    QWriteLocker lock(img->rwLock());

    TIFF *file = TIFFOpen(qPrintable(path), "r");

    if (file) {
      quint32 imageWidth = 0;
      quint32 imageHeight = 0;
      quint16 sampleFormat = 0;
      quint16 samplesPerPixel = 0;
      quint16 bitsPerSample = 0;
      quint16 fillOrder = 0;

      if ((TIFFGetFieldDefaulted(file, TIFFTAG_IMAGEWIDTH, &imageWidth)==1) &&
          (TIFFGetFieldDefaulted(file, TIFFTAG_IMAGELENGTH, &imageHeight)==1) &&
          (TIFFGetFieldDefaulted(file, TIFFTAG_SAMPLESPERPIXEL, &samplesPerPixel)==1) &&
          (TIFFGetFieldDefaulted(file, TIFFTAG_BITSPERSAMPLE, &bitsPerSample)==1) &&
          (TIFFGetFieldDefaulted(file, TIFFTAG_SAMPLEFORMAT, &sampleFormat)==1) &&
          (TIFFGetFieldDefaulted(file, TIFFTAG_FILLORDER, &fillOrder)==1)) {

        img -> resize(imageWidth, imageHeight);
        img -> clear();

        size_t scanlineSize = TIFFScanlineSize(file);
//        printf("Scan line size = %d, imageHeight = %d, imageWidth = %d\n", scanlineSize, imageHeight, imageWidth);
        quint8* buffer = (quint8*) malloc(scanlineSize);

        for (quint32 y=0; y<imageHeight; y++) {
          if (TIFFReadScanline(file, buffer, y)==1) {
            for (quint32 x=0; x<imageWidth; x++) {
              switch (sampleFormat) {
              case SAMPLEFORMAT_INT:
                switch (bitsPerSample) {
                case 1:
                case 2:
                case 4:
                  img -> setValue(x,y, unpackSignedBitField(buffer, bitsPerSample, x, fillOrder));
                  break;
                case 8:
                  img -> setValue(x,y, (T)((qint8*) buffer)[x]);
                  break;
                case 16:
                  img -> setValue(x,y, (T)((qint16*) buffer)[x]);
                  break;
                case 32:
                  img -> setValue(x,y, (T)((qint32*) buffer)[x]);
                  break;
                }
                break;
              case SAMPLEFORMAT_UINT:
                switch (bitsPerSample) {
                case 1:
                case 2:
                case 4:
                  img -> setValue(x,y, unpackUnsignedBitField(buffer, bitsPerSample, x, fillOrder));
                  break;
                case 8:
                  img -> setValue(x,y, (T)((quint8*) buffer)[x]);
                  break;
                case 16:
                  img -> setValue(x,y, (T)((quint16*) buffer)[x]);
                  break;
                case 32:
                  img -> setValue(x,y, (T)((quint32*) buffer)[x]);
                  break;
                }
                break;
              case SAMPLEFORMAT_IEEEFP:
                switch (bitsPerSample) {
                case 32:
                  img -> setValue(x,y, (T)((float*) buffer)[x]);
                  break;
                case 64:
                  img -> setValue(x,y, (T)((double*) buffer)[x]);
                  break;
                }
                break;
              }
            }
          }
        }

        free(buffer);
      }

      img -> getTiffMetaData(file);

      TIFFClose(file);

      img -> calculateRange();

      img -> setDefaultFileName(path);

      return this;
    }
  }

  return NULL;
}

template <typename T>
QcepImageDataFormat<T>* QcepImageDataFormatTiff<T>::saveFile(QString /*path*/, QcepImageData<T> * /*img*/)
{
  return NULL;
}

template class QcepImageDataFormatTiff<unsigned short>;
template class QcepImageDataFormatTiff<short>;
template class QcepImageDataFormatTiff<unsigned int>;
template class QcepImageDataFormatTiff<int>;
template class QcepImageDataFormatTiff<double>;
