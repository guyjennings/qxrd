#ifndef QIMGIMAGEDATAFORMATHIS_H
#define QIMGIMAGEDATAFORMATHIS_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qcepimagedataformat.h"
#include "qcepimagedata-ptr.h"

class QXRD_EXPORT QxrdImageDataFormatHis : public QcepImageDataFormat<double>
{
 public:
  QxrdImageDataFormatHis(QString name = "his");

  QxrdImageDataFormatHis* canLoadFile(QString path);
  QxrdImageDataFormatHis* canSaveFile(QString path);
  QxrdImageDataFormatHis* loadFile(QString path, QcepImageData<double> *img);
  QxrdImageDataFormatHis* saveFile(QString path, QcepImageData<double> *img, int);
  Priority priority() const;

private:
  void checkError(int e);
};

#endif