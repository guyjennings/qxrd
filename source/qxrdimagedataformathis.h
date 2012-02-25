#ifndef QIMGIMAGEDATAFORMATHIS_H
#define QIMGIMAGEDATAFORMATHIS_H

#include "qcepmacros.h"
#include "qcepimagedataformat.h"
template <typename T> class QcepImageData;

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
