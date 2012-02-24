#ifndef QIMGIMAGEDATAFORMATHIS_H
#define QIMGIMAGEDATAFORMATHIS_H

#include "qcepmacros.h"

#include "qcepimagedata.h"
#include "qcepimagedataformat.h"

class QxrdImageDataFormatHis : public QcepImageDataFormat<double>
{
 public:
  QxrdImageDataFormatHis(QString name = "his");

  QxrdImageDataFormatHis* canLoadFile(QString path);
  QxrdImageDataFormatHis* loadFile(QString path, QcepImageData<double> *img);
  QxrdImageDataFormatHis* saveFile(QString path, QcepImageData<double> *img);
  Priority priority() const;
};

#endif

class QxrdImageDataFormatHis;
