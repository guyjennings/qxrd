/******************************************************************
*
*  $Id: qcepimagedataformattiff.h,v 1.2 2010/09/13 20:00:25 jennings Exp $
*
*******************************************************************/

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
  HEADER_IDENT("$Id: qcepimagedataformattiff.h,v 1.2 2010/09/13 20:00:25 jennings Exp $");
};

extern void qceptiff_warningHandler(const char *module, const char *fmt, va_list ap);

template <typename T>
QcepImageDataFormatTiff<T>::QcepImageDataFormatTiff(QString name)
  : QcepImageDataFormat<T>(name)
{
//  TIFFSetWarningHandler(&qceptiff_warningHandler);
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

      if ((TIFFGetFieldDefaulted(file, TIFFTAG_IMAGEWIDTH, &imageWidth)==1) &&
          (TIFFGetFieldDefaulted(file, TIFFTAG_IMAGELENGTH, &imageHeight)==1) &&
          (TIFFGetFieldDefaulted(file, TIFFTAG_SAMPLESPERPIXEL, &samplesPerPixel)==1) &&
          (TIFFGetFieldDefaulted(file, TIFFTAG_BITSPERSAMPLE, &bitsPerSample)==1) &&
          (TIFFGetFieldDefaulted(file, TIFFTAG_SAMPLEFORMAT, &sampleFormat)==1)) {

        img -> resize(imageWidth, imageHeight);
        img -> clear();

        size_t scanlineSize = TIFFScanlineSize(file);
//        printf("Scan line size = %d, imageHeight = %d, imageWidth = %d\n", scanlineSize, imageHeight, imageWidth);
        void* buffer = malloc(scanlineSize);

        for (quint32 y=0; y<imageHeight; y++) {
          if (TIFFReadScanline(file, buffer, y)==1) {
            for (quint32 x=0; x<imageWidth; x++) {
              switch (sampleFormat) {
              case SAMPLEFORMAT_INT:
                switch (bitsPerSample) {
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
#endif

/******************************************************************
*
*  $Log: qcepimagedataformattiff.h,v $
*  Revision 1.2  2010/09/13 20:00:25  jennings
*  Merged
*
*  Revision 1.1.2.2  2010/04/23 20:14:54  jennings
*  Fixed bug with displaying rectangular images - width and height were swapped in QxrdRasterData::QxrdRasterData
*
*  Revision 1.1.2.1  2010/04/13 19:29:12  jennings
*  Added qceplib to cvs
*
*  Revision 1.12  2009/11/09 18:00:35  jennings
*  *** empty log message ***
*
*  Revision 1.11  2009/09/22 20:41:07  jennings
*  Set filename and title properties when loading data files
*
*  Revision 1.10  2009/09/22 18:34:14  jennings
*  Set filename and title properties when loading tiff file
*
*  Revision 1.9  2009/09/11 19:37:51  jennings
*  Added type casts to remove some compiler warnings
*
*  Revision 1.8  2009/09/10 21:36:43  jennings
*  Moved TIFF error handling into main program
*
*  Revision 1.7  2009/09/09 22:32:12  jennings
*  Started to add TIFF metadata support
*
*  Revision 1.6  2009/08/04 16:45:20  jennings
*  Moved mask data into separate class
*
*  Revision 1.5  2009/06/28 16:33:20  jennings
*  Eliminated compiler warnings
*
*  Revision 1.4  2009/06/27 22:50:33  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

