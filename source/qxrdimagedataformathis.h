#ifndef QIMGIMAGEDATAFORMATHIS_H
#define QIMGIMAGEDATAFORMATHIS_H

#include "qcepmacros.h"
#include "qcepimagedataformat.h"
#include "qcepimagedata-ptr.h"

class QxrdImageDataFormatHis : public QcepImageDataFormat<double>
{
 public:
  QxrdImageDataFormatHis(QString name = "his");

  QxrdImageDataFormatHis* canLoadFile(QString path);
  QxrdImageDataFormatHis* canSaveFile(QString path);
  QxrdImageDataFormatHis* loadFile(QString path, QcepImageData<double> *img);
  QxrdImageDataFormatHis* saveFile(QString path, QcepImageData<double> *img, int);
  Priority priority() const;
};

#endif
